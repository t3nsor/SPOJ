// 2026-07-30
// `A` is a recursive algorithm for adding two integers.
// `C` computes `~(a%b)` (we don't care about the case where `b` is 0).  This
// can be inferred by examining its behavior for some small values of `a` and
// `b`, then proven inductively.
// `D` computes `a*b` mod 10^9+7.  `E` is obvious.
// `F` computes `a` to the `b`th power, mod 10^9+7.
// The condition C(i,k)*C(j,k)==1 is true only when `i%k` and `j%k` are each 0.
// We have to worry initially about overflow, but it is easy to determine that
// it cannot happen within the bounds on `n` (proof left as exercise for
// reader).  So ultimately the program is computing the sum of i^j mod 10^9+7
// where i, j fall within the range [1,n] and are relatively prime.  To speed
// this up, for each i we compute the sum of all j <= `n` that are relatively
// prime to `i` using inclusion-exclusion approach and then use a
// straightforward modular exponentiation implementation (not `F`).
#include <stdio.h>
#include <vector>
using namespace std;

using LL = long long;

constexpr int MOD = 1e9 + 7;

int mexp(int b, LL e) {
    if (e == 0) return 1;
    LL x = mexp(b, e/2);
    x = (x*x) % MOD;
    if (e&1) x = (x*b) % MOD;
    return x;
}

int pfactor[100001];

int main() {
    for (int i = 2; i <= 316; i++) {
        if (pfactor[i]) continue;
        for (int j = i; j <= 100000; j += i) pfactor[j] = i;
    }
    for (int i = 317; i <= 100000; i++) if (pfactor[i] == 0) pfactor[i] = i;

    int n;
    if (scanf("%d",&n) != 1 || n <= 0 || n > 100000) {
        puts("ERROR");
        return 0;
    }
    LL r = 1;
    for (int i = 1; i <= n; i++) {
        vector<int> sqfd = {1};
        int j = i;
        while (j > 1) {
            const int f = pfactor[j];
            while (j % f == 0) j /= f;
            const int sz = sqfd.size();
            for (int i = 0; i < sz; i++) {
                sqfd.push_back(sqfd[i]*-f);
            }
        }
        LL e = 0;
        for (auto d : sqfd) {
            bool neg = d < 0;
            if (neg) d = -d;
            LL cnt = n/d;
            e += (d*cnt*(cnt+1)/2)*(neg?-1:1);
        }
        r = (r*mexp(i,e))%MOD;
    }
    printf("%d\n", (int)r);
    scanf("%*s");
}
