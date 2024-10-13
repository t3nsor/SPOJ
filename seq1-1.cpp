// 2024-10-12
// This is the first approach I thought of, although presumably there are other
// ideas that also work, because my solution takes 0.35s while 97% of the other
// AC users got <= 0.03s. The idea is to consider the prefix sums of the
// sequence. If P_n denotes the sum of the first n elements of S, then we know
// 0 <= P_n <= n. We can create 2n variables representing the predicates
// P_n <= 0, P_n <= 1, ..., P_n <= n - 1, and their negations. These variables
// are not independent since P_n <= j implies P_n <= j + 1. The constraints
// P_{n + 1} - P_n \in {0, 1} are translated into additional implications, as
// are the constraints A0, B0, L0, A1, B1, L1. Having built up an implication
// graph, we solve it in the same way as the implication graph from a 2SAT
// problem: if a variable A and its negation ~A are in the same strongly
// connected component, then satisfaction is impossible; otherwise, construct a
// solution. This is an O(n^2) algorithm with a high constant factor since the
// graph can have ~7 million edges in the worst case. It got TLE at first, so I
// had to rewrite it to avoid vectors and use only static arrays, which sped it
// up by a factor of 4.
// (Perhaps an LP solution is also possible?)
#include <algorithm>
#include <iostream>
#include <string.h>
#include <utility>
using namespace std;
constexpr int MAXV = 1001000;
constexpr int MAXDEG = 9;
enum ConstraintKind {
    LESS_EQ,
    GREATER
};
struct Constraint {
    ConstraintKind kind;
    int idx;
    int val;
    bool always_true() const {
        return (kind == LESS_EQ && val >= idx) ||
               (kind == GREATER && val < 0);
    }
    int serialize() const {
        const int start = idx * (idx - 1);
        return start + 2*val + (kind == GREATER);
    }
};
Constraint invert(Constraint c) {
    c.kind = (c.kind == LESS_EQ ? GREATER : LESS_EQ);
    return c;
}
int deg[MAXV];
int adj[MAXDEG * MAXV];
int post[MAXV];
bool vis[MAXV];
int tadj[MAXDEG * MAXV];
int tdeg[MAXV];
int tofs[MAXV];
int sccid[MAXV];
auto& kadj = tadj;
auto& kdeg = tdeg;
auto& kofs = tofs;
void push_back(int v, int val) {
    if (deg[v] >= MAXDEG) throw;
    adj[MAXDEG * v + deg[v]++] = val;
}
void mark_true(Constraint c) {
    if (!c.always_true()) {
        push_back(invert(c).serialize(), c.serialize());
    }
}
void add_edge(Constraint c1, Constraint c2) {
    push_back(c1.serialize(), c2.serialize());
    push_back(invert(c2).serialize(), invert(c1).serialize());
}
void dfs1(int u, int& sz) {
    if (vis[u]) return;
    vis[u] = 1;
    for (int i = 0; i < deg[u]; i++) {
        dfs1(adj[MAXDEG * u + i], sz);
    }
    post[sz++] = u;
}
void dfs2(int nscc, int u) {
    if (sccid[u] >= 0) return;
    sccid[u] = nscc;
    for (int i = 0; i < tdeg[u]; i++) {
        dfs2(nscc, tadj[tofs[u] + i]);
    }
}
void dfs3(int& cur, int u) {
    if (vis[u]) return;
    vis[u] = 1;
    for (int i = 0; i < kdeg[u]; i++) {
        dfs3(cur, kadj[kofs[u] + i]);
    }
    post[u] = cur++;
}
void do_testcase() {
    int N; cin >> N;
    memset(deg, 0, sizeof(int)*N*(N+1));
    for (int i = 1; i <= N; i++) {
        for (int j = 0; j < i - 1; j++) {
            // P_i <= j implies P_i <= j + 1
            add_edge({LESS_EQ, i, j}, {LESS_EQ, i, j + 1});
        }
    }
    for (int i = 1; i < N; i++) {
        for (int j = 0; j < i; j++) {
            // P_i <= j implies P_{i + 1} <= j + 1
            add_edge({LESS_EQ, i, j}, {LESS_EQ, i + 1, j + 1});
            // P_i > j implies P_{i + 1} > j
            add_edge({GREATER, i, j}, {GREATER, i + 1, j});
        }
    }
    int A0, B0, L0, A1, B1, L1; cin >> A0 >> B0 >> L0 >> A1 >> B1 >> L1;
    int tmp = A0;
    A0 = L0 - B0;
    B0 = L0 - tmp;
    mark_true({GREATER, L0, A0 - 1});
    mark_true({LESS_EQ, L0, B0});
    mark_true({GREATER, L1, A1 - 1});
    mark_true({LESS_EQ, L1, B1});
    // A0 <= P_{i + L0} - P_i <= B0
    for (int i = 1; i + L0 <= N; i++) {
        // P_i <= j implies P_{i + L0} <= B0 + j
        for (int j = 0; j < i && B0 + j < i + L0; j++) {
            add_edge({LESS_EQ, i, j}, {LESS_EQ, i + L0, j + B0});
        }
        // P_i > j implies P_{i + L0} > A0 + j
        for (int j = 0; j < i; j++) {
            add_edge({GREATER, i, j}, {GREATER, i + L0, j + A0});
        }
    }
    // ditto
    for (int i = 1; i + L1 <= N; i++) {
        for (int j = 0; j < i && B1 + j < i + L1; j++) {
            add_edge({LESS_EQ, i, j}, {LESS_EQ, i + L1, j + B1});
        }
        for (int j = 0; j < i; j++) {
            add_edge({GREATER, i, j}, {GREATER, i + L1, j + A1});
        }
    }
    // find strongly connected components
    memset(vis, 0, N*(N+1)*sizeof(bool));
    int sz = 0;
    for (int i = 0; i < N*(N+1); i++) {
        dfs1(i, sz);
    }
    memset(tdeg, 0, sizeof(int)*N*(N+1));
    for (int i = 0; i < N*(N+1); i++) {
        for (int j = 0; j < deg[i]; j++) {
            tdeg[adj[MAXDEG * i + j]]++;
        }
    }
    tofs[0] = 0;
    for (int i = 1; i < N*(N+1); i++) {
        tofs[i] = tofs[i - 1] + tdeg[i - 1];
    }
    memset(tdeg, 0, sizeof(int)*N*(N+1));
    for (int i = 0; i < N*(N+1); i++) {
        for (int j = 0; j < deg[i]; j++) {
            const int v = adj[MAXDEG * i + j];
            tadj[tofs[v] + tdeg[v]++] = i;
        }
    }
    memset(sccid, -1, sizeof(int)*N*(N+1));
    int nscc = 0;
    for (int i = N*(N+1) - 1; i >= 0; i--) {
        const int u = post[i];
        if (sccid[u] >= 0) continue;
        dfs2(nscc++, u);
    }
    // build kernel DAG
    memset(kdeg, 0, sizeof(int)*nscc);
    for (int i = 0; i < N*(N+1); i++) {
        for (int j = 0; j < deg[i]; j++) {
            const int s1 = sccid[i];
            const int s2 = sccid[adj[MAXDEG*i + j]];
            kdeg[s1]++;
        }
    }
    kofs[0] = 0;
    for (int i = 1; i < nscc; i++) {
        kofs[i] = kofs[i - 1] + kdeg[i - 1];
    }
    memset(kdeg, 0, sizeof(int)*nscc);
    for (int i = 0; i < N*(N+1); i++) {
        for (int j = 0; j < deg[i]; j++) {
            const int s1 = sccid[i];
            const int s2 = sccid[adj[MAXDEG*i + j]];
            kadj[kofs[s1] + kdeg[s1]++] = s2;
        }
    }
    memset(vis, 0, nscc);
    int cur = 0;
    for (int i = 0; i < nscc; i++) {
        dfs3(cur, i);
    }
    static bool truth[MAXV];
    // assign truth values
    for (int i = 0; i < N*(N+1); i+= 2) {
        const int p1 = post[sccid[i]];
        const int p2 = post[sccid[i + 1]];
        if (p1 == p2) {
            cout << "-1\n";
            return;
        }
        truth[i] = (p1 < p2);
    }
    // construct sequence
    Constraint c;
    c.kind = LESS_EQ;
    c.val = 0;
    for (c.idx = 1; c.idx <= N; c.idx++) {
        if (truth[c.serialize()]) {
            cout << '0';
        } else {
            cout << '1';
            ++c.val;
        }
    }
    cout << '\n';
}
int main() {
    for (int i = 0; i < 5; i++) do_testcase();
}
