// 2014-05-01
#include <cstdio>
bool islucky(int x) {
    int cnt = 0;
    int p = 2;
    while (x > 1) {
        while (x % p) p++;
        cnt++; if (cnt == 3) return true;
        while (x % p == 0) x /= p;
    }
    return false;
}
int lucky[1000];
int main() {
    int cnt = 0; int x = 1;
    while (cnt < 1000) {
        if (islucky(x)) {
            lucky[cnt++] = x;
        }
        x++;
    }
    int T; scanf("%d", &T);
    while (T--) {
        int n; scanf("%d", &n);
        printf("%d\n", lucky[n-1]);
    }
}
