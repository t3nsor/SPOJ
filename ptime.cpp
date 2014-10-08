// 2014-09-26
#include <cstdio>
bool isprime(int x) {
    for (int i = 2; i*i <= x; i++) {
        if (x%i == 0) return false;
    }
    return true;
}
int main() {
    int N; scanf("%d", &N);
    bool first = true;
    for (int i = 2; i <= N; i++) {
        if (!isprime(i)) continue;
        int exp = 0, n = N/i;
        while (n) {
            exp += n;
            n /= i;
        }
        if (!first) printf(" * ");
        first = false;
        printf("%d^%d", i, exp);
    }
    putchar('\n');
    return 0;
}
