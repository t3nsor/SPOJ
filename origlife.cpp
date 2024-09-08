// 2024-09-08
// The concept behind this solution is simple. The bulk of the time is spent in
// the step where the evolution of every possible grid is computed (step 1
// below), so it is crucial to optimize the evolution as much as possible. We
// use bitmasks to remove as many branches as possible from the evolution loops.
//
// This solution is a further optimized version of an older one that already
// used bitmasks (see commit 0a9f654), which used an __int128 to represent the
// live neighbour counts for all cells (allocating 4 bits to each cell). When a
// When a cell is live, its effect was to add a certain value to this number
// (i.e. the sum of 2^(4p) for each neighbour p), and we precompute this value
// for each cell. Note that `do_tc` was made a template so that the inner loops
// (using `j`) run a constant number of times and can be unrolled with no
// branches.
//
// This version has a few additional optimizations:
// * We use BFS to avoid exploring the entire graph (effect is small)
// * All remaining branches have been eliminated from step 1 by representing the
//   rule as another bitmask
// * Number of operations in the inner loops have been reduced. One improvement
//   is to use 5 bits per count and always have it represent 2 times the live
//   neighbour count. Then we can just add 0 or 1 (for the liveness of the cell
//   itself) and do the lookup in the rule bitmask.
// * `__int128` is no longer used. Once we ensure that no neighbour count
//   straddles the boundary between the upper and lower qwords, the latter are
//   totally independent from each other but the compiler is not smart enough to
//   figure this out, so we directly operate on the two qwords separately.
#include <ctype.h>
#include <stdio.h>
#include <vector>
using namespace std;
constexpr int dr[8]={0,0,1,1,1,-1,-1,-1};
constexpr int dc[8]={1,-1,1,0,-1,1,0,-1};
template <int m, int n>
void do_tc(int a, int b, int c) {
    // step 0: precompute all neighbours and transitions
    unsigned long long nh[20] = {0};
    unsigned long long nl[20] = {0};
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < 8; k++) {
                const int i2 = i + dr[k];
                const int j2 = j + dc[k];
                if (i2 >= 0 && i2 < m && j2 >= 0 && j2 < n) {
                    const int cell = i2*n + j2;
                    if (cell < 10) {
                        nl[i*n + j] |= 2ull << 5 * cell;
                    } else {
                        nh[i*n + j] |= 2ull << 5 * (cell - 10);
                    }
                }
            }
        }
    }
    unsigned rule = 0;
    for (int i = 0; i <= 8; i++) {
        if (i >= a && i <= b) {
            rule |= 1u << (2*i + 1);
        }
        if (i > c) {
            rule |= 1u << (2*i);
        }
    }
    // step 1: find all evolutions and gardens of Eden
    vector<int> succ(1 << (m * n));
    vector<int> hasparent(1 << (m * n));
    vector<int> dist(1 << (m * n));
    const int INF = 1e9;
    for (int i = 0; i < (1 << (m * n)); i++) {
        unsigned long long cntl = 0, cnth = 0;
        unsigned long long b = i;
        for (int j = 0; j < m * n; j++) {
            unsigned long long mask = -(b & 1);
            cnth += mask & nh[j];
            cntl += mask & nl[j];
            b >>= 1;
        }
        b = i;
        if /* constexpr */ (m * n > 10) {
            for (int j = 0; j < 10; j++) {
                const int thiscnt = cntl & 31;
                cntl >>= 5;
                const int alive = b & 1;
                b >>= 1;
                succ[i] = (succ[i] << 1) + ((rule >> (thiscnt + alive)) & 1);
            }
            for (int j = 10; j < m * n; j++) {
                const int thiscnt = cnth & 31;
                cnth >>= 5;
                const int alive = b & 1;
                b >>= 1;
                succ[i] = (succ[i] << 1) + ((rule >> (thiscnt + alive)) & 1);
            }
        } else {
            for (int j = 0; j < m * n; j++) {
                const int thiscnt = cntl & 31;
                cntl >>= 5;
                const int alive = b & 1;
                b >>= 1;
                succ[i] = (succ[i] << 1) + ((rule >> (thiscnt + alive)) & 1);
            }
        }
        hasparent[succ[i]] = true;
        dist[succ[i]] = INF;
    }
    int board = 0;
    for (int i = 0; i < m * n; i++) {
        int cell;
        do {
            cell = getchar();
        } while (isspace(cell));
        if (cell == '*') board |= 1 << i;
    }
    if (!hasparent[board]) {
        puts("0"); return;
    }
    // step 2: BFS
    constexpr int qsize = 1.1e6;
    vector<int> Q(qsize);
    int qhead = 0, qtail = 0;
    for (int i = 0; i < (1 << (m * n)); i++) {
        if (!hasparent[i]) Q[qtail++] = i;
    }
    while (qhead < qtail) {
        const int b = Q[qhead++];
        const int sb = succ[b];
        if (dist[sb] == INF) {
            dist[sb] = dist[b] + 1;
            if (sb == board) {
                printf("%d\n", dist[sb]);
                return;
            }
            Q[qtail++] = sb;
        }
    }
    puts("-1");
}
using FP = void (*)(int, int, int);
FP fp[4][5] = {
  {do_tc<1, 1>, do_tc<1, 2>, do_tc<1, 3>, do_tc<1, 4>, do_tc<1, 5>},
  {do_tc<2, 1>, do_tc<2, 2>, do_tc<2, 3>, do_tc<2, 4>, do_tc<2, 5>},
  {do_tc<3, 1>, do_tc<3, 2>, do_tc<3, 3>, do_tc<3, 4>, do_tc<3, 5>},
  {do_tc<4, 1>, do_tc<4, 2>, do_tc<4, 3>, do_tc<4, 4>, do_tc<4, 5>}
};
int main() {
    int t; scanf("%d", &t);
    while (t--) {
        int m, n, a, b, c; scanf("%d %d %d %d %d", &m, &n, &a, &b, &c);
        fp[m - 1][n - 1](a, b, c);
    }
}
