// 2025-08-28
// simple precomputation
#include <math.h>
#include <stdio.h>
#include <vector>
using namespace std;
const double pi = acos(-1.0);
void do_testcase(int cs, int N) {
    vector<double> v(N + 1);
    for (int i = 1; i <= N; i++) scanf("%lf", &v[i]);
    int bestf1, bestf2;
    double bestdiff = 1e9;
    vector<vector<double>> sins(601);
    for (int f = 400; f <= 600; f++) {
        sins[f].assign(N + 1, 0);
        for (int i = 1; i <= N; i++) {
            sins[f][i] = sin(f*pi*i/(180.0*N));
        }
    }
    for (int f1d = 400; f1d <= 600; f1d++) {
        for (int f2d = f1d; f2d <= 600; f2d++) {
            double diff = 0;
            for (int i = 1; i <= N; i++) {
                diff += fabs(v[i] - sins[f1d][i] - sins[f2d][i]);
            }
            if (diff < bestdiff) {
                bestdiff = diff;
                bestf1 = f1d;
                bestf2 = f2d;
            }
        }
    }
    printf("Case %d, f1 = %d, f2 = %d\n", cs, bestf1, bestf2);
}
int main() {
    for (int cs = 1;; cs++) {
        int N;  scanf("%d", &N); if (!N) break;
        do_testcase(cs, N);
    }
}
