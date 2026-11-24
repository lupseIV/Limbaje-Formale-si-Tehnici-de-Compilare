/* Analizor sintactic pentru MLP (Lab1AnalizatorLexical/SpecificareMPL(BNF)).
 * Apeleaza lexerul (lexer.l) la cerere; se opreste la prima eroare sintactica.
 * Gramatica nu are conflicte: `bison -Wcounterexamples parser.y` nu raporteaza nimic.
 */

%{
#include "Diagnostics.h"
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;
namespace fs = std::filesystem;

int yylex();
extern FILE* yyin;
void yyerror(const char* message);

unique_ptr<CodesTable> codes;
SymbolTable ts;
Fip fip;
vector<CompileError> errors;
%}

/* Mesaje de forma: syntax error, unexpected ";", expecting ID */
%define parse.error verbose

/* Tipurile de atomi: bison genereaza enum-ul in parser.hpp, inclus si de lexer.
   Al doilea nume (intre ghilimele) apare in mesajele de eroare. */
%token INCLUDE "#include" IOSTREAM "iostream" USING "using" NAMESPACE "namespace"
%token STD "std" STRUCT "struct" INT "int" DOUBLE "double" MAIN "main"
%token RETURN "return" CIN "cin" COUT "cout" ENDL "endl"
%token IF "if" ELSE "else" WHILE "while"
%token SHL "<<" SHR ">>" LE "<=" GE ">=" EQ "==" NE "!="
%token ASSIGN "=" PLUS "+" MINUS "-" MUL "*" DIV "/" LT "<" GT ">"
%token SEMI ";" COMMA "," DOT "." LBRACE "{" RBRACE "}" LPAREN "(" RPAREN ")"
%token ID CONST

/* Dangling else: dupa `if (c) instr`, cu `else` urmator, bison poate
   reduce (if fara else) sau deplasa (else apartine acestui if).
   ELSE are precedenta mai mare decat regula fara else => se alege deplasarea,
   deci else-ul se leaga de cel mai apropiat if, ca in C/C++. */
%precedence LOWER_THAN_ELSE
%precedence ELSE

%start program

%%

program
    : antet lista_struct main_fn
    | antet main_fn
    ;

antet
    : INCLUDE LT IOSTREAM GT USING NAMESPACE STD SEMI
    ;

main_fn
    : INT MAIN LPAREN RPAREN LBRACE lista_instr RETURN CONST SEMI RBRACE
    ;

lista_struct
    : def_struct
    | lista_struct def_struct
    ;

def_struct
    : STRUCT ID LBRACE lista_decl RBRACE SEMI
    ;

lista_decl
    : decl
    | lista_decl decl
    ;

decl
    : tip lista_id SEMI
    ;

lista_id
    : ID
    | lista_id COMMA ID
    ;

tip
    : INT
    | DOUBLE
    | ID
    ;

lista_instr
    : instr
    | lista_instr instr
    ;

instr
    : decl
    | atribuire
    | intrare
    | iesire
    | instr_if
    | instr_while
    | instr_comp
    ;

instr_comp
    : LBRACE lista_instr RBRACE
    ;

atribuire
    : variabila ASSIGN expr SEMI
    ;

variabila
    : ID
    | ID DOT ID
    ;

intrare
    : CIN lista_citire SEMI
    ;

lista_citire
    : SHR variabila
    | lista_citire SHR variabila
    ;

iesire
    : COUT lista_afisare SEMI
    ;

lista_afisare
    : SHL element
    | lista_afisare SHL element
    ;

element
    : expr
    | ENDL
    ;

instr_if
    : IF LPAREN conditie RPAREN instr %prec LOWER_THAN_ELSE
    | IF LPAREN conditie RPAREN instr ELSE instr
    ;

instr_while
    : WHILE LPAREN conditie RPAREN instr
    ;

conditie
    : expr op_rel expr
    ;

op_rel
    : LT | LE | GT | GE | EQ | NE
    ;

expr
    : termen
    | expr PLUS termen
    | expr MINUS termen
    ;

termen
    : factor
    | termen MUL factor
    | termen DIV factor
    ;

factor
    : variabila
    | CONST
    | LPAREN expr RPAREN
    ;

%%

// Apelata de yyparse() la prima eroare sintactica; yytext este atomul la care s-a blocat.
void yyerror(const char* message) {
    string near = (yytext && *yytext) ? string(" near '") + yytext + "'" : " at end of file";
    errors.push_back({yylineno, tokCol, "syntax", string(message) + near});
}

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
        codes = make_unique<CodesTable>("code_configs");
    } catch (const exception& e) {
        cerr << "Config error: " << e.what() << endl;
        return 1;
    }

    yyparse();
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

    if (errors.empty()) {
        cout << "Program corect sintactic. FIP/TS in " << outDir.string() << endl;
        return 0;
    }

    ofstream err(outDir / "errors.txt");
    for (const CompileError& e : errors) {
        string line = "Line " + to_string(e.line) + ", Col " + to_string(e.col) + ": "
                    + e.kind + ": " + e.message;
        err << line << "\n";
        cerr << line << "\n";
    }
    cout << errors.size() << " error(s), see " << (outDir / "errors.txt").string() << endl;
    return 1;
}
