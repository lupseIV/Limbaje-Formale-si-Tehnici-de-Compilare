# Lab4 – Automate finite

## Compilare si rulare

```
g++ -std=c++17 -Wall -Wextra -o af main.cpp FiniteAutomaton.cpp
./af automate/exemplu_afd.txt    # citire din fisier
./af                             # alegere: fisier sau tastatura
```

De la tastatura se scrie automatul in acelasi format ca fisierul, linie cu linie,
iar citirea se incheie cu o linie care contine doar `STOP_AUTOMAT`.

Meniul afiseaza multimea starilor, alfabetul, tranzitiile si starile finale.
Optiunile 5 (verificare secventa) si 6 (cel mai lung prefix acceptat) functioneaza
doar pentru un automat determinist.

## Formatul fisierului (EBNF)

```
automat        = { linie_goala } , antet , { linie_corp | linie_goala } ;
antet          = decl_stari , { linie_goala } , decl_alfabet
               | decl_alfabet , { linie_goala } , decl_stari ;

decl_stari     = "stari" , sp , nume_stare , { sp , nume_stare } , nl ;
decl_alfabet   = "alfabet" , sp , element , { sp , element } , nl ;

linie_corp     = ( decl_start | decl_final | tranzitie ) , nl ;
decl_start     = "start" , sp , nume_stare ;
decl_final     = "final" , sp , nume_stare , { sp , nume_stare } ;
tranzitie      = nume_stare , sp , element , sp , nume_stare ;

element        = simbol | simbol , "-" , simbol ;      (* interval ASCII a-b *)
simbol         = caracter ;
nume_stare     = caracter , { caracter } ;
caracter       = ? orice caracter ASCII tiparibil, fara spatiu ? ;

linie_goala    = [ sp ] , [ comentariu ] , nl ;
comentariu     = "//" , { ? orice caracter in afara de nl ? } ;
sp             = ( " " | "\t" ) , { " " | "\t" } ;
nl             = ? sfarsit de linie ? ;
```

Restrictii care nu se pot exprima in EBNF (verificate de program, cu numarul liniei):

- `decl_start` apare exact o data; fiecare declaratie (`stari`, `alfabet`) apare o singura data;
- `nume_stare` nu poate fi `stari`, `alfabet`, `start` sau `final`, iar starile sunt distincte;
- orice stare folosita in `start`, `final` sau o tranzitie este declarata in `stari`;
- orice simbol folosit intr-o tranzitie este in alfabet;
- la un interval `a-b`, trebuie `a <= b` (cod ASCII);
- spatiile de la inceputul liniei sunt ignorate.

O tranzitie cu interval, de exemplu `q1 0-9 q1`, inseamna cate o tranzitie pentru fiecare
simbol din interval. Automatul este determinist daca pentru fiecare pereche (stare, simbol)
exista cel mult o stare urmatoare.

## Exemple

- `automate/literali_intregi.txt` – AFD pentru constantele intregi din C/C++ (zecimal, octal,
  hexazecimal, binar, separatori `'` si sufixe `u`, `l`, `ll`) – data de test ceruta.
- `automate/exemplu_afd.txt` – AFD pentru numere binare fara zerouri nesemnificative.
- `automate/exemplu_afn.txt` – AFN pentru secvente peste {a, b} care se termina cu `ab`.
