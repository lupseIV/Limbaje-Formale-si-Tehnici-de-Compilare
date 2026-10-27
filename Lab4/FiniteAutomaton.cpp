#include "FiniteAutomaton.h"
#include <algorithm>
#include <sstream>
#include <stdexcept>
using namespace std;

static bool isKeyword(const string& word) {
    return word == "stari" || word == "alfabet" || word == "start" || word == "final";
}

// "a" -> {'a'}, "a-f" -> {'a', ..., 'f'}
static vector<char> expandSymbols(const string& token, const string& where) {
    if (token.size() == 1)
        return {token[0]};
    if (token.size() == 3 && token[1] == '-') {
        if (token[0] > token[2])
            throw runtime_error(where + ": interval invers '" + token + "'");
        vector<char> result;
        for (int c = token[0]; c <= token[2]; c++)
            result.push_back(static_cast<char>(c));
        return result;
    }
    throw runtime_error(where + ": simbol invalid '" + token + "' (un caracter sau interval a-b)");
}

int FiniteAutomaton::stateIndex(const string& name, const string& where) const {
    auto it = indexByName.find(name);
    if (it == indexByName.end())
        throw runtime_error(where + ": starea '" + name + "' nu este declarata");
    return it->second;
}

vector<int> FiniteAutomaton::symbolIndices(const string& token, const string& where) const {
    vector<int> result;
    for (char c : expandSymbols(token, where)) {
        auto it = indexBySymbol.find(c);
        if (it == indexBySymbol.end())
            throw runtime_error(where + ": simbolul '" + string(1, c) + "' nu este in alfabet");
        result.push_back(it->second);
    }
    return result;
}

FiniteAutomaton::FiniteAutomaton(istream& in, const string& source, const string& stopWord) {
    bool haveStates = false, haveAlphabet = false;
    string line;
    int lineNo = 0;
    while (getline(in, line)) {
        lineNo++;
        istringstream ss(line);
        vector<string> parts;
        string word;
        while (ss >> word)
            parts.push_back(word);
        if (!stopWord.empty() && parts.size() == 1 && parts[0] == stopWord)
            break;
        if (parts.empty() || parts[0].rfind("//", 0) == 0)
            continue;

        string where = source + ":" + to_string(lineNo);

        if (parts[0] == "stari") {
            if (haveStates)
                throw runtime_error(where + ": 'stari' declarat de doua ori");
            if (parts.size() < 2)
                throw runtime_error(where + ": 'stari' fara nicio stare");
            for (size_t i = 1; i < parts.size(); i++) {
                if (isKeyword(parts[i]))
                    throw runtime_error(where + ": '" + parts[i] + "' este cuvant rezervat");
                if (indexByName.count(parts[i]))
                    throw runtime_error(where + ": starea '" + parts[i] + "' declarata de doua ori");
                indexByName[parts[i]] = static_cast<int>(stateNames.size());
                stateNames.push_back(parts[i]);
            }
            haveStates = true;
        } else if (parts[0] == "alfabet") {
            if (haveAlphabet)
                throw runtime_error(where + ": 'alfabet' declarat de doua ori");
            if (parts.size() < 2)
                throw runtime_error(where + ": 'alfabet' fara niciun simbol");
            for (size_t i = 1; i < parts.size(); i++)
                for (char c : expandSymbols(parts[i], where))
                    if (!indexBySymbol.count(c)) {
                        indexBySymbol[c] = static_cast<int>(symbols.size());
                        symbols.push_back(c);
                    }
            haveAlphabet = true;
        } else {
            if (!haveStates || !haveAlphabet)
                throw runtime_error(where + ": 'stari' si 'alfabet' trebuie declarate inainte");
            if (transitions.empty()) {
                final_.assign(stateNames.size(), false);
                transitions.assign(stateNames.size(), vector<vector<int>>(symbols.size()));
            }

            if (parts[0] == "start") {
                if (parts.size() != 2)
                    throw runtime_error(where + ": 'start' are exact o stare");
                if (start_ != -1)
                    throw runtime_error(where + ": 'start' declarat de doua ori");
                start_ = stateIndex(parts[1], where);
            } else if (parts[0] == "final") {
                for (size_t i = 1; i < parts.size(); i++)
                    final_[stateIndex(parts[i], where)] = true;
            } else if (parts.size() == 3) {
                int from = stateIndex(parts[0], where);
                int to = stateIndex(parts[2], where);
                for (int sym : symbolIndices(parts[1], where)) {
                    vector<int>& targets = transitions[from][sym];
                    // the same transition written twice is not nondeterminism
                    if (find(targets.begin(), targets.end(), to) == targets.end())
                        targets.push_back(to);
                }
            } else {
                throw runtime_error(where + ": linie invalida '" + line + "'");
            }
        }
    }

    if (!haveStates || !haveAlphabet)
        throw runtime_error(source + ": lipseste 'stari' sau 'alfabet'");
    if (start_ == -1)
        throw runtime_error(source + ": lipseste 'start'");
}

bool FiniteAutomaton::isDeterministic() const {
    for (const auto& row : transitions)
        for (const auto& targets : row)
            if (targets.size() > 1)
                return false;
    return true;
}

bool FiniteAutomaton::accepts(const string& sequence) const {
    int state = start_;
    for (char c : sequence) {
        auto it = indexBySymbol.find(c);
        if (it == indexBySymbol.end() || transitions[state][it->second].empty())
            return false;
        state = transitions[state][it->second][0];
    }
    return final_[state];
}

int FiniteAutomaton::longestAcceptedPrefix(const string& sequence) const {
    int state = start_;
    int best = final_[state] ? 0 : -1;
    for (size_t i = 0; i < sequence.size(); i++) {
        auto it = indexBySymbol.find(sequence[i]);
        if (it == indexBySymbol.end() || transitions[state][it->second].empty())
            break;
        state = transitions[state][it->second][0];
        if (final_[state])
            best = static_cast<int>(i) + 1;
    }
    return best;
}
