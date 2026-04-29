// 2026-04-29
// The fact that the fastest solution on SPOJ has running time 0.24s is a hint
// that the algorithm for this problem is a bit slow, probably something like
// O(N^4).  Here's how we do it.
//
// First, we should ask whether we can always choose each row greedily, that
// is, by picking the lexicographically minimal row N - M times we never need
// to backtrack.  The answer is yes; in fact, any valid choice for a row always
// makes it possible to fill in the remaining rows.  To see this, observe that
// adding a row is equivalent to finding a perfect matching in a bipartite
// graph where the N nodes on the left represent entries in this row and the N
// nodes on the right represent the possible values 1 ... N for those entries.
// The previously-filled K rows remove K possible edges for each left node,
// leaving each left node with degree N - K, and because each value appears K
// times among those rows, each right node similarly has degree N - K.
// Assuming N - K > 0, this graph always has a perfect matching according to
// Hall's marriage theorem.
//
// So, our algorithm will repeatedly just fill in the next row with the
// lexicographically smallest possibility.  For a given row, we must find a
// "lexicographically smallest perfect matching", where one matching is less
// than another if the lowest-numbered left vertex whose partner differs between
// the two matchings has a lower-numbered partner in the former than in the
// latter.  Our objective is to do this in O(N^3) time so that the overall
// algorithm will be O(N^4).
//
// Given any perfect matching, we can modify it into a lexicographically
// smallest perfect matching as follows.  For each vertex u on the left side (in
// increasing order) let S be the set of vertices on the right side that are
// lower than u's current partner.  The objective is to find the lowest vertex
// in S (if any) such that a perfect matching exists if that vertex is made u's
// partner.  A vertex v can be made u's partner if we can find an augmenting
// cycle that starts at u, then alternates between taking the edge from the
// current (left) node to its partner (representing a node to which a new
// partner must be assigned) and taking an unused edge from the current (right)
// node back to the left (representing the choice of new partner for that node),
// before finally arriving at v (assuming, of course, that the edge u-v is
// actually in the graph).  For a given v, this condition can be tested on
// O(E+V) time; in fact, a single DFS suffices to test it for all nodes in S,
// since the DFS can find all nodes on the right that have an edge to u and that
// are reachable by an alternating path as described above.  After the
// lowest-numbered partner is found for u, both nodes are removed from the graph
// before proceeding to find the lowest-numbered partner for the lowest
// remaining left node, and so on.  Each matching will be found in O(V(E+V))
// time, i.e., O(N^3), as required.
//
// We can get AC using adjacency matrices, but it's a bit slower (1.95s,
// compared with this solution, which is 0.56s).
//
// Note that the original version of this problem accepted any valid set of
// remaining rows and did not require the lexicographically smallest:
// https://ipsc.ksp.sk/2003/real/problems/a.html

#include <algorithm>
#include <iostream>
#include <queue>
#include <string.h>
#include <vector>
using namespace std;
int adj[220][220];
void remove_value(vector<int>& v, int x) {
    v.erase(find(v.begin(), v.end(), x));
}
vector<int> get_row(const vector<vector<int>>& ladj,
                    const vector<vector<int>>& radj) {
    const int N = ladj.size();
    // find perfect matching
    vector<int> lp(N, -1), rp(N, -1);
    for (int i = 0; i < N; i++) {
        vector<int> pred(N, -1);
        queue<int> Q;
        int r = -1;
        for (const auto u : ladj[i]) { pred[u] = u; Q.push(u); }
        for (;;) {
            const int u = Q.front();
            Q.pop();
            if (rp[u] < 0) { r = u; break; }
            for (const auto v : ladj[rp[u]]) {
                if (v != u && pred[v] < 0) {
                    pred[v] = u;
                    Q.push(v);
                }
            }
        }
        while (pred[r] != r) {
            int l = rp[pred[r]];
            lp[l] = r;
            rp[r] = l;
            r = pred[r];
        }
        lp[i] = r;
        rp[r] = i;
    }
    // improve matching
    for (int i = 0; i < N; i++) {
        vector<int> pred(N, -1);
        queue<int> Q;
        Q.push(lp[i]);
        while (!Q.empty()) {
            const int u = Q.front();
            Q.pop();
            for (const auto v : radj[u]) {
                if (v > i && lp[v] != u && pred[lp[v]] < 0) {
                    pred[lp[v]] = u;
                    Q.push(lp[v]);
                }
            }
        }
        int l = -1, r = lp[i];
        for (const auto u : ladj[i]) {
            if (pred[u] >= 0 && u < r) {
                r = u;
                l = rp[u];
            }
        }
        if (r == lp[i]) continue;
        lp[i] = r;
        while (l != i) {
            const int nextl = rp[pred[r]];
            lp[l] = pred[r];
            rp[pred[r]] = l;
            l = nextl;
            r = pred[r];
        }
        rp[lp[i]] = i;
    }
    return lp;
}
void do_testcase() {
    int N, M; cin >> N >> M;
    vector<vector<int>> ladj(N), radj(N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            ladj[i].push_back(j);
            radj[j].push_back(i);
        }
    }
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            int x; cin >> x; --x;
            remove_value(ladj[j], x);
            remove_value(radj[x], j);
        }
    }
    cout << N - M << '\n';
    for (int i = M; i < N; i++) {
        auto row = get_row(ladj, radj);
        for (int j = 0; j < N; j++) {
            if (j > 0) cout << ' ';
            cout << row[j] + 1;
            remove_value(ladj[j], row[j]);
            remove_value(radj[row[j]], j);
        }
        cout << endl;
    }
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T; while (T--) { do_testcase(); cout << '\n'; }
}
