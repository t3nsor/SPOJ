// 2014-05-27
#include <iostream>
#include <cmath>
const double pi = 2*acos(0.0);
const double g = 9.806;
using namespace std;
int main() {
    int T; cin >> T;
    for (int _cs = 1; _cs <= T; _cs++) {
        double Ta, Sp; cin >> Ta >> Sp;
        double sine = g*Ta/(Sp*Sp);
        printf("Scenario #%d: ", _cs);
        if (sine > 1) { cout << "-1\n"; }
        else { printf("%.2f\n", 0.5*asin(sine)*180.0/pi); }
    }
}
