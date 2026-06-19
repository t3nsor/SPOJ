// 2026-06-19
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
const double pi = acos(-1.0);
double hypot3(double x, double y, double z) {
    return hypot(x, hypot(y, z));
}
double do_testcase(int n) {
    double x1, y1, z1; scanf("%lf %lf %lf", &x1, &y1, &z1);
    double x2, y2, z2; scanf("%lf %lf %lf", &x2, &y2, &z2);
    if (x1 > x2) swap(x1, x2);
    if (y1 > y2) swap(y1, y2);
    if (z1 > z2) swap(z1, z2);
    vector<double> px(n), py(n), pz(n);
    vector<int> perm(n);
    for (int i = 0; i < n; i++)  {
        perm[i] = i;
        scanf("%lf %lf %lf", &px[i], &py[i], &pz[i]);
    }
    double result = 0;
    do {
        vector<double> bx, by, bz; vector<double> br;
        double cur = 0;
        for (int i = 0; i < n; i++) {
            const double x = px[perm[i]],
                         y = py[perm[i]],
                         z = pz[perm[i]];
            double r = min({x2 - x, x - x1, y2 - y, y - y1, z2 - z, z - z1});
            for (int j = 0; j < bx.size(); j++) {
                r = min(r, hypot3(bx[j] - x, by[j] - y, bz[j] - z) - br[j]);
            }
            if (r <= 0) continue;
            cur += 4*pi*r*r*r/3;
            bx.push_back(x);
            by.push_back(y);
            bz.push_back(z);
            br.push_back(r);
        }
        result = max(result, cur);
    } while (next_permutation(perm.begin(), perm.end()));
    return (x2 - x1)*(y2 - y1)*(z2 - z1) - result;
}
int main() {
    int cs = 1;
    for (;;) {
        int n; scanf("%d", &n);
        if (n == 0) break;
        printf("Box %d: %lld\n\n", cs++, (long long)(do_testcase(n) + 0.5));
    }
}
