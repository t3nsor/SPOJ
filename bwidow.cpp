// 2014-10-02
#include <cstdio>
int r[1000];
int R[1000];
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        int N; scanf("%d", &N);
        int biggest_R = -1;
        int biggest_pos;
        for (int i = 0; i < N; i++) {
            scanf("%d %d", r+i, R+i);
            if (R[i] > biggest_R) {
                biggest_R = R[i];
                biggest_pos = i;
            }
        }
        bool ok = true;
        for (int i = 0; i < N; i++) {
            if (i == biggest_pos) continue;
            if (R[i] >= r[biggest_pos]) {
                ok = false;
                break;
            }
        }
        if (ok) {
            printf("%d\n", biggest_pos+1);
        } else {
            puts("-1");
        }
    }
    return 0;
}
