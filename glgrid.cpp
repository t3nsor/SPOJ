// 2026-08-01
// The problem statmeent is unclear; based on the sample data it appears to
// simply be asking for the k smallest nonnegative values of x such that
// ax^2 + bx + c is a perfect square (but the first time you read it you might
// think that the value of x also must be such that what the algorithm actually
// does is independent of the order of the div and mod operations; apparently
// that's not what the problem statement means).  If b^2 - 4ac = 0 this is
// trivial.  Otherwise, if a = 0, that is a special case; otherwise, let p
// denote 2ax + b; then if ax^2 + bx + c is a perfect square, it must be the
// case that p^2 - 4aq^2 = b^2 - 4ac where p, q are integers (we have to filter
// out values of p that don't yield an integer value of x).  If `a` is a perfect
// square then the left side factors; otherwise, we have a generalized Pell
// equation and solve it using the technique described here:
// https://kconrad.math.uconn.edu/blurbs/ugradnumthy/pelleqn2.pdf

#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;

using LL = long long;

LL pell4a[101] = {
  -1, -1, 3, 7, -1, 9, 5, 127, 17, -1,
  19, 199, 7, 649, 15, 31, -1, 33, 17, 57799,
  9, 55, 197, 1151, 49, -1, 51, 1351, 127, 9801,
  11, 4620799, 577, 23, 35, 71, -1, 73, 37, 25,
  721, 2049, 13, 24248647, 199, 161, 24335, 4607, 97, -1,
  99, 4999, 649, 66249, 485, 89, 15, 151, 19603, 561799,
  31, 1766319049, 63, 127, -1, 129, 65, 4771081927, 33, 7775,
  251, 24220799, 17, 2281249, 3699, 1351, 57799, 351, 53, 12799,
  161, -1, 163, 13447, 55, 285769, 10405, 1567, 77617, 500001,
  19, 4954951, 1151, 12151, 2143295, 39, 4801, 62809633, 99, 199,
  -1
};

void do_testcase() {
    int a, b, c, k; scanf("%d %d %d %d", &a, &b, &c, &k);
    const int n = b*b - 4*a*c;
    if (n == 0) {
        for (int i = 0; i < k; i++) { printf("%d\n", i); }
        return;
    }
    if (a == 0) {
        // note b = 0 is already covered by the case above
        // if bx + c = q^2 for some q, then q^2 = c (mod b)
        vector<LL> r;
        for (int q = 0; q < b; q++) if (q*q % b == c % b) r.push_back(q);
        for (LL m = 0;; m++) {
            for (int i = 0; i < r.size(); i++) {
                const LL q = m*b + r[i];
                if (q*q < c) continue;
                printf("%lld\n", (q*q-c)/b);
                if (0 == --k) return;
            }
        }
        throw;
    }
    if (pell4a[a] == -1) {
        // perfect square case, a != 0
        // (p + 2rq)(p - 2rq) = b^2 - 4ac where r = sqrt(a)
        const int r = sqrt(a);
        for (int i = sqrt(n); i >= 1; i--) {
            if (n % i) continue;
            const int j = n / i;
            if ((i + j) % 2) continue;
            const int p = (i + j) / 2;
            if ((p - i) % (2*r)) continue;
            if ((p - b) % (2*a)) continue;
            printf("%d\n", (p - b)/(2*a));
            if (0 == --k) return;
        }
        throw;
    }
    // general case
    // oops I used `x` and `y` below when I should have used `p` and `q`
    const LL d = 4*a;
    const LL x1 = pell4a[a];
    const LL y1 = x1/sqrt(d);
    vector<pair<LL, LL>> fund;
    const LL xmax = LL(2e12)*a + b;
    if (n == 1) {
        fund.emplace_back(1, 0);
    } else {
        const LL ymax = sqrt(n*pell4a[a]/(2*d));
        // ymax <= 1726825
        for (LL y = 0; y <= ymax; y++) {
            const LL x = sqrt(d*y*y + n);
            if (x*x - d*y*y != n) continue;
            fund.emplace_back(x, y);
        }
        // Handle fundamental solutions with negative y.
        // For sufficiently large k, (x1 + y1 sqrt(d))^k (x - y sqrt(d)) is
        // eventually of the form x' + y' sqrt(d) with y' > 0.  To determine how
        // far we need to search, consider the following:
        //   (x - y sqrt(d)) = n/(x + y sqrt(d)),
        // implying that the larger y is, the smaller the real value of
        // x - y sqrt(d).  That means that as long as
        // (x1 + y1 sqrt(d))^k (x - y sqrt(d)) continues to produce negative y,
        // since the real value is getting bigger with increasing k, the value
        // of y must be getting smaller.  That means we don't need to worry
        // about hitting overflow before we get to a solution with positive y.
        const int np = fund.size();
        for (int i = 0; i < np; i++) {
            LL x = fund[i].first;
            LL y = -fund[i].second;
            while (y < 0) {
                LL t = x1*x + d*y1*y;
                y = y1*x + x1*y;
                x = t;
            }
            bool unique = true;
            for (int j = 0; j < np; j++) if (x == fund[i].first) unique = false;
            if (unique && x <= xmax) fund.emplace_back(x, y);
        }
    }
    vector<LL> solns;
    for (const auto f : fund) {
        LL x = f.first;
        LL y = f.second;
        for (;;) {
            if ((x - b) % (2*a) == 0) {
                solns.push_back((x - b) / (2*a));
            }
            if ((y > 0 && d*y1 > xmax / y) || x1 > xmax / x) break;
            LL t = x1*x + d*y1*y;
            y = y1*x + x1*y;
            x = t;
            if (x > xmax) break;
        }
    }
    sort(solns.begin(), solns.end());
    if (solns.size() < k) throw;
    for (int i = 0; i < k; i++) {
        printf("%lld\n", solns[i]);
    }
}

int main() {
    int T; scanf("%d", &T); while (T--) do_testcase();
}
