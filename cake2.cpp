// 2026-04-25
// The idea is pretty simple: every time a new line is added, the number of
// regions increases by I + 1, where I is the number of points at which the new
// line intersects existing lines (this may be less than the number of existing
// lines, in case some are parallel to the new line or some intersection points
// are not unique).  To count unique intersection points this code uses a simple
// sorting-based approach (optimizing it is left as an exercise for the reader).
// We express the intersection points parametrically, i.e. we find the numerator
// and denominator of t such that the intersection point is at (x1 + t(x2 - x1),
// y1 + t(y2 - y1)).  This allows us to fit all results of calculations in the
// range of `int` instead of having to go to `long long`.
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <utility>
#include <vector>
using namespace std;
int gcd(int x, int y) {
    return x == 0 ? y : gcd(y % x, x);
}
int main() {
    int N; cin >> N;
    vector<int> X1(N), Y1(N), X2(N), Y2(N);
    int result = 1;
    for (int i = 0; i < N; i++) {
        cin >> X1[i] >> Y1[i] >> X2[i] >> Y2[i];
        const int x1 = X1[i];
        const int y1 = Y1[i];
        const int x2 = X2[i];
        const int y2 = Y2[i];
        vector<pair<int, int>> isct;
        for (int j = 0; j < i; j++) {
            const int x3 = X1[j];
            const int y3 = Y1[j];
            const int x4 = X2[j];
            const int y4 = Y2[j];
            int den = (x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4);
            if (den == 0) continue;
            int num = (x1 - x3)*(y3 - y4) - (y1 - y3)*(x3 - x4);
            const int g = gcd(abs(num), abs(den));
            num /= g;
            den /= g;
            if (den < 0) { num = -num; den = -den; }
            isct.emplace_back(num, den);
        }
        sort(isct.begin(), isct.end());
        int ucnt = 0;
        for (int j = 0; j < isct.size(); j++) {
            if (j == 0 || isct[j] != isct[j - 1]) ++ucnt;
        }
        result += ucnt + 1;
    }
    cout << result << '\n';
}
