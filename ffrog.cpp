// 2026-07-25
// Moore's k-Nim.  We take the binary representation of each pile, but sum the
// bits mod (k + 1) instead of 2.  Note that the row and column coordinates for
// each frog are two independent piles.
#include <stdio.h>
int buf[400000];  // (factor, value) pairs
void do_testcase() {
    int m, n, k; scanf("%d %d %d", &m, &n, &k);
    int cnt = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            long long residue = 0;
            char c;
            do {
                c = getchar_unlocked();
            } while (c <= 32);
            do {
                residue = (10*residue + (c - '0')) % (k + 1);
                c = getchar_unlocked();
            } while (c > 32);
            buf[2*cnt] = residue;
            buf[2*cnt + 1] = i;
            cnt++;
            buf[2*cnt] = residue;
            buf[2*cnt + 1] = j;
            cnt++;
        }
    }
    for (;;) {
        long long acc = 0;
        for (int i = 0; i < cnt; i++) {
            acc += buf[2*i]*(buf[2*i + 1] & 1);
        }
        if (acc == 0) {
            puts("SmartFrog Wins!.");
            return;
        } else if (acc % (k + 1) != 0) {
            puts("SensibleFrog Wins!.");
            return;
        } else {
            for (int i = 0; i < cnt; i++) {
                buf[2*i + 1] >>= 1;
            }
        }
    }
}
int main() {
    int T; scanf("%d", &T);
    while (T--) do_testcase();
}
