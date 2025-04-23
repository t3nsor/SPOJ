// 2025-04-23
#include <algorithm>
#include <iostream>
using namespace std;
// returns d = gcd(a,b); finds x,y such that d = ax + by
// (taken from Stanford codebook)
int extended_euclid(int a, int b, int &x, int &y) {
    int xx = y = 0;
    int yy = x = 1;
    while (b) {
        int q = a/b;
        int t = b; b = a%b; a = t;
        t = xx; xx = x-q*xx; x = t;
        t = yy; yy = y-q*yy; y = t;
    }
    return a;
}

// return smallest nonnegative x such that ax = b (mod m)
int solve(int a, int b, int m) {
    a = (a%m+m)%m; b = (b%m+m)%m;
    int x, _, g = extended_euclid(a, m, x, _);
    if (b % g) return 1e9; b /= g;
    m /= g;
    return (((long long)b*x)%m+m)%m;
}

int main() {
    int C[15], P[15], L[15];
    int T; cin >> T; while (T--) {
        int N; cin >> N; int M = 0;
        for (int i = 0; i < N; i++) {
            cin >> C[i] >> P[i] >> L[i]; --C[i];
            if (C[i] >= M) M = C[i] + 1;
        }
top:
        for (int i = 1; i < N; i++) {
            for (int j = 0; j < i; j++) {
                if (solve(P[i] - P[j], C[j] - C[i], M) <= min(L[i], L[j])) {
                    M++; goto top;
                }
            }
        }
        cout << M << '\n';
    }
}
