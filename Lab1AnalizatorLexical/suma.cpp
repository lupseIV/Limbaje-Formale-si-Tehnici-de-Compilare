#include <iostream>
using namespace std;

int main() {
    int n, i, x, s;
    cin >> n;
    s = 0;
    i = 0;
    while (i < n) {
        cin >> x;
        s = s + x;
        i = i + 1;
    }
    cout << s << endl;
    return 0;
}
