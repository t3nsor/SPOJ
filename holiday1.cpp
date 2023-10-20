// 2023-10-20
// The problem description is bad at explaining this, but basically we want to
// maximize the minimum pairwise GCD after k holidays are removed. We can do
// this by binary searching to determine whether it's possible to get all
// pairwise GCDs over a certain target. This reduces to finding a clique of size
// n-k among edges whose GCDs exceed the target.
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <vector>
using namespace std;
using ll = long long;
ll p[30];
ll g[30][30];
int a[30];
bool isc1[1<<15]; int best[1<<15]; int bestm[1<<15];
bool isc2[1<<15]; int mask[1<<15];
int t, n, k;
ll gcd(ll x, ll y) {
    if (x == 0) return y;
    return gcd(y%x, x);
}
bool cmp(int x, int y) {
    return __builtin_popcount(x) < __builtin_popcount(y);
};
int main() {
    cin >> t;
    while (t--) {
        cin >> n >> k;
        for (int i = 0; i < n; i++) cin >> p[i];
        vector<ll> gcds;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                gcds.push_back(g[i][j] = gcd(p[i], p[j]));
            }
        }
        sort(gcds.begin(), gcds.end());
        gcds.erase(unique(gcds.begin(), gcds.end()), gcds.end());
        vector<int> ord1, ord2;
        for (int i = 1; i < (1 << n/2); i++) {
            ord1.push_back(i);
        }
        for (int i = 0; i < (1 << (n+1)/2); i++) {
            ord2.push_back(i);
        }
        sort(ord1.begin(), ord1.end(), cmp);
        sort(ord2.begin(), ord2.end(), cmp);
        isc1[0] = 1;
        best[0] = 0;
        int l = 0, r = gcds.size();
        int best_mask;
        for (;;) {
            int mid = (l + r) / 2;
            ll target = gcds[mid];
            for (int i = 0; i < n; i++) {
                a[i] = 0;
                for (int j = 0; j < n; j++) {
                    if (g[i][j] >= target) a[i] |= (1 << j);
                }
            }
            for (int q : ord1) {
                isc1[q] = 0; best[q] = 0;
                for (int i = 0; i < n/2; i++) {
                    if (!(q&(1<<i))) continue;
                    int q2 = q-(1<<i);
                    if (isc1[q2] && !(~a[i] & q)) {
                        isc1[q] = 1;
                        best[q] = __builtin_popcount(q);
                        bestm[q] = q;
                    } else {
                        if (best[q2] > best[q]) {
                            best[q] = best[q2];
                            bestm[q] = bestm[q2];
                        }
                    }
                }
            }
            bool success = false;
            for (int q : ord2) {
                if (q == 0) {
                    isc2[q] = 1;
                    mask[q] = (1 << n) - 1;
                } else {
                    int b = __builtin_ctz(q);
                    int q2 = q-(1<<b);
                    isc2[q] = isc2[q2] && !(~(a[b+n/2] >> (n/2)) & q);
                    if (isc2[q]) mask[q] = mask[q2] & a[b+n/2]; else continue;
                }
                int m = mask[q] & ((1 << (n/2)) - 1);
                int c = __builtin_popcount(q) + best[m];
                if (c >= n - k) {
                    success = true;
                    best_mask = (q << (n/2)) + bestm[m];
                }
            }
            if (success) {
                if (l == mid) break; else l = mid;
            } else {
                r = mid;
            }
        }
        int remaining = __builtin_popcount(best_mask);
        int remove = remaining - (n - k);
        int first = true;
        for (int i = 0; i < n; i++) {
            if (!(best_mask & (1 << i))) {
                if (!first) putchar(' ');
                printf("%d", i + 1);
                first = false;
            } else if (remove) {
                --remove;
                if (!first) putchar(' ');
                printf("%d", i + 1);
                first = false;
            }
        }
        putchar('\n');
    }
}
