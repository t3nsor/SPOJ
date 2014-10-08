// 2014-10-07
#include <cstdio>
#include <cstring>
int main() {
    int c; scanf("%d\n", &c);
    while (c--) {
        char s[1111];
        gets(s);
        int l = strlen(s);
        int cnt[256] = {0};
        for (int i = 0; i < l; i++) {
            cnt[s[i]]++;
        }
        int best = 0;
        for (char c = 'A'; c <= 'Z'; c++) {
            if (cnt[c] > best) {
                best = cnt[c];
            }
        }
        bool fail = false;
        char bestchar = 0;
        for (char c = 'A'; c <= 'Z'; c++) {
            if (cnt[c] == best) {
                if (bestchar == 0) {
                    bestchar = c;
                } else {
                    fail = true;
                }
            }
        }
        if (fail) {
            puts("NOT POSSIBLE");
            continue;
        }
        int shift = (bestchar - 'E' + 26)%26;
        for (int i = 0; i < l; i++) {
            if (s[i] != ' ') {
                s[i] = 'A' + (s[i] - 'A' - shift + 26)%26;
            }
        }
        printf("%d %s\n", shift, s);
    }
    return 0;
}
