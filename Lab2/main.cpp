#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
const set<string> KEYWORDS = {
    "int", "double", "struct", "if", "else", "while",
    "cin", "cout", "endl", "return", "main",
    "#include", "iostream", "using", "namespace", "std"
};

const set<string> OPERATORS = {
    "+", "-", "*", "/", "=", "==", "!=",
    "<", "<=", ">", ">=", "<<", ">>"
};
const set<string> SEPARATORS = { "(", ")", "{", "}", ";", ",", "." };

string classify(const string& tok) {
    if (KEYWORDS.count(tok))   return "reserved keyword";
    if (OPERATORS.count(tok))  return "operator";
    if (SEPARATORS.count(tok)) return "separator";
    if (isalpha(tok[0]) || tok[0] == '_')       return "identifier";
    return "constant";
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " filename" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file) {
        cerr << "Error: could not open " << argv[1] << endl;
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string src = buffer.str();

    size_t i = 0;
    int line = 1;

    while (i < src.size()) {
        char c = src[i];

        if (c == '\n') {
            line++;
            i++;
            continue;
        }
        if (isspace(c)) {
            i++;
            continue;
        }

        if (isalpha(c) || c == '_') {
            string word;
            while (i < src.size() && (isalnum(src[i]) || src[i] == '_')) {
                word += src[i];
                i++;
            }

            cout << "Line " << line << ": " << word  << " -> " << classify(word) << endl;
        } else if (isdigit(c)) {
            string constant;
            while (i < src.size() && isdigit(src[i])) {
                constant += src[i];
                i++;
            }

            if (i+1 < src.size() && src[i] == '.' && isdigit(src[i+1])) {
                constant += src[i];
                i++;
                while (i < src.size() && isdigit(src[i])) {
                    constant += src[i];
                    i++;
                }
            }

            if (i < src.size() && (isalpha(src[i]) || src[i] == '_')) {
                while (i < src.size() && (isalnum(src[i]) || src[i] == '_')) {
                    constant += src[i];
                    i++;
                }
                cout << "Line " << line << ": LEXICAL ERROR - invalid constant '" << constant << "'\n";
            } else {
                cout << "Line " << line << ": " << constant << " -> " << classify(constant) << "\n";
            }
        } else if (c == '"') {
            string s_constant;
            s_constant += src[i];
            i++;

            while (i < src.size() && src[i] != '"' && src[i] != '\n') {
                if (src[i] == '\\' && i + 1 < src.size() && src[i + 1] != '\n') {
                    s_constant += src[i];
                    s_constant += src[i + 1];
                    i += 2;
                } else {
                    s_constant += src[i];
                    i++;
                }
            }

            if (i < src.size() && src[i] == '"') {
                s_constant += src[i];
                i++;
                cout << "Line " << line << ": " << s_constant
                     << " -> " << classify(s_constant) << "\n";
            } else {
                cout << "Line " << line << ": LEXICAL ERROR - unterminated string "
                     << s_constant << "\n";
            }
        } else {

            if ( c == '#' && src.substr(i,8) == "#include") {
                cout << "Line " << line << ": " << "#include" << " -> " << classify("#include") << "\n";
                i += 8;
                continue;
            }

            if (i + 1 < src.size()) {
                string two = string(1, src[i]) + src[i + 1];
                if (OPERATORS.count(two)) {
                    cout << "Line " << line << ": " << two << " -> " << classify(two) << "\n";
                    i += 2;
                    continue;
                }
            }

            string one(1, src[i]);
            if (OPERATORS.count(one) || SEPARATORS.count(one)) {
                cout << "Line " << line << ": " << one << " -> " << classify(one) << "\n";
            } else {
                cout << "Line " << line << ": LEXICAL ERROR - invalid character '" << one << "'\n";
            }
            i++;

        }
    }

    return 0;
}