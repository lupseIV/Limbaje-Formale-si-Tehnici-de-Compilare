# Lab 8 (Tema 4) – Analizor sintactic cu BISON

`parser.y` contine regulile de productie pentru MLP, dupa `Lab1AnalizatorLexical/SpecificareMPL(BNF)`.
`lexer.l` este lexerul din Lab 7, modificat sa intoarca cate un atom la fiecare apel `yylex()`.

## Cum lucreaza

- **Pull:** `yyparse()` cere atomii unul cate unul de la `yylex()`. Tipurile atomilor (`IF`, `WHILE`, `LE`, `ID`, `CONST`, ...) sunt declarate cu `%token` in `parser.y`; bison genereaza enum-ul in `parser.hpp`, inclus si de lexer.
- **FIP / TS** se construiesc in lexer, pe masura ce atomii sunt ceruti (ca in Lab 7).
- **Erori lexicale** (`1abc`, `09`, `@`): sunt inregistrate, textul gresit e sarit, analiza continua.
- **Erori sintactice:** analiza se opreste la prima eroare; se raporteaza linia si coloana atomului la care s-a blocat parserul.
- `errors.txt` contine ambele tipuri de erori, in ordinea aparitiei.

## Conflicte

Gramatica nu are conflicte. Singurul conflict posibil, *dangling else*, e rezolvat cu precedente:

    %precedence LOWER_THAN_ELSE
    %precedence ELSE
    ...
    instr_if : IF ( conditie ) instr %prec LOWER_THAN_ELSE
             | IF ( conditie ) instr ELSE instr

`ELSE` are precedenta mai mare, deci bison alege deplasarea: `else` apartine celui mai apropiat `if` (ca in C/C++).
Fara aceste linii, `bison -Wcounterexamples` raporteaza 1 conflict shift/reduce pe `else`.

Diferente fata de BNF: `;` in plus de la finalul lui `<lista_citire>` a fost eliminat; listele folosesc recursivitate la stanga (`lista : x | lista x`), care descrie acelasi limbaj dar e recomandata in bison (stiva nu creste cu lungimea listei).

## Build (din Lab8/)

    sudo apt install bison flex      # o singura data
    bison -d -Wall -Wcounterexamples -o parser.cpp parser.y
    flex -o lexer.cpp lexer.l
    g++ -std=c++17 -Wall -Wextra -o parser parser.cpp lexer.cpp CodesTable.cpp SymbolTable.cpp Fip.cpp

## Rulare

    ./parser teste/cmmdc.cpp

- corect: `Program corect sintactic`, cod de iesire 0
- cu erori: erorile pe ecran (`Line L, Col C: lexical|syntax: mesaj`), cod de iesire 1
- `teste/<nume>.lex/`: `FIP.txt`, `TS.txt` si `errors.txt` (doar daca exista erori)

## Teste

| Fisier | Rezultat |
|---|---|
| `cmmdc.cpp`, `cerc.cpp`, `suma.cpp`, `fara_spatii.cpp` | corecte |
| `sintaxa_if_imbricat.cpp` | corect (`if` in `if`, `else` legat de cel interior) |
| `sintaxa_lipsa_pv.cpp` | lipseste `;` – eroare raportata pe linia urmatoare, la `cout` |
| `sintaxa_acolada.cpp` | `{` neinchisa – eroare la `return` |
| `sintaxa_else.cpp` | `else` fara `if` |
| `sintaxa_lexical.cpp`, `eroare1..3.cpp` | eroare lexicala, apoi eroarea sintactica pe care o produce golul |
