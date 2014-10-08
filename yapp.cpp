// 2014-10-07
#include <cstdio>
const int MOD = 1000000007;
int powermod(int base, int exponent, int mod) {
    int res = 1;
    int pwr = base;
    int cur = 1;
    while (cur <= exponent) {
        if (cur & exponent) {
            res = (res*(long long)pwr)%mod;
        }
        pwr = (pwr*(long long)pwr)%mod;
        cur <<= 1;
    }
    return res;
}
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        int N; scanf("%d", &N);
        printf("%d\n", powermod(2, N-1, MOD));
    }
    return 0;
}
