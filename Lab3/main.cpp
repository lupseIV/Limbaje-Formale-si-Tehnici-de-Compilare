#include "AutomataScanner.h"
#include "CodesTable.h"
#include "FiniteAutomata.h"
#include "Fip.h"
#include "SymbolTable.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
namespace fs = std::filesystem;

const string CONFIG_DIR = "code_configs";

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " source_file" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file) {
        cerr << "Error: could not open " << argv[1] << endl;
        return 1;
    }
    stringstream buffer;
    buffer << file.rdbuf();

    SymbolTable ts;
    Fip fip;
    vector<LexError> errors;
    try {
        CodesTable codes(CONFIG_DIR);
        FiniteAutomata fa(CONFIG_DIR + "/automaton.txt");
        AutomataScanner scanner(fa, codes);
        scanner.scan(buffer.str(), ts, fip);
        errors = scanner.errors();
    } catch (const exception& e) {
        cerr << "Config error: " << e.what() << endl;
        return 1;
    }
    fip.finalize(ts.positionsById());

    // cmmdc.cpp -> cmmdc.lex/ next to the source, replaced on every run.
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
