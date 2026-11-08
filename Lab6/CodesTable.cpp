#include "CodesTable.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
using namespace std;

CodesTable::CodesTable(const string& dir) {
    loadFile(dir + "/special.txt", AtomType::SPECIAL);
    loadFile(dir + "/keywords.txt", AtomType::KEYWORD);
    loadFile(dir + "/operators.txt", AtomType::OPERATOR);
    loadFile(dir + "/separators.txt", AtomType::SEPARATOR);

    idCode_ = find("ID", AtomType::SPECIAL);
    constCode_ = find("CONST", AtomType::SPECIAL);
    if (idCode_ == -1 || constCode_ == -1)
        throw runtime_error(dir + "/special.txt must define both ID and CONST");
}

void CodesTable::loadFile(const string& path, AtomType type) {
    ifstream in(path);
    if (!in)
        throw runtime_error("cannot open " + path);

    string line;
    int lineNo = 0;
    while (getline(in, line)) {
        lineNo++;
        istringstream ss(line);
        string atom;
        if (!(ss >> atom))
            continue;   // blank line

        string where = path + ":" + to_string(lineNo);
        int code;
        string extra;
        if (!(ss >> code) || (ss >> extra))
            throw runtime_error(where + ": expected 'atom code'");
        if (byAtom.count(atom))
            throw runtime_error(where + ": atom '" + atom + "' is defined twice");
        if (usedCodes.count(code))
            throw runtime_error(where + ": code " + to_string(code) + " is already used");

        byAtom[atom] = {code, type};
        usedCodes.insert(code);
    }
}

int CodesTable::find(const string& atom, AtomType type) const {
    auto it = byAtom.find(atom);
    if (it == byAtom.end() || it->second.type != type)
        return -1;
    return it->second.code;
}
