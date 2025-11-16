// 2025-11-16
// This problem boils down to expressing the area of removed rock as a
// piecewise quadratic function (of the starting x-coordinate of the strip),
// then finding the minimum.  Each piece is the difference between two
// quantities: the integral of the given piecewise linear function between x and
// x + L, and the minimum value of the given piecewise linear function between x
// and x + L.  The former is quadratic in x, while the latter is linear in x.
// The former has its breakpoints at x-coordinates of given points and at such
// values minus L.  The latter has some additional breakpoints, which must
// therefore also be taken into account.  To wit, let f denote the original
// piecewise linear function, and let M(x) be the minimum value of f on the
// interval [x, x + L].
// 1. If x2 is a local minimum of f, the largest x1 < x2 such that x2 - x1 < L
//    (if any) is potentially a breakpoint of m even if it's not a breakpoint of
//    f.
// 2. Likewise, for the smallest x3 > x2 such that x3 - x2 < L (if any), x3 - L
//    is a potential breakpoint of M.
// 3. Finally, if there are two pieces of f where the one on the left has
//    positive slope and the one on the right has negative slope and there
//    exists a value y that is attained by both pieces at x-coordinates that
//    are exactly L apart, then those x-coordinates may be additional
//    breakpoints of m (technically, only if all intermediate points are on one
//    side of the horizontal line at y).
// It's possible to implement this solution in subquadratic time, but this is
// left as an exercise for the reader.
//
// NB: Using a random testcase generator, I discovered that the official
// solution by Jan Kuipers is wrong.  For the following test case:
// 6 22
// 0 1
// 6 0
// 17 10
// 22 4
// 24 4
// 36 3
// the correct answer is 44.909 (attained when the left edge of the airport is
// at x = 14) but the official solution gives 46.214 (attained when the left
// edge is at x = 9.69466).  Since the other three official solutions contain
// notes acknowledging that they're incorrect, that means ALL of the official
// solutions are incorrect!  Apparently, this is a VERY tricky problem.

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;
void do_testcase() {
    int N, L; cin >> N >> L;
    vector<int> Xo(N), Yo(N);  // original coordinates
    vector<double> X;  // merged breakpoints
    for (int i = 0; i < N; i++) cin >> Xo[i] >> Yo[i];
    for (int i = 0; i < N; i++) {
        X.push_back(Xo[i]);
        if (Xo[i] > Xo[0] + L) X.push_back(Xo[i] - L);
    }
    // find additional breakpoints
    // case 1: scan left
    for (int i = 2; i < N - 1; i++) {
        if (Yo[i] < Yo[i - 1] && Yo[i] <= Yo[i + 1]) {
            for (int j = i - 2; j >= 0; j--) {
                if (Yo[j] <= Yo[i]) {
                    double x = Xo[j] + (Yo[i] - Yo[j]) * (Xo[j + 1] - Xo[j]) /
                                       (double)(Yo[j + 1] - Yo[j]);
                    if (x + L < Xo[N - 1]) X.push_back(x);
                    break;
                }
            }
        }
    }
    // case 2: scan right
    for (int i = 1; i < N - 2; i++) {
        if (Yo[i] < Yo[i + 1] && Yo[i] <= Yo[i - 1]) {
            for (int j = i + 1; j + 1 < N; j++) {
                if (Yo[j + 1] <= Yo[i]) {
                    double x = Xo[j] + (Yo[i] - Yo[j]) * (Xo[j + 1] - Xo[j]) /
                                       (double)(Yo[j + 1] - Yo[j]);
                    if (x - L > Xo[0]) X.push_back(x - L);
                    break;
                }
            }
        } 
    }
    // case 3
    for (int i = 0; Xo[i] + L < Xo[N - 1]; i++) {
        if (Yo[i + 1] < Yo[i]) continue;
        for (int j = i + 1; j + 1 < N; j++) {
            if (Xo[j] >= Xo[i + 1] + L) break;
            if (Xo[j + 1] <= Xo[i] + L) continue;
            if (Yo[j + 1] > Yo[j] ||
                (Yo[i + 1] == Yo[i] && Yo[j + 1] == Yo[j])) continue;
            double m1 = (Yo[i + 1] - Yo[i]) / double(Xo[i + 1] - Xo[i]);
            double m2 = (Yo[j + 1] - Yo[j]) / double(Xo[j + 1] - Xo[j]);
            if (fabs(m1 - m2) < 1e-10) continue;
            double x = (Yo[j] - Yo[i] + m1*Xo[i] + m2*(L - Xo[j]))/(m1 - m2);
            if (x > Xo[i] && x < Xo[i + 1] &&
                x + L > Xo[j] && x + L < Xo[j + 1]) {
                X.push_back(x);
            }
        }
    }
    sort(X.begin(), X.end());
    X.erase(unique(X.begin(), X.end()), X.end());
    vector<double> Y(X.size());
    auto interpolate = [](double x1, double y1, double x2, double y2, double x)
                       {
                           return y1 + (x - x1)*(y2 - y1)/(x2 - x1);
                       };

    for (int i = 0; i < X.size(); i++) {
        const auto it = upper_bound(Xo.begin(), Xo.end(), X[i]) - 1;
        const int j = it - Xo.begin();
        Y[i] = interpolate(Xo[j], Yo[j], Xo[j + 1], Yo[j + 1], X[i]);
    }
    double result = 1e9;
    double lastarea, lastm, lasty2, lasts1, lasts2;
    for (int i = 0; X[i] + L <= Xo[N - 1]; i++) {
        double area = 0, m = Y[i];
        double s2;  // slope of segment right after current window
        double y2;  // y coordinate at right end of current window
        for (int j = i;
             j + 1 < X.size() && X[j] < X[i] + L + 1e-9;
             j++) {
            double xend = min(X[i] + L, X[j + 1]);
            double yend = interpolate(X[j], Y[j], X[j + 1], Y[j + 1], xend);
            area += 0.5 * (xend - X[j]) * (yend + Y[j]);
            m = min(m, yend);
            y2 = yend;
            s2 = (Y[j + 1] - Y[j]) / (X[j + 1] - X[j]);
        }
        result = min(result, area - m*L);
        if (i > 0) {
            double dx = X[i] - X[i - 1];
            // the coefficients for the quadratic that we must minimize
            double a = 0.5 * (lasts2 - lasts1);
            double b = L*(lastm - m)/dx + lasty2 - Y[i - 1];
            double c = lastarea - L*lastm;
            if (a > 1e-10) {
                double x = -0.5*b/a;
                if (x > 0 && x < X[i] - X[i - 1]) {
                    result = min(result, a*x*x + b*x + c);
                }
            }
        }
        lastarea = area;
        lasts1 = (Y[i + 1] - Y[i]) / (X[i + 1] - X[i]);
        lasts2 = s2;
        lasty2 = y2;
        lastm = m;
    }

    cout << fixed << setprecision(6) << result << '\n';
}
int main() {
    int T; cin >> T;
    while (T--) do_testcase();
}
