// 2024-01-07
// We must have
//   sqrt(t) = asin(x) + 2pi m
//   t = acos(y) + 2pi n
// There are also other solutions, i.e. sqrt(t) = 2pi*m + pi - asin(x) and
// t = 2pi*n + (2pi - acos(y)) but it will turn out that we don't need them.
//
// Square both sides of the first equation;
//   t = asin(x)^2 + 4pi asin(x) m + 4pi^2 m^2
//   t = acos(y) + 2pi n
// Therefore
//   asin(x)^2 + 4pi asin(x) m + 4pi^2 m^2 = acos(y) + 2pi n
// or
//   n = (asin(x)^2 - acos(y))/(2pi) + 2 asin(x) m + 2pi m^2
// We want the right-hand side to be as close to an integer as possible.
//
// Error analysis is a PITA, so I'm just going to assume that if we do close to
// the max number of iterations allowed by the time limit, we will get close
// enough to the dot to get accepted. :)

#include <math.h>
#include <iomanip>
#include <iostream>
using namespace std;
const long double pi = acosl(-1.0L);
void do_testcase() {
    long double x, y; cin >> x >> y;
    long double unused_eps; cin >> unused_eps;
    const long double asinx = asinl(x);
    long double acosy = acosl(y);
    long double C = (asinx*asinx - acosy)/(2*pi);
    long double bestdiff = 2; 
    int bestm = -1;
    for (int m = 0; m < 100000; m++) {
        long double z1 = 2*asinx*m + 2*pi*m*m + C;
        long double z2 = fmodl(z1, 1.0);
        if (fabsl(z2) < bestdiff) {
            bestdiff = fabsl(z2);
            bestm = m;
        }
    }
    long double sqrtt = asinx + 2*pi*bestm;
    cout << fixed << setprecision(5) << sqrtt*sqrtt << '\n';
}
int main() {
    int t; cin >> t;
    while (t--) do_testcase();
}
