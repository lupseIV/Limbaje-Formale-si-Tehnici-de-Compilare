#include <iostream>
using namespace std;

int main() {
    int a, b;
    cin >> a >> b;
    a = a % b;
    b = 0x1F;
    cout << a << endl;
    cout << b << endl;
    return 0;
}
