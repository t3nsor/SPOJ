// 2024-09-05
// Problem statement makes no sense... in the 8th bullet point it says that the
// value of the pot increases when a player wins. But it is what it is.
#include <iostream>
#include <stdlib.h>
using namespace std;
int main() {
    int a, b, c;
    for (;;) {
        cin >> a >> b >> c;
        if (a == 0 && b == 0 && c == 0) return 0;
        if ((a - c) % b != 0) {
            cout << "No accounting tablet\n";
        } else {
            cout << (abs((a - c) / b) + 2) / 3 << '\n';
        }
    }
}
