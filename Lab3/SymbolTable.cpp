#include "SymbolTable.h"
#include <fstream>
using namespace std;

int SymbolTable::lowerBound(const string& key) const {
    int lo = 0, hi = static_cast<int>(entries.size());
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (entries[mid].text < key)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

int SymbolTable::add(const string& text, SymbolKind kind) {
    int pos = lowerBound(text);
    if (pos < static_cast<int>(entries.size()) && entries[pos].text == text)
        return entries[pos].id;   // already present: reuse its id

    Symbol sym{text, kind, nextId++};
    entries.insert(entries.begin() + pos, sym);
    return sym.id;
}

vector<int> SymbolTable::positionsById() const {
    vector<int> newPos(nextId);
    for (int i = 0; i < static_cast<int>(entries.size()); i++)
        newPos[entries[i].id] = i;
    return newPos;
}

bool SymbolTable::save(const string& path) const {
    ofstream out(path);
    if (!out) return false;
    for (const Symbol& s : entries)
        out << s.text << " " << (s.kind == SymbolKind::ID ? "ID" : "CONST") << "\n";
    return true;
}
