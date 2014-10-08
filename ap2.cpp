// 2014-04-24
#include <cstdio>
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        long long x, y, sum;
        scanf("%lld %lld %lld", &x, &y, &sum);
        long long N = (2*sum)/(x+y);
        long long d = (y - x)/(N - 5);
        printf("%lld\n", N);
        x -= 3*d;
        for (long long i = 0; i < N - 1; i++) {
            printf("%lld ", x += d);
        }
        printf("%lld\n", x += d);
    }
}
