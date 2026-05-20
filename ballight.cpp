// 2026-05-20
// The problem description on SPOJ is wrong, even though it appears to have
// been copied verbatim from the source.  The digit in the ten-thousandths
// place should be rounded to even (the default for `printf` on common
// platforms), while the problem description says "rounded up" (which in
// English normally means rounded toward infinity).  Perhaps it's a
// mistranslation from the original Spanish or Portuguese.  This is why having
// access to old test data can be really helpful!
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
const double pi = acos(-1.0);
double clmp(double x, double a, double b) {
    return max(a, min(b, x));
}
double do_testcase(int L, int C, double X, double Y) {
    vector<double> lx(L), ly(L);
    vector<double> cx(C), cy(C), cr(C);
    for (int i = 0; i < L; i++) scanf("%lf %lf", &lx[i], &ly[i]);
    for (int i = 0; i < C; i++) scanf("%lf %lf %lf", &cx[i], &cy[i], &cr[i]);
    // 0: bottom, 1: right, 2: top, 3: left
    vector<pair<double, bool>> lit[4];
    for (int i = 0; i < L; i++) {
        vector<pair<double, bool>> dark;
        int depth = 0;
        for (int j = 0; j < C; j++) {
            const double x = cx[j] - lx[i];
            const double y = cy[j] - ly[i];
            double a = atan2(y, x);
            if (a < 0) a += 2*pi;
            const double b = asin(cr[j] / hypot(x, y));
            double c = a - b;
            if (c < 0) c += 2*pi;
            double d = a + b;
            if (d >= 2*pi) d -= 2*pi;
            dark.emplace_back(c, false);
            dark.emplace_back(d, true);
            if (d < c) ++depth;
        }
        sort(dark.begin(), dark.end());
        double last = 0.0;
        vector<pair<double, double>> local_lit;
        for (const auto p : dark) {
            if (depth == 0 && p.first > last) {
                local_lit.emplace_back(last, p.first);
            }
            if (p.second) --depth; else ++depth;
            last = p.first;
        }
        if (last < 2*pi && depth == 0) {
            local_lit.emplace_back(last, 2*pi);
        }
        for (const auto p : local_lit) {
            const double a = p.first;
            const double b = p.second;
            if (b > pi) {
                const double x1 = a <= pi
                                  ? 0
                                  : clmp(lx[i] - (ly[i] / tan(a)), 0.0, X);
                const double x2 = clmp(lx[i] - (ly[i] / tan(b)), 0.0, X);
                lit[0].emplace_back(x1, false);
                lit[0].emplace_back(x2, true);
            }
            if (a < 0.5*pi) {
                const double y1 = clmp(ly[i] + (X - lx[i])*tan(a), 0.0, Y);
                const double y2 = b >= 0.5*pi
                                  ? Y
                                  : clmp(ly[i] + (X - lx[i])*tan(b), 0.0, Y);
                lit[1].emplace_back(y1, false);
                lit[1].emplace_back(y2, true);
            } else if (b > 1.5*pi) {
                const double y1 = a <= 1.5*pi
                                  ? 0
                                  : clmp(ly[i] + (X - lx[i])*tan(a), 0.0, Y);
                const double y2 = clmp(ly[i] + (X - lx[i])*tan(b), 0.0, Y);
                lit[1].emplace_back(y1, false);
                lit[1].emplace_back(y2, true);
            }
            if (a < pi) {
                const double x1 = clmp(lx[i] + (Y - ly[i]) / tan(a), 0.0, X);
                const double x2 = b >= pi
                                  ? 0
                                  : clmp(lx[i] + (Y - ly[i]) / tan(b), 0.0, X);
                lit[2].emplace_back(x2, false);
                lit[2].emplace_back(x1, true);
            }
            if (a < 1.5*pi && b > 0.5*pi) {
                const double y1 = a <= 0.5*pi
                                  ? Y
                                  : clmp(ly[i] - lx[i]*tan(a), 0.0, Y);
                const double y2 = b >= 1.5*pi
                                  ? 0
                                  : clmp(ly[i] - lx[i]*tan(b), 0.0, Y);
                lit[3].emplace_back(y2, false);
                lit[3].emplace_back(y1, true);
            }
        }
    }
    double result = 0;
    for (int i = 0; i < 4; i++) {
        sort(lit[i].begin(), lit[i].end());
        double last = 0;
        int depth = 0;
        for (const auto& p : lit[i]) {
            if (depth > 0) result += p.first - last;
            if (p.second) --depth; else ++depth;
            last = p.first;
        }
        if (depth > 0) result += (i % 2 == 0 ? X : Y) - last;
    }
    return result;
}
int main() {
    for (;;) {
        int L, C, X, Y;
        scanf("%d %d %d %d", &L, &C, &X, &Y);
        if (L + C + X + Y == 0) break;
        printf("%.4f\n", do_testcase(L, C, X, Y));
    }
}
