#pragma once
#include <string>
#include <vector>

struct FipEntry {
    int code;
    int pos;
};

class Fip {
public:
    void add(int code, int symbolId = -1) { entries.push_back({code, symbolId}); }
    void finalize(const std::vector<int>& newPos);
    const std::vector<FipEntry>& all() const { return entries; }
    bool save(const std::string& path) const;

private:
    std::vector<FipEntry> entries;
    bool finalized = false;
};
