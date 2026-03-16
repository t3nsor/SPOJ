// 2026-03-15
// This problem isn't that hard, and normally I wouldn't even write an
// explanation for it, but for the fact that it has only 102 accepted users.
//
// First, there can be koopa families that aren't related to king koopa.  We
// should always beat all members of those families (e.g., starting from the
// root and ending with the leaves).  That's the easy part (but the problem
// statement tries to avoid calling your attention to it).
//
// For the main tree rooted at king koopa we have a tree recursion problem.  In
// an optimal solution, two of king koopa's immediate children are beaten
// (unless he has fewer than two) and it must be the case that all descendants
// of the one beaten first are also beaten.  The second child that's beaten
// forces us to beat king koopa immediately, at which point the game ends.  All
// other children cannot be beaten.  So our recursion has to compute three
// values for each node: the total score of its subtree, the maximum score we
// can get if we beat this node at the end, and the maximum score we can get if
// we aren't allowed to beat this node.  The rest is left as an exercise for the
// reader.
#include <algorithm>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
struct Top2 {
    int v1 = -1;
    int i1;
    int v2 = -1;
    int i2;
    void update(int v, int i) {
        if (v > v1) {
            v2 = v1;
            i2 = i1;
            v1 = v;
            i1 = i;
            return;
        }
        if (v > v2) {
            v2 = v;
            i2 = i;
        }
    }
};
struct Result {
    int total;
    int use;
    int nouse;
};
void dfs(const vector<vector<int>>& children,
         const vector<int>& t,
         vector<Result>& r,
         int u) {
    r[u].total = t[u];
    r[u].use = t[u];
    r[u].nouse = 0;
    Top2 d1, d2;
    for (const auto v : children[u]) {
        dfs(children, t, r, v);
        r[u].total += r[v].total;
        r[u].use += r[v].nouse;
        r[u].nouse += r[v].nouse;
        d1.update(r[v].total - r[v].nouse, v);
        d2.update(r[v].use - r[v].nouse, v);
    }
    if (children[u].size() >= 1) r[u].nouse += d1.v1;
    if (children[u].size() <= 1) {
        r[u].use = r[u].total;
    } else if (d1.i1 != d2.i1) {
        r[u].use += d1.v1 + d2.v1;
    } else {
        r[u].use += max(d1.v1 + d2.v2, d1.v2 + d2.v1);
    }
}
void do_testcase(int N) {
    vector<int> t(N), p(N);
    vector<vector<int>> children(N);
    for (int i = 0; i < N; i++) scanf("%d", &t[i]);
    for (int i = 0; i < N; i++) {
        scanf("%d", &p[i]);
        if (p[i] >= 0) children[p[i]].push_back(i);
    }
    vector<Result> r(N);
    int result = 0;
    for (int i = 0; i < N; i++) {
        if (p[i] == -1) {
            dfs(children, t, r, i);
            if (i == N - 1) result += r[i].use; else result += r[i].total;
        }
    }
    printf("%d\n", result);
}
int main() {
    for (;;) {
        int N; scanf("%d", &N);
        if (N == 0) break;
        do_testcase(N);
    }
}
