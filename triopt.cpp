// 2024-09-01
// Zero is not considered a natural number in this problem (which is why S is
// always at least 3).
// To maximize or minimize e.g. sin(x) + sin(y) + sin(z) subject to
// x + y + z = S, we could try all values of x from 1 to S - 2, and for each
// such value look up the max/min of sin(y) + sin((S - x) - y) subject to
// 0 < y < S - x. The problem then becomes how to precompute all max/min of
// sin(y) + sin(N - y) for each N in 2, ..., 999999. We can do this using the
// identity sin(a) + sin(b) = 2 sin((a+b)/2) cos((a-b)/2), noting that for each
// value of N the sin factor in this expression is a constant. We then just need
// to know the max/min of the set {cos(N/2 - 1), cos(N/2 - 2), ...}, for which
// we keep 2 running max/mins depending on the parity of N as we loop through
// all N. We can similarly precompute a table of all max/min of
// cos(y) + cos(N - y). These two tables allow us to handle all possible
// combinations of sin and cos functions in the input.
#include <algorithm>
#include <iostream>
#include <math.h>
#include <string>
using namespace std;
double sin_table[1999997];
double cos_table[1999997];
double ssmin[1000000];
double ssmax[1000000];
double ccmin[1000000];
double ccmax[1000000];
void do_testcase() {
    int nsin = 0;
    for (int i = 0; i < 3; i++) {
        string fn; cin >> fn; if (fn == "sin") ++nsin;
    }
    string obj; cin >> obj;
    int S; cin >> S;
    double result;
    if (obj == "min") {
        result = 4;
        if (nsin == 0) {
            for (int i = 1; i <= S - 2; i++) {
                result = min(result, cos_table[2*i] + ccmin[S - i]);
            }
        } else if (nsin == 1) {
            for (int i = 1; i <= S - 2; i++) {
                result = min(result, sin_table[2*i] + ccmin[S - i]);
            }
        } else if (nsin == 2) {
            for (int i = 1; i <= S - 2; i++) {
                result = min(result, cos_table[2*i] + ssmin[S - i]);
            }
        } else {
            for (int i = 1; i <= S - 2; i++) {
                result = min(result, sin_table[2*i] + ssmin[S - i]);
            }
        }
    } else {
        result = -4;
        if (nsin == 0) {
            for (int i = 1; i <= S - 2; i++) {
                result = max(result, cos_table[2*i] + ccmax[S - i]);
            }
        } else if (nsin == 1) {
            for (int i = 1; i <= S - 2; i++) {
                result = max(result, sin_table[2*i] + ccmax[S - i]);
            }
        } else if (nsin == 2) {
            for (int i = 1; i <= S - 2; i++) {
                result = max(result, cos_table[2*i] + ssmax[S - i]);
            }
        } else {
            for (int i = 1; i <= S - 2; i++) {
                result = max(result, sin_table[2*i] + ssmax[S - i]);
            }
        }
    }
    printf("%.10f\n", result);
}
int main() {
    for (int i = 0; i <= 1999996; i++) {
        sin_table[i] = sin(i / 2.0);
        cos_table[i] = cos(i / 2.0);
    }
    double cmin[2] = {2, 2};
    double cmax[2] = {-2, -2};
    for (int N = 2; N <= 999999; N++) {
        cmin[N % 2] = min(cmin[N % 2], cos_table[N - 2]);
        cmax[N % 2] = max(cmax[N % 2], cos_table[N - 2]);
        const double s = 2 * sin_table[N];
        if (s > 0) {
            ssmin[N] = s * cmin[N % 2];
            ssmax[N] = s * cmax[N % 2];
        } else {
            ssmin[N] = s * cmax[N % 2];
            ssmax[N] = s * cmin[N % 2];
        }
        const double c = 2 * cos_table[N];
        if (c > 0) {
            ccmin[N] = c * cmin[N % 2];
            ccmax[N] = c * cmax[N % 2];
        } else {
            ccmin[N] = c * cmax[N % 2];
            ccmax[N] = c * cmin[N % 2];
        }
    }
    int T; cin >> T; while (T--) do_testcase();
}
