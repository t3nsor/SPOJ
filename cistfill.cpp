// 2014-05-02
#include <cstdio>
#include <utility>
#include <algorithm>
using namespace std;
int b[50000], h[50000], a[50000];
pair<int, int> A[100000];
int main() {
    int k; scanf("%d", &k);
    while (k--) {
        int n; scanf("%d", &n);
        for (int i = 0; i < n; i++) {
            int x, y; scanf("%d %d %d %d", b + i, h + i, &x, &y);
            a[i] = x*y;
            A[2*i] = make_pair(b[i], i);
            A[2*i+1] = make_pair(b[i] + h[i], i);
        }
        sort(A, A+2*n);
        int V; scanf("%d", &V);
        int area = 0, last = 0;
        for (int i = 0; i < 2*n && V > 0; i++) {
            pair<int, int> P = A[i];
            if (V <= (P.first - last)*area) {
                printf("%.2f\n", last + (double)V/area);
            }
            V -= (P.first - last)*area;
            if (P.first == b[P.second]) {
                area += a[P.second];
            } else {
                area -= a[P.second];
            }
            last = P.first;
        }
        if (V > 0) {
            puts("OVERFLOW");
        }
    }
}
