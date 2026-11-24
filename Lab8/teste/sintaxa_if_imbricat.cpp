#include <iostream>
using namespace std;

int main() {
    int a, b;
    cin >> a >> b;
    if (a < b)
        if (a == 0) cout << a << endl; else cout << b << endl;
    else {
        a = (a + b) * 2 / 3;
    }
    return 0;
}
