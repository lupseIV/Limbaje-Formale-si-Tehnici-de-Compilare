/* Analizor lexical pentru MLP (subset C++, vezi Lab1AnalizatorLexical/SpecificareMPL(BNF))
 * generat cu flex. Construieste FIP si TS (tabel unic, ordonat lexicografic) si
 * raporteaza erorile lexicale cu linia si coloana la care apar.
 */

%option noyywrap nounput noinput yylineno

%{
#include "CodesTable.h"
#include "Fip.h"
#include "SymbolTable.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;
namespace fs = std::filesystem;

const string CONFIG_DIR = "code_configs";

struct LexError {
    int line;
    int col;
    string message;
};

static unique_ptr<CodesTable> codes;
static SymbolTable ts;
static Fip fip;
static vector<LexError> errors;

// Coloana la care incepe atomul curent (yylineno da linia).
static int tokCol = 1;
static int nextCol = 1;
#define YY_USER_ACTION { tokCol = nextCol; nextCol += yyleng; }

static void show(const char* kind) {
    cout << yylineno << ":" << tokCol << "\t" << kind << "\t" << yytext << "\n";
}

static void emitAtom(AtomType type, const char* kind) {
    int code = codes->find(yytext, type);
    if (code == -1) {
        errors.push_back({yylineno, tokCol, string("unknown atom '") + yytext + "'"});
        return;
    }
    show(kind);
    fip.add(code);
}

static void emitSymbol(SymbolKind kind) {
    bool isId = kind == SymbolKind::ID;
    show(isId ? "ID" : "CONST");
    fip.add(isId ? codes->idCode() : codes->constCode(), ts.add(yytext, kind));
}

static void error(const string& message) {
    errors.push_back({yylineno, tokCol, message});
}
%}

LETTER      [A-Za-z_]
DIGIT       [0-9]
WORD        [A-Za-z0-9_]

/* <const_int>  ::= 0 | <cifra_nenula> {<cifra>}
   <const_real> ::= <const_int> . <sir_cifre>          ex: 0  42  3.14  0.5 */
INT         0|[1-9]{DIGIT}*
REAL        {INT}\.{DIGIT}+

KEYWORD     "#include"|iostream|using|namespace|std|struct|int|double|main|return|cin|cout|endl|if|else|while
OPERATOR    "<<"|">>"|"<="|">="|"=="|"!="|"="|"+"|"-"|"*"|"/"|"<"|">"
SEPARATOR   [;,.{}()]

%%

\n                  { nextCol = 1; }
[ \t\r]+            { /* spatiile albe separa atomii, nu apar in FIP */ }

{REAL}              { emitSymbol(SymbolKind::CONST); }
{INT}               { emitSymbol(SymbolKind::CONST); }

 /* o constanta lipita de litere/cifre (1abc, 09, 0x, 1e, 3.14f) este eroare;
    la lungime egala castiga regulile de mai sus */
({INT}|{REAL}){WORD}+ { error(string("invalid token '") + yytext + "'"); }

{KEYWORD}           { emitAtom(AtomType::KEYWORD, "KEYWORD"); }
{LETTER}{WORD}*     { emitSymbol(SymbolKind::ID); }
{OPERATOR}          { emitAtom(AtomType::OPERATOR, "OPERATOR"); }
{SEPARATOR}         { emitAtom(AtomType::SEPARATOR, "SEPARATOR"); }

.                   { error(string("invalid character '") + yytext + "'"); }

%%

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " source_file" << endl;
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        cerr << "Error: could not open " << argv[1] << endl;
        return 1;
    }

    try {
        codes = make_unique<CodesTable>(CONFIG_DIR);
    } catch (const exception& e) {
        cerr << "Config error: " << e.what() << endl;
        return 1;
    }

    cout << "linie:col\ttip\tatom\n";
    yylex();
    fclose(yyin);
    fip.finalize(ts.positionsById());

    // cmmdc.cpp -> cmmdc.lex/ langa sursa, recreat la fiecare rulare.
    fs::path src(argv[1]);
    fs::path outDir = src.parent_path() / (src.stem().string() + ".lex");
    fs::remove_all(outDir);
    fs::create_directories(outDir);

    if (!ts.save((outDir / "TS.txt").string()) || !fip.save((outDir / "FIP.txt").string())) {
        cerr << "Error: could not write output in " << outDir << endl;
        return 1;
    }

    if (!errors.empty()) {
        ofstream err(outDir / "errors.txt");
        for (const LexError& e : errors)
            err << "Line " << e.line << ", Col " << e.col << ": " << e.message << "\n";
        cout << errors.size() << " lexical error(s), see " << (outDir / "errors.txt").string() << endl;
        return 1;
    }

    cout << "OK, output written to " << outDir.string() << endl;
    return 0;
}
