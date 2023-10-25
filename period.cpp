// 2023-10-24
#include <stdio.h>
char s[1000000];
int p[1000001];
void do_testcase() {
    int N; scanf("%d", &N);
    int c;
    do {
        c = getchar_unlocked();
    } while (c <= 32);
    int i = 0;
    do {
        s[i++] = c;
        c = getchar_unlocked();
    } while (c > 32);
    p[0] = -1;
    for (int i = 1; i <= N; i++) {
        int x = p[i - 1];
        p[i] = 0;
        while (x >= 0) {
            if (s[i - 1] == s[x]) {
                p[i] = x + 1;
                break;
            } else {
                x = p[x];
            }
        }
        if (p[i] > 0 && (i % (i - p[i])) == 0) {
            printf("%d %d\n", i, i / (i - p[i]));
        }
    }
}
int main() {
    int T; scanf("%d", &T);
    for (int i = 1; i <= T; i++) {
        printf("Test case #%d\n", i);
        do_testcase();
        putchar('\n');
    }
}
