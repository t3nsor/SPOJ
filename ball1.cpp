// 2023-04-16
// The easiest way to solve this problem is to write a simulation function and
// then investigate the empirical relationship between the input values and the
// answer. Assume the ball's position is given by (x, y) where 0 <= x <= X and
// 0 <= y <= Y (therefore, X and Y are each 1 less than the input values in the
// format given in the problem description). Without loss of generality, suppose
// the initial position is (x0, 0). If gcd(X, Y) divides x0, the ball will
// eventually bounce into a corner and the answer will be 2 (as in the diagram
// at the left). Otherwise, we find empirically that the answer depends only on
// X, Y and not on x0. In general the number of steps taken by the ball before
// it returns to its starting position will be 2 lcm(X, Y) (a.k.a. 2XY/G, where
// G = gcd(X, Y)) but some positions will be occupied twice. By examining the
// difference between 2 lcm(X, Y) and the empirically determined answer,
// particularly by fixing X and then seeing how the difference depends on Y, it
// doesn't take long to guess the correct formula.
#include <iostream>
using namespace std;
int gcd(int x, int y) {
    if (x == 0) return y;
    return gcd(y % x, x);
}
int calculate(int X, int Y, int x0) {
    const int G = gcd(X, Y);
    if (x0 % G == 0) return 2;
    return 2*X*Y/G - 4*(X/G)*(Y/G) + 2*X/G + 2*Y/G;
}
int main() {
    int d; cin >> d; while (d--) {
        int X, Y, a, b; cin >> X >> Y >> a >> b; X--; Y--; a--; b--;
        if (b != 0 && b != Y) {
            swap(X, Y);
            swap(a, b);
        }
        cout << calculate(X, Y, a) << '\n';
    }
}
