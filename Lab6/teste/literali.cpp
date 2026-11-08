#include <iostream>
using namespace std;

int main() {
    int d, o, h, b;
    double r;
    d = 1'000'000 + 42u + 7l + 7ul + 7LL + 7llu + 0;
    o = 017 + 0'7;
    h = 0x1F + 0XaBc + 0xFFu;
    b = 0b101 + 0B1'0;
    r = 3.14 + 3. + .5 + 1e10 + 2.5e-3 + 1E+2 + 2.5f + 1.0L;
    cout << d << endl;
    cout << o << endl;
    cout << h << endl;
    cout << b << endl;
    cout << r << endl;
    return 0;
}
