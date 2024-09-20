// 2024-09-20
#include <set>
#include <stdio.h>
using namespace std;
void do_testcase() {
    int N, K, P; scanf("%d %d %d", &N, &K, &P);
    set<int> S = {0};
    int psum = 0;
    int best = P;
    for (int i = 0; i < N; i++) {
        int x; scanf("%d", &x);
        psum = (psum + x) % P;
        auto it = S.upper_bound((psum - K + P) % P);
        if (it == S.begin()) {
            it = S.end();
        }
        --it;
        const int rem = (psum - *it + P) % P;
        if (rem >= K && rem < best) best = rem;
        S.insert(psum);
    }
    printf("%d\n", best);
}
int main() {
    int T; scanf("%d", &T);
    while (T--) do_testcase();
}
