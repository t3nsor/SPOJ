// 2014-06-07
#include <cstdio>
// compute ceil(log(x)/log(2))
int log2(int x) {
    int pwr = 1;
    int res = 0;
    while (pwr < x) {
        pwr <<= 1;
        res++;
    }
    return res;
}
int main() {
    int T; scanf("%d", &T);
    for (int _cs = 1; _cs <= T; _cs++) {
        int N, M, K;
        scanf("%d %d %d", &N, &M, &K);
        long long volume = N*M; volume *= K;
        printf("Case #%d: %lld %d\n", _cs, volume-1,
               log2(N) + log2(M) + log2(K));
    }
}
