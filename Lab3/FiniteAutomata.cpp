#include "FiniteAutomata.h"
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>
using namespace std;

static bool isKnownClass(const string& cls) {
    return cls == "letter" || cls == "digit" || cls == "nonzero";
}

static bool inClass(const string& cls, char ch) {
    unsigned char c = static_cast<unsigned char>(ch);
    if (cls == "letter")  return isalpha(c) || c == '_';
    if (cls == "digit")   return isdigit(c);
    if (cls == "nonzero") return c >= '1' && c <= '9';
    return false;
}

int FiniteAutomata::stateIndex(const string& name) {
    auto it = indexByName.find(name);
    if (it != indexByName.end())
        return it->second;

    int idx = static_cast<int>(labels.size());
    indexByName[name] = idx;
    labels.emplace_back();
    literal.emplace_back();
    byClass.emplace_back();
    return idx;
}

FiniteAutomata::FiniteAutomata(const string& path) {
    ifstream in(path);
    if (!in)
        throw runtime_error("cannot open " + path);

    string line;
    int lineNo = 0;
    while (getline(in, line)) {
        lineNo++;
        istringstream ss(line);
        vector<string> parts;
        string word;
        while (ss >> word)
            parts.push_back(word);
        if (parts.empty() || parts[0].rfind("//", 0) == 0)
            continue;

        string where = path + ":" + to_string(lineNo);

        if (parts[0] == "start" && parts.size() == 2) {
            start_ = stateIndex(parts[1]);
        } else if (parts[0] == "final" && parts.size() == 3) {
            labels[stateIndex(parts[1])] = parts[2];
        } else if (parts.size() == 3) {
            int from = stateIndex(parts[0]);
            int to = stateIndex(parts[2]);
            const string& symbol = parts[1];

            if (symbol.size() == 1) {
                if (literal[from].count(symbol[0]))
                    throw runtime_error(where + ": duplicate transition on '" + symbol + "'");
                literal[from][symbol[0]] = to;
            } else {
                if (!isKnownClass(symbol))
                    throw runtime_error(where + ": unknown character class '" + symbol + "'");
                for (const auto& t : byClass[from])
                    if (t.first == symbol)
                        throw runtime_error(where + ": duplicate transition on class '" + symbol + "'");
                byClass[from].push_back({symbol, to});
            }
        } else {
            throw runtime_error(where + ": cannot parse '" + line + "'");
        }
    }

    if (start_ == -1)
        throw runtime_error(path + ": missing 'start' line");
}

int FiniteAutomata::next(int state, char c) const {
    auto it = literal[state].find(c);
    if (it != literal[state].end())
        return it->second;
    for (const auto& t : byClass[state])
        if (inClass(t.first, c))
            return t.second;
    return -1;
}
