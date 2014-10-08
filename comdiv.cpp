// 2014-04-30
#include <cstdio>
using namespace std;
bool prime[1000];
int gcd(int x, int y) {
    if (x == 0) return y;
    return gcd(y%x, x);
}
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        int a, b; scanf("%d %d", &a, &b);
        int g = gcd(a, b);
        int res = 1, p = 2;
        while (g > 1) {
            while (p < 1000 && g % p) p++;
            if (p == 1000) p = g;
            int pwr = 0;
            while (g % p == 0) {
                g /= p; ++pwr;
            }
            res *= pwr+1;
        }
        printf("%d\n", res);
    }
}
