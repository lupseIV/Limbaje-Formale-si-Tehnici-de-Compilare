#pragma once
// Stare comuna intre lexer.l si parser.y: tabelele (FIP, TS, coduri)
// si lista de erori (lexicale + sintactice), in ordinea aparitiei.
#include "CodesTable.h"
#include "Fip.h"
#include "SymbolTable.h"
#include <memory>
#include <string>
#include <vector>

struct CompileError {
    int line;
    int col;
    std::string kind;     // "lexical" sau "syntax"
    std::string message;
};

extern std::unique_ptr<CodesTable> codes;
extern SymbolTable ts;
extern Fip fip;
extern std::vector<CompileError> errors;

// Pozitia atomului curent, actualizata de lexer.
extern int yylineno;
extern int tokCol;
extern char* yytext;
