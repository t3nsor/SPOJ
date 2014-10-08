// 2014-05-01
#include <cstdio>
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        double n; scanf("%lf", &n);
        printf("%.11f\n", 0.5/(n+2) - 0.5/(n+1) + 0.75);
    }
}
