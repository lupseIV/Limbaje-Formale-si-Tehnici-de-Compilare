#include "Fip.h"
#include <fstream>
using namespace std;

void Fip::finalize(const vector<int>& newPos) {
    if (finalized) return;
    for (FipEntry& e : entries)
        if (e.pos != -1)
            e.pos = newPos[e.pos];
    finalized = true;
}

bool Fip::save(const string& path) const {
    ofstream out(path);
    if (!out) return false;
    for (const FipEntry& e : entries)
        out << e.code << " " << e.pos << "\n";
    return true;
}
