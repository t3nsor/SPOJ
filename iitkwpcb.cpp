// 2014-09-22
#include <cstdio>
long long gcd(long long x, long long y) {
    if (x == 0) return y;
    else return gcd(y%x, x);
}
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        long long n; scanf("%lld", &n);
        long long m = n/2;
        for (;;) {
            if (gcd(n, m) == 1) {
                printf("%lld\n", m);
                break;
            }
            m--;
        }
    }
    return 0;
}
