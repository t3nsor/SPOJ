// 2025-06-15
#include <stack>
#include <stdio.h>
#include <vector>
using namespace std;
void do_testcase(int N1, int N2, int D) {
    vector<int> in(N1 + N2);
    vector<vector<int>> child(N1 + N2);
    for (int i = 0; i < D; i++) {
        int x, y; scanf("%d %d", &x, &y); --x; --y;
        in[y]++; child[x].push_back(y);
    }
    int result = 1e9;
    for (int sd = 0; sd <= 1; sd++) {
        auto incopy = in;
        stack<int> S[2];
        for (int i = 0; i < N1; i++) {
            if (incopy[i] == 0) S[0].push(i);
        }
        for (int i = N1; i < N1 + N2; i++) {
            if (incopy[i] == 0) S[1].push(i);
        }
        int cur = 2; int d = sd; int rem = N1 + N2;
        while (rem) {
            if (S[d].empty()) {
                ++cur;
                d = !d;
            }
            while (!S[d].empty()) {
                const int u = S[d].top();
                S[d].pop();
                --rem;
                for (const int v : child[u]) {
                    if (0 == --incopy[v]) {
                        if (v < N1) S[0].push(v); else S[1].push(v);
                    }
                }
            }
        }
        result = min(result, cur);
    }
    printf("%d\n", result);
}
int main() {
    for (;;) {
        int N1, N2, D; scanf("%d %d %d", &N1, &N2, &D);
        if (N1 + N2 + D == 0) break;
        do_testcase(N1, N2, D);
    }
}
