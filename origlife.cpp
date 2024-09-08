// 2024-09-07
// The concept behind this solution is simple. The bulk of the time is spent in
// the step where the evolution of every possible grid is computed (step 1
// below), so it is crucial to optimize the evolution as much as possible. We
// use bitmasks to remove as many branches as possible from the evolution loops.
// The idea is to use a number with 4mn bits that uses 4 of the bits to
// represent the live neighbour count of each cell. When a cell is live, its
// effect is to add a certain value to this number (i.e. the sum of 2^(4p) for
// each neighbour p), and we precompute this value for each cell. Note that
// `do_tc` was made a template so that the inner loops (using `j`) run a
// constant number of times and can be unrolled with no branches.
#include <ctype.h>
#include <stdio.h>
#include <vector>
using namespace std;
constexpr int dr[8]={0,0,1,1,1,-1,-1,-1};
constexpr int dc[8]={1,-1,1,0,-1,1,0,-1};
template <int m, int n>
void do_tc(int a, int b, int c) {
    // step 0: precompute all neighbours
    __int128 neigh[20] = {0};
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < 8; k++) {
                const int i2 = i + dr[k];
                const int j2 = j + dc[k];
                if (i2 >= 0 && i2 < m && j2 >= 0 && j2 < n) {
                    neigh[i*n + j] |= __int128(1) << (4 * (i2*n + j2));
                }
            }
        }
    }
    // step 1: find all evolutions and gardens of Eden
    vector<int> succ(1 << (m * n));
    vector<int> hasparent(1 << (m * n));
    const int INF = 1e9;
    vector<int> best(1 << (m * n), INF);
    for (int i = 0; i < (1 << (m * n)); i++) {
        succ[i] = 0;
        __int128 cnt = 0;
        for (int j = 0; j < m * n; j++) {
            cnt += ((i & (1 << j)) >> j) * neigh[j];
        }
        for (int j = 0; j < m * n; j++) {
            const int thiscnt = (cnt & (__int128(15) << (4 * j))) >> (4 * j);
            succ[i] |= (1 << j) *
                       ((i & (1 << j)) && (thiscnt >= a && thiscnt <= b) ||
                        !(i & (1 << j)) && thiscnt > c);
        }
        hasparent[succ[i]] = true;
    }
    // step 2: compute all answers
    for (int i = 0; i < (1 << (m * n)); i++) {
        if (hasparent[i]) continue;
        int gen = 0;
        int board = i;
        while (gen < best[board]) {
            best[board] = gen++;
            board = succ[board];
        }
    }
    int board = 0;
    for (int i = 0; i < m * n; i++) {
        int cell;
        do {
            cell = getchar();
        } while (isspace(cell));
        if (cell == '*') board |= 1 << i;
    }
    printf("%d\n", best[board] == INF ? -1 : best[board]);
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
