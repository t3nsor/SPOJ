// 2026-07-19
#include <stdio.h>
using LLL = __int128;
void print(LLL x) {
    char buf[50];
    int len = 0;
    do {
        buf[len++] = (x % 10) + '0';
        x /= 10;
    } while (x > 0);
    while (len--) {
        putchar(buf[len]);
    }
}
constexpr LLL mask64 = (LLL(1) << 64) - 1;
int ctz(LLL x) {
    if (x == 0) return -1;
    else if ((x & mask64) != 0) return __builtin_ctzll(x & mask64);
    else return 64 + __builtin_ctzll(x >> 64);
}
int popcnt(LLL x) {
    return __builtin_popcountll(x >> 64) + __builtin_popcountll(x & mask64);
}
bool can(int L, int chunks, LLL mx) {
    LLL next = 0;
    while (chunks--) {
        LLL used = 0;
        int pos;
        for (;;) {
            pos = ctz(next);
            if (pos == -1) pos = L;
            const LLL incr = LLL(1) << pos;
            const int prefixcnt = popcnt(next >> pos);
            LLL nextused = used + incr*popcnt(next >> pos) + (incr*pos)/2;
            if (nextused > mx) break;
            used = nextused;
            next += incr;
            if (next == (LLL(1) << L)) return true;
        }
        while (pos--) {
            const LLL incr = LLL(1) << pos;
            const int prefixcnt = popcnt(next >> pos);
            LLL nextused = used + incr*popcnt(next >> pos) + (incr*pos)/2;
            if (nextused <= mx) {
                used = nextused;
                next += incr;
            }
        }
    }
    return false;
}
int main() {
    int K, M; scanf("%d %d", &K, &M);
    LLL l = K, r = K*(LLL(1) << (K - 1));
    while (r > l) {
        const LLL m = (l + r) / 2;
        if (can(K, M, m)) {
            r = m;
        } else {
            l = m + 1;
        }
    }
    print(l); putchar('\n');
}
