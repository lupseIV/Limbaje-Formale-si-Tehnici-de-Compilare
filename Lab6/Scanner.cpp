#include "Scanner.h"
#include <cctype>
#include <utility>
using namespace std;

static bool isWordChar(char ch) {
    unsigned char c = static_cast<unsigned char>(ch);
    return isalnum(c) || c == '_';
}

void Scanner::scan(const string& src, SymbolTable& ts, Fip& fip) {
    size_t i = 0, n = src.size();
    int line = 1, col = 1;

    // Moves i forward by count characters, keeping line/col up to date.
    auto advance = [&](size_t count) {
        for (size_t k = 0; k < count && i < n; k++, i++) {
            if (src[i] == '\n') {
                line++;
                col = 1;
            } else {
                col++;
            }
        }
    };

    // On equal length the earlier automaton wins (a real before an integer, ...).
    const pair<Kind, const FiniteAutomaton*> candidates[] = {
        {Kind::REAL, &fa.reals},
        {Kind::INT, &fa.integers},
        {Kind::ID, &fa.identifiers},
        {Kind::OP, &fa.operatorsSeparators},
    };

    while (i < n) {
        if (isspace(static_cast<unsigned char>(src[i]))) {
            advance(1);
            continue;
        }

        // Longest match over all automata.
        int best = 0;
        Kind kind = Kind::OP;
        for (const auto& [k, automaton] : candidates) {
            int len = automaton->longestAcceptedPrefix(src, i);
            if (len > best) {
                best = len;
                kind = k;
            }
        }

        if (best == 0) {
            errors_.push_back({line, col, string("invalid character '") + src[i] + "'"});
            advance(1);
            continue;
        }

        size_t end = i + best;
        // A constant directly followed by a letter or digit (1abc, 09, 0x).
        if ((kind == Kind::INT || kind == Kind::REAL) && end < n && isWordChar(src[end])) {
            while (end < n && isWordChar(src[end]))
                end++;
            errors_.push_back({line, col, "invalid token '" + src.substr(i, end - i) + "'"});
            advance(end - i);
            continue;
        }

        emit(src.substr(i, best), kind, line, col, ts, fip);
        advance(best);
    }
}

void Scanner::emit(const string& tok, Kind kind, int line, int col,
                   SymbolTable& ts, Fip& fip) {
    switch (kind) {
    case Kind::ID: {
        int kw = codes.find(tok, AtomType::KEYWORD);
        if (kw != -1)
            fip.add(kw);
        else
            fip.add(codes.idCode(), ts.add(tok, SymbolKind::ID));
        return;
    }
    case Kind::INT:
    case Kind::REAL:
        fip.add(codes.constCode(), ts.add(tok, SymbolKind::CONST));
        return;
    case Kind::OP:
        // operators, separators and #include (a keyword) share one automaton
        for (AtomType type : {AtomType::OPERATOR, AtomType::SEPARATOR, AtomType::KEYWORD}) {
            int code = codes.find(tok, type);
            if (code != -1) {
                fip.add(code);
                return;
            }
        }
        errors_.push_back({line, col, "unknown atom '" + tok + "'"});
        return;
    }
}
