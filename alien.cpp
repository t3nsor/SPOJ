// 2014-09-23
#include <cstdio>
int main() {
    int T; scanf("%d", &T);
    int p[100000];
    while (T--) {
        int A, B;
        scanf("%d %d", &A, &B);
        for (int i = 0; i < A; i++) {
            scanf("%d", p+i);
        }
        int l = 0, r = 0, sum = 0;
        int bests = -1, bestp;
        for (;;) {
            if (sum <= B) {
                if (r - l > bests || r - l == bests && sum < bestp) {
                    bests = r -l; bestp = sum;
                }
                if (r == A) break;
                else sum += p[r++];
            } else {
                sum -= p[l++];
            }
        }
        printf("%d %d\n", bestp, bests);
    }
}
