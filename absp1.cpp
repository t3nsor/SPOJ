// 2014-10-07
#include <cstdio>
int a[10000];
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        int N; scanf("%d", &N);
        long long res = 0;
        long long sum = 0;
        for (int i = 0; i < N; i++) {
            scanf("%d", a + i);
            if (i > 0) {
                res += i*(long long)a[i] - sum;
            }
            sum += a[i];
        }
        printf("%lld\n", res);
    }
    return 0;
}
