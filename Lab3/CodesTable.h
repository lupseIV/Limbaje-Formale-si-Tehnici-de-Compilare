#pragma once
#include <map>
#include <set>
#include <string>

enum class AtomType { KEYWORD, OPERATOR, SEPARATOR, SPECIAL };

class CodesTable {
public:
    explicit CodesTable(const std::string& dir);
    int find(const std::string& atom, AtomType type) const;
    int idCode() const { return idCode_; }
    int constCode() const { return constCode_; }

private:
    struct Entry {
        int code;
        AtomType type;
    };

    void loadFile(const std::string& path, AtomType type);

    std::map<std::string, Entry> byAtom;
    std::set<int> usedCodes;
    int idCode_ = -1;
    int constCode_ = -1;
};
