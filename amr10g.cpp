// 2014-04-25
#include <iostream>
#include <algorithm>
#include <cstdio>
using namespace std;
int a[20000];
int main() {
    int N, K;
    int T; scanf("%d", &T);
    while (T--) {
        scanf("%d %d", &N, &K);
        for (int i = 0; i < N; i++) {
            scanf("%d", a+i);
        }
        sort(a, a + N);
        int best = 2e9;
        for (int i = 0; i <= N - K; i++) {
            best = min(best, a[i + K - 1] - a[i]);
        }
        printf("%d\n", best);
    }
}
