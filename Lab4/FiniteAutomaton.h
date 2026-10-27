#pragma once
#include <istream>
#include <map>
#include <string>
#include <vector>

// Finite automaton (possibly nondeterministic) read from a text stream:
//   stari q0 q1 q2
//   alfabet 0-9 a-f x
//   start q0
//   final q1 q2
//   q0 1-9 q1
// "stari" and "alfabet" come before every other line. A symbol is one
// character; "a-f" is the range of consecutive ASCII characters a..f.
// Lines starting with "//" are comments. Throws std::runtime_error on errors.
class FiniteAutomaton {
public:
    // Reads until end of stream, or until a line equal to stopWord (if not empty).
    FiniteAutomaton(std::istream& in, const std::string& source,
                    const std::string& stopWord = "");

    const std::vector<std::string>& states() const { return stateNames; }
    const std::vector<char>& alphabet() const { return symbols; }
    int start() const { return start_; }
    bool isFinal(int state) const { return final_[state]; }
    // Next states for (state, symbol index); empty if there is no transition.
    const std::vector<int>& next(int state, int symbol) const { return transitions[state][symbol]; }

    // Deterministic iff every (state, symbol) has at most one next state.
    bool isDeterministic() const;

    // The two functions below require a deterministic automaton.
    bool accepts(const std::string& sequence) const;
    // Length of the longest accepted prefix, or -1 if no prefix is accepted.
    int longestAcceptedPrefix(const std::string& sequence) const;

private:
    int stateIndex(const std::string& name, const std::string& where) const;
    std::vector<int> symbolIndices(const std::string& token, const std::string& where) const;

    std::vector<std::string> stateNames;
    std::map<std::string, int> indexByName;
    std::vector<char> symbols;
    std::map<char, int> indexBySymbol;
    int start_ = -1;
    std::vector<bool> final_;
    // transitions[state][symbol] = next states
    std::vector<std::vector<std::vector<int>>> transitions;
};
