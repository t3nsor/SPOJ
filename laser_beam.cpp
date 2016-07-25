// 2016-07-24
#include <cstdio>
int gcd(int x, int y) {
    return x == 0 ? y : gcd(y%x, x);
}
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        int x, y;
        scanf("%d %d", &x, &y);
        printf("%d\n", 1 + gcd(x-1, y-1));
    }
}
