// 2014-10-02
#include <cstdio>
const int MAX = 100000;
const int MOD = 5000000;
int BIT[51][MAX];
int query(int* b, int idx) {
    int res = 0;
    while (idx >= 0) {
        res += b[idx];
        if (res >= MOD) res -= MOD;
        idx = (idx&(idx+1))-1;
    }
    return res;
}
void update(int* b, int idx, int val) {
    while (idx < MAX) {
        b[idx] += val;
        if (b[idx] >= MOD) b[idx] -= MOD;
        idx |= idx+1;
    }
}
int main() {
    int N, K; scanf("%d %d", &N, &K);
    while (N--) {
        int x; scanf("%d", &x);
        update(BIT[1], x, 1);
        for (int i = 2; i <= K; i++) {
            update(BIT[i], x, query(BIT[i-1], x-1));
        }
    }
    printf("%d\n", query(BIT[K], 99999));
    return 0;
}
