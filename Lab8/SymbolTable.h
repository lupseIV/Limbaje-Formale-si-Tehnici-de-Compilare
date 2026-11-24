#pragma once
#include <string>
#include <vector>

enum class SymbolKind { ID, CONST };

struct Symbol {
    std::string text;
    SymbolKind kind;
    int id;
};

class SymbolTable {
public:
    int add(const std::string& text, SymbolKind kind);

    std::vector<int> positionsById() const;

    const Symbol& at(int pos) const { return entries[pos]; }
    int size() const { return (int)entries.size(); }

    bool save(const std::string& path) const;

private:
    int lowerBound(const std::string& key) const;

    std::vector<Symbol> entries;
    int nextId = 0;
};
