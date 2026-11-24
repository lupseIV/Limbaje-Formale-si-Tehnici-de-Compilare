/* Analizor lexical pentru MLP (vezi Lab1AnalizatorLexical/SpecificareMPL(BNF)),
 * adaptat din Lab 7 pentru a lucra "la cerere" (pull) cu parserul bison:
 * yyparse() apeleaza yylex(), care intoarce cate un atom (tipul din enum-ul
 * generat de bison in parser.hpp). In paralel se construiesc FIP si TS.
 * Erorile lexicale sunt inregistrate, textul gresit e sarit si analiza continua.
 */

%option noyywrap nounput noinput yylineno

%{
#include "Diagnostics.h"
#include "parser.hpp"
#include <string>
using namespace std;

int tokCol = 1;
static int nextCol = 1;
#define YY_USER_ACTION { tokCol = nextCol; nextCol += yyleng; }

static void lexError(const string& message) {
    errors.push_back({yylineno, tokCol, "lexical", message});
}

// Cuvant cheie / operator / separator: adauga codul in FIP si intoarce tipul atomului.
static int atom(AtomType type, int token) {
    int code = codes->find(yytext, type);
    if (code == -1)
        lexError(string("atom '") + yytext + "' missing from code_configs");
    else
        fip.add(code);
    return token;
}

// Identificator / constanta: adauga in TS, apoi (cod, pozitie TS) in FIP.
static int symbol(SymbolKind kind, int token) {
    bool isId = kind == SymbolKind::ID;
    fip.add(isId ? codes->idCode() : codes->constCode(), ts.add(yytext, kind));
    return token;
}
%}

LETTER      [A-Za-z_]
DIGIT       [0-9]
WORD        [A-Za-z0-9_]

/* <const_int>  ::= 0 | <cifra_nenula> {<cifra>}
   <const_real> ::= <const_int> . <sir_cifre>          ex: 0  42  3.14  0.5 */
INT         0|[1-9]{DIGIT}*
REAL        {INT}\.{DIGIT}+

%%

\n                  { nextCol = 1; }
[ \t\r]+            { /* spatiile albe separa atomii, nu ajung la parser */ }

{REAL}              { return symbol(SymbolKind::CONST, CONST); }
{INT}               { return symbol(SymbolKind::CONST, CONST); }

 /* constanta lipita de litere/cifre (1abc, 09, 1e5, 3.14f): eroare, se sare */
({INT}|{REAL}){WORD}+ { lexError(string("invalid token '") + yytext + "'"); }

"#include"          { return atom(AtomType::KEYWORD, INCLUDE); }
"iostream"          { return atom(AtomType::KEYWORD, IOSTREAM); }
"using"             { return atom(AtomType::KEYWORD, USING); }
"namespace"         { return atom(AtomType::KEYWORD, NAMESPACE); }
"std"               { return atom(AtomType::KEYWORD, STD); }
"struct"            { return atom(AtomType::KEYWORD, STRUCT); }
"int"               { return atom(AtomType::KEYWORD, INT); }
"double"            { return atom(AtomType::KEYWORD, DOUBLE); }
"main"              { return atom(AtomType::KEYWORD, MAIN); }
"return"            { return atom(AtomType::KEYWORD, RETURN); }
"cin"               { return atom(AtomType::KEYWORD, CIN); }
"cout"              { return atom(AtomType::KEYWORD, COUT); }
"endl"              { return atom(AtomType::KEYWORD, ENDL); }
"if"                { return atom(AtomType::KEYWORD, IF); }
"else"              { return atom(AtomType::KEYWORD, ELSE); }
"while"             { return atom(AtomType::KEYWORD, WHILE); }

{LETTER}{WORD}*     { return symbol(SymbolKind::ID, ID); }

"<<"                { return atom(AtomType::OPERATOR, SHL); }
">>"                { return atom(AtomType::OPERATOR, SHR); }
"<="                { return atom(AtomType::OPERATOR, LE); }
">="                { return atom(AtomType::OPERATOR, GE); }
"=="                { return atom(AtomType::OPERATOR, EQ); }
"!="                { return atom(AtomType::OPERATOR, NE); }
"="                 { return atom(AtomType::OPERATOR, ASSIGN); }
"+"                 { return atom(AtomType::OPERATOR, PLUS); }
"-"                 { return atom(AtomType::OPERATOR, MINUS); }
"*"                 { return atom(AtomType::OPERATOR, MUL); }
"/"                 { return atom(AtomType::OPERATOR, DIV); }
"<"                 { return atom(AtomType::OPERATOR, LT); }
">"                 { return atom(AtomType::OPERATOR, GT); }

";"                 { return atom(AtomType::SEPARATOR, SEMI); }
","                 { return atom(AtomType::SEPARATOR, COMMA); }
"."                 { return atom(AtomType::SEPARATOR, DOT); }
"{"                 { return atom(AtomType::SEPARATOR, LBRACE); }
"}"                 { return atom(AtomType::SEPARATOR, RBRACE); }
"("                 { return atom(AtomType::SEPARATOR, LPAREN); }
")"                 { return atom(AtomType::SEPARATOR, RPAREN); }

.                   { lexError(string("invalid character '") + yytext + "'"); }

%%
