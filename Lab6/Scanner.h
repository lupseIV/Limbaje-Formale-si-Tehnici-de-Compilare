#pragma once
#include "CodesTable.h"
#include "FiniteAutomaton.h"
#include "Fip.h"
#include "SymbolTable.h"
#include <string>
#include <vector>

struct LexError {
    int line;
    int col;
    std::string message;
};

// The automata that recognise the lexical atoms; each one is a separate DFA.
struct LexAutomata {
    const FiniteAutomaton& identifiers;
    const FiniteAutomaton& integers;
    const FiniteAutomaton& reals;
    const FiniteAutomaton& operatorsSeparators;
};

// Splits the source into atoms by running every automaton from the current
// position and keeping the longest accepted prefix (whitespace is optional).
class Scanner {
public:
    Scanner(const LexAutomata& automata, const CodesTable& codes)
        : fa(automata), codes(codes) {}
    void scan(const std::string& src, SymbolTable& ts, Fip& fip);
    const std::vector<LexError>& errors() const { return errors_; }

private:
    enum class Kind { REAL, INT, ID, OP };

    void emit(const std::string& tok, Kind kind, int line, int col,
              SymbolTable& ts, Fip& fip);

    LexAutomata fa;
    const CodesTable& codes;
    std::vector<LexError> errors_;
};
