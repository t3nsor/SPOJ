// 2021-10-01
#include <cstdlib>
#include <iostream>
#include <utility>
using namespace std;
// requires a, b >= 0
// returns gcd(a, b) and sets x, y such that ax + by = gcd
int extended_euclid(int a, int b, int& x, int& y) {
    if (a == 0) {
        x = 0; y = 1; return b;
    }
    int x2, y2;
    const int result = extended_euclid(b%a, a, x2, y2);
    // (b%a)*x2 + a*y2 = result
    // -> (b%a)*x2 + (a*(b/a))*x2 + a*y2 = result + (a*(b/a))*x2
    // -> b*x2 + a*y2 = result + (a*(b/a))*x2
    // -> a*(y2-(b/a)*x2) + b*x2 = result
    y = x2;
    x = y2 - (b/a) * x2;
    return result;
}
void do_testcase() {
    int a, b, c, d; cin >> a >> b >> c >> d;
    if (a == 0 && c == 0) {
        // swap x coordinates with y coordinates; this is equivalent
        // to the original problem
        swap(a, b);
        swap(c, d);
    }
    if (a < 0) {
        // replace (a, b) with its additive inverse; again equivalent
        a *= -1;
        b *= -1;
    }
    if (c < 0) {
        // ditto
        c *= -1;
        d *= -1;
    }
    int p0, q0;
    const int GCD = extended_euclid(a, c, p0, q0);
    const int m = abs(a) + abs(b);
    int result = m;
    // try all possible x-coordinates of linear combinations of (a, b), (c, d)
    // but don't go beyond |a|+|b| in absolute value because the minimum has to
    // be at least as good as |a|+|b|.
    // For each x-coordinate value (which we call `target`) below, we compute
    // coefficients p, q such that ap + cq = target, and then adjust them in
    // order to minimize the absolute y-coordinate, |bp + dq|.
    for (int target = m - (m % GCD); target > -m; target -= GCD) {
        long long p = p0, q = q0;
        p *= (target / GCD);
        q *= (target / GCD);
        auto cur = b * p + d * q;  // p, q are already long long
        // by adjusting p, q we can change this value by
        // increments of (ad - bc)/gcd(a, c)
        auto increment = ((long long)a * d - (long long)b * c) / GCD;
        if (increment) {
            increment = abs(increment);
            cur = ((cur % increment) + increment) % increment;
            if (abs(cur - increment) < cur) {
                cur -= increment;
            }
        }
        if (target == 0 && cur == 0) {
            // this isn't allowed, so make cur the smallest nonzero value
            if (increment == 0) continue;
            cur += increment;
        }
        result = min<long long>(result, abs(target) + abs(cur));
    }
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    for (int i = 0; i < 10; i++) {
        do_testcase();
    }
}
