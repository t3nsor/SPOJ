// 2014-10-02
#include <cstdio>
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        double N; scanf("%lf", &N);
        printf("%.8f\n", (N-1)/N + 1/(N*(N+1)));
    }
    return 0;
}
