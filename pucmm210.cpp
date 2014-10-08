// 2014-10-01
#include <cstdio>
const int MOD = 1000000003;
int main() {
    int f[1000001]; f[0] = 0;
    int sum[1000001]; sum[0] = 0;
    for (int i = 1; i <= 1000000; i++) {
        long long cube = i; cube *= i; cube *= i;
        cube %= MOD;
        f[i] = f[i-1] + cube;
        if (f[i] >= MOD) f[i] -= MOD;
        sum[i] = sum[i-1] + f[i];
        if (sum[i] >= MOD) sum[i] -= MOD;
    }
    int T; scanf("%d", &T);
    while (T--) {
        int n; scanf("%d", &n);
        printf("%d\n", sum[n]);
    }
    return 0;
}
