// 2024-05-27
#include <stdio.h>
void get(char* buf, int& len) {
    char c;
    do {
        c = getchar_unlocked();
    } while (c <= 32);
    do {
        buf[len++] = c;
        c = getchar_unlocked();
    } while (c > 32);
}
void do_testcase() {
    static char X[510000], Y[510000];
    int Xlen = 0, Ylen = 0;
    get(X, Xlen);
    get(Y, Ylen);
    int l = 0, r = Ylen / Xlen;
    while (r > l) {
        const int m = (l + r + 1) / 2;
        bool fail = false;
        int i = 0;
        for (int j = 0; j < Xlen; j++) {
            for (int k = 0; k < m; k++) {
                while (i < Ylen && Y[i] != X[j]) ++i;
                if (i == Ylen) {
                    fail = true;
                    goto end;
                }
                ++i;
            }
        }
end:
        if (fail) {
            r = m - 1;
        } else {
            l = m;
        }
    }
    printf("%d\n", l);
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
