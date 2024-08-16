// 2024-08-12
#include <stdio.h>
#include <string.h>
char buf[33333];
void do_testcase() {
    fgets(buf, sizeof(buf), stdin);
    const int N = strlen(buf) - 1;
    buf[N] = 0;
    for (int i = 0; i < N; i += 2) {
        if (i == N - 1 || buf[i] == '0') {
            break;
        }
        if (buf[i + 1] == '1') {
            for (int j = i + 1; j < N; j++) {
                buf[j] = '0' + '1' - buf[j];
            }
            break;
        }
        buf[i + 1] = '1';
    }
    puts(buf);
}
int main() {
    int T; scanf("%d", &T);
    fgets(buf, sizeof(buf), stdin);
    while (T--) do_testcase();
}
