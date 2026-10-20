#pragma once
#include "CodesTable.h"
#include "FiniteAutomata.h"
#include "Fip.h"
#include "SymbolTable.h"
#include <string>
#include <vector>

struct LexError {
    int line;
    int col;
    std::string message;
};

class AutomataScanner {
public:
    AutomataScanner(const FiniteAutomata& fa, const CodesTable& codes)
        : fa(fa), codes(codes) {}
    void scan(const std::string& src, SymbolTable& ts, Fip& fip);
    const std::vector<LexError>& errors() const { return errors_; }

private:
    void emit(const std::string& tok, const std::string& label,
              int line, int col, SymbolTable& ts, Fip& fip);

    const FiniteAutomata& fa;
    const CodesTable& codes;
    std::vector<LexError> errors_;
};
