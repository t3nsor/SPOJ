// 2026-09-30
// goddamnit i am so sick of this genre of problem
#include <algorithm>
#include <stdio.h>
#include <utility>
using namespace std;
const int MAX = 10000;
const int MOD = 5000000;
int BIT[51][MAX];
int S[MAX];
pair<int, int> a[MAX];
int last[MAX];
int adjust[51][MAX];
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
    for (int i = 0; i < N; i++) {
        scanf("%d", &S[i]);
        a[i] = make_pair(S[i], i);
    }
    // coordinate compress...
    sort(a, a + N);
    int cur = 0;
    for (int i = 0; i < N; i++) {
        if (i == 0 || a[i].first != a[i - 1].first) cur++;
        S[a[i].second] = cur - 1;
    }
    fill(last, last + MAX, -1);
    for (int i = 0; i < N; i++) {
        if (last[S[i]] == -1) update(BIT[1], S[i], 1);
        for (int j = 2; j <= K; j++) {
            const int q = query(BIT[j - 1], S[i] - 1);
            update(BIT[j], S[i], (q + MOD - adjust[j][S[i]]) % MOD);
            adjust[j][S[i]] = q;
        }
        last[S[i]] = i;
    }
    printf("%d\n", query(BIT[K], MAX - 1));
    return 0;
}
