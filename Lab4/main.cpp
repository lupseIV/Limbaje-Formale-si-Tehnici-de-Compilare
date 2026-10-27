#include "FiniteAutomaton.h"
#include <fstream>
#include <iostream>
#include <memory>
using namespace std;

const string STOP_WORD = "STOP_AUTOMAT";

static unique_ptr<FiniteAutomaton> readAutomaton(int argc, char* argv[]) {
    string path;
    if (argc >= 2) {
        path = argv[1];
    } else {
        cout << "Citire automat:\n  1. din fisier\n  2. de la tastatura\nOptiune: ";
        string choice;
        getline(cin, choice);
        if (choice == "2") {
            cout << "Introduceti automatul (acelasi format ca fisierul), terminat cu "
                 << STOP_WORD << ":\n";
            return make_unique<FiniteAutomaton>(cin, "tastatura", STOP_WORD);
        }
        cout << "Fisier: ";
        getline(cin, path);
    }

    ifstream file(path);
    if (!file)
        throw runtime_error("nu pot deschide " + path);
    return make_unique<FiniteAutomaton>(file, path);
}

static void printStates(const FiniteAutomaton& fa) {
    cout << "Q = { ";
    for (const string& s : fa.states())
        cout << s << ' ';
    cout << "}\n";
}

static void printAlphabet(const FiniteAutomaton& fa) {
    cout << "Sigma = { ";
    for (char c : fa.alphabet())
        cout << c << ' ';
    cout << "}\n";
}

static void printTransitions(const FiniteAutomaton& fa) {
    const auto& states = fa.states();
    for (size_t s = 0; s < states.size(); s++)
        for (size_t a = 0; a < fa.alphabet().size(); a++) {
            const vector<int>& targets = fa.next(static_cast<int>(s), static_cast<int>(a));
            if (targets.empty())
                continue;
            cout << "d(" << states[s] << ", " << fa.alphabet()[a] << ") = { ";
            for (int t : targets)
                cout << states[t] << ' ';
            cout << "}\n";
        }
}

static void printFinals(const FiniteAutomaton& fa) {
    cout << "F = { ";
    for (size_t s = 0; s < fa.states().size(); s++)
        if (fa.isFinal(static_cast<int>(s)))
            cout << fa.states()[s] << ' ';
    cout << "}\n";
}

static bool readSequence(const FiniteAutomaton& fa, string& sequence) {
    if (!fa.isDeterministic()) {
        cout << "Automatul nu este determinist.\n";
        return false;
    }
    cout << "Secventa (linie goala = secventa vida): ";
    return static_cast<bool>(getline(cin, sequence));
}

int main(int argc, char* argv[]) {
    unique_ptr<FiniteAutomaton> fa;
    try {
        fa = readAutomaton(argc, argv);
    } catch (const exception& e) {
        cerr << "Eroare: " << e.what() << endl;
        return 1;
    }

    cout << "Automat citit (" << (fa->isDeterministic() ? "determinist" : "nedeterminist")
         << "), stare initiala " << fa->states()[fa->start()] << ".\n";

    string option;
    while (true) {
        cout << "\n1. Multimea starilor\n2. Alfabetul\n3. Tranzitiile\n"
                "4. Multimea starilor finale\n5. Verifica secventa (AFD)\n"
                "6. Cel mai lung prefix acceptat (AFD)\n0. Iesire\nOptiune: ";
        if (!getline(cin, option) || option == "0")
            break;

        string sequence;
        if (option == "1") {
            printStates(*fa);
        } else if (option == "2") {
            printAlphabet(*fa);
        } else if (option == "3") {
            printTransitions(*fa);
        } else if (option == "4") {
            printFinals(*fa);
        } else if (option == "5") {
            if (readSequence(*fa, sequence))
                cout << (fa->accepts(sequence) ? "Secventa este acceptata.\n"
                                               : "Secventa NU este acceptata.\n");
        } else if (option == "6") {
            if (readSequence(*fa, sequence)) {
                int len = fa->longestAcceptedPrefix(sequence);
                if (len < 0)
                    cout << "Niciun prefix nu este acceptat.\n";
                else if (len == 0)
                    cout << "Cel mai lung prefix acceptat: epsilon (secventa vida)\n";
                else
                    cout << "Cel mai lung prefix acceptat: " << sequence.substr(0, len) << '\n';
            }
        } else {
            cout << "Optiune invalida.\n";
        }
    }
    return 0;
}
