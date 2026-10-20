#pragma once
#include <map>
#include <string>
#include <utility>
#include <vector>

// Deterministic finite automaton loaded from a config file:
//   start q0
//   final qId ID
//   q0 letter qId
// A one-character symbol is a literal; a word is a character class
// (letter, digit, nonzero). Literal transitions are checked before classes.
// Lines starting with "//" are comments. Throws std::runtime_error on errors.
class FiniteAutomata {
public:
    explicit FiniteAutomata(const std::string& path);
    int start() const { return start_; }
    // Next state, or -1 if there is no transition.
    int next(int state, char c) const;
    bool isFinal(int state) const { return !labels[state].empty(); }
    const std::string& label(int state) const { return labels[state]; }

private:
    int stateIndex(const std::string& name);

    std::map<std::string, int> indexByName;
    std::vector<std::string> labels;   // empty = not final
    std::vector<std::map<char, int>> literal;
    std::vector<std::vector<std::pair<std::string, int>>> byClass;
    int start_ = -1;
};
