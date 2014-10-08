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
    int t; scanf("%d", &t);
    while (t--) {
        int n; scanf("%d", &n);
        printf("%d\n", 1+log2(n));
    }
}
