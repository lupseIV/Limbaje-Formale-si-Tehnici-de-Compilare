#include <iostream>
using namespace std;

struct cerc {
    double raza;
};

int main() {
    cerc c;
    double p, a;
    cin >> c.raza;
    p = 2 * 3.14159 * c.raza;
    a = 3.14159 * c.raza * c.raza;
    cout << p << endl;
    cout << a << endl;
    return 0;
}