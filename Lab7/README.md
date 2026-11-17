# Lab 7 – Analizor lexical cu FLEX

`lexer.l` contine expresiile regulate pentru atomii MLP, exact cum sunt definiti in `Lab1AnalizatorLexical/SpecificareMPL(BNF)`:

| Atom | Regex flex |
|---|---|
| ID | `[A-Za-z_][A-Za-z0-9_]*` |
| const_int | `0\|[1-9][0-9]*` |
| const_real | `{INT}\.[0-9]+` |
| cuvinte cheie | `#include iostream using namespace std struct int double main return cin cout endl if else while` |
| operatori | `= + - * / < <= > >= == != << >>` |
| separatori | `; , . { } ( )` si spatii albe |
Reutilizeaza din Lab 6: `CodesTable`, `SymbolTable` (TS unica, ordonata lexicografic), `Fip` si `code_configs/`.

## Build (din Lab7/)

    sudo apt install flex          # o singura data
    flex -o lexer.cpp lexer.l
    g++ -std=c++17 -Wall -Wextra -o lexer lexer.cpp CodesTable.cpp SymbolTable.cpp Fip.cpp

## Rulare

    ./lexer teste/cmmdc.cpp

- pe ecran: atomii lexicali (`linie:col  tip  atom`)
- `teste/cmmdc.lex/TS.txt`, `FIP.txt` (pozitiile numerotate de la 0, ca in Lab 6)
- `teste/cmmdc.lex/errors.txt` daca exista erori (`Line L, Col C: mesaj`), cod de iesire 1

## Ordinea regulilor

flex alege potrivirea cea mai lunga; la lungime egala castiga regula scrisa prima. De aceea:
- `{REAL}` / `{INT}` sunt inaintea regulii de eroare `({INT}|{REAL}){WORD}+`, deci `42`, `3.14` sunt corecte, iar `1abc`, `09`, `0x1F`, `1e5`, `2.5f` sunt erori;
- `{KEYWORD}` este inaintea lui `{LETTER}{WORD}*`, deci `while` e cuvant cheie, dar `whilex` e identificator;
- `.` (orice alt caracter) este ultima regula: caracter invalid.
