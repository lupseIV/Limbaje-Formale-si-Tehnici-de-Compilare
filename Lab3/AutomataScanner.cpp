#include "AutomataScanner.h"
#include <cctype>
#include <stdexcept>
using namespace std;

static bool isWordChar(char ch) {
    unsigned char c = static_cast<unsigned char>(ch);
    return isalnum(c) || c == '_';
}

void AutomataScanner::scan(const string& src, SymbolTable& ts, Fip& fip) {
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

    while (i < n) {
        if (isspace(static_cast<unsigned char>(src[i]))) {
            advance(1);
            continue;
        }

        // Longest match: walk while transitions exist, remember the last final state.
        int state = fa.start();
        size_t j = i, lastEnd = 0;
        string lastLabel;
        while (j < n) {
            int nextState = fa.next(state, src[j]);
            if (nextState == -1)
                break;
            state = nextState;
            j++;
            if (fa.isFinal(state)) {
                lastEnd = j;
                lastLabel = fa.label(state);
            }
        }

        if (lastLabel.empty()) {
            errors_.push_back({line, col, string("invalid character '") + src[i] + "'"});
            advance(1);
            continue;
        }

        // A constant directly followed by a letter or digit (123abc, 0x1F, 01).
        if (lastLabel == "CONST" && lastEnd < n && isWordChar(src[lastEnd])) {
            size_t end = lastEnd;
            while (end < n && isWordChar(src[end]))
                end++;
            errors_.push_back({line, col, "invalid token '" + src.substr(i, end - i) + "'"});
            advance(end - i);
            continue;
        }

        emit(src.substr(i, lastEnd - i), lastLabel, line, col, ts, fip);
        advance(lastEnd - i);
    }
}

void AutomataScanner::emit(const string& tok, const string& label,
                           int line, int col, SymbolTable& ts, Fip& fip) {
    if (label == "ID") {
        int kw = codes.find(tok, AtomType::KEYWORD);
        if (kw != -1)
            fip.add(kw);
        else
            fip.add(codes.idCode(), ts.add(tok, SymbolKind::ID));
        return;
    }
    if (label == "CONST") {
        fip.add(codes.constCode(), ts.add(tok, SymbolKind::CONST));
        return;
    }

    AtomType type;
    if (label == "KEYWORD")        type = AtomType::KEYWORD;
    else if (label == "OPERATOR")  type = AtomType::OPERATOR;
    else if (label == "SEPARATOR") type = AtomType::SEPARATOR;
    else throw runtime_error("automaton uses unsupported label '" + label + "'");

    int code = codes.find(tok, type);
    if (code == -1)
        errors_.push_back({line, col, "unknown atom '" + tok + "'"});
    else
        fip.add(code);
}
