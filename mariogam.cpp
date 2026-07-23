// 2026-07-23
// The result of advancing by one time step can be described by applying a
// linear operator A to a vector whose entries give the probability that Mario
// is in a particular state.  A state is a combination of a valid position in
// the grid and the number of lives Mario has.  Let v_0 denote the initial state
// and let w be a vector such that w(i) is the reward for entering state i.
// Then, the expected number of coins collected by Mario is wv_0 + wAv_0 +
// wA^2 v_0 + ... (infinite series) (note that the first term is always 0
// because Mario doesn't start at a coin box, but this doesn't affect the
// analysis).  We can rewrite this as w(v_0 + Av_0 + A^2 v_0 + ...).  We want to
// find the vector v = v_0 + Av_0 + A^2 v_0 + ..., which can be done by using
// the fact that v = v_0 + Av, or in other words (I - A)v = v_0.  This will have
// a unique solution for v if 1 is not an eigenvalue of A, i.e., there is no
// steady-state vector in the given maze.  We start by determining whether the
// expected number of coins is infinite; this will be the case if and only if
// there is a sink component (defined here as a strongly connected component
// that has no outgoing edges) that is reachable, contains no monsters, and
// contains at least one coin box in it with positive value.  Otherwise, for
// every reachable sink component that has no coin boxes with positive value,
// we will change every space in that region to a monster to ensure that Mario
// dies when he enters it; as a result of this preprocessing there will be no
// (nonzero) steady state and we can then find the uniquely determined v using
// Gaussian elimination.
#include <array>
#include <ctype.h>
#include <iostream>
#include <string>
#include <string.h>
#include <utility>
#include <vector>
using namespace std;
constexpr int dr[4] = {1, 0, -1, 0};
constexpr int dc[4] = {0, 1, 0, -1};
int R, C;
string maze[15];
pair<int, int> pdest[15][15];
bool vis[15][15][3][15][15][3];
bool vis2[15][15][3];
vector<vector<array<int, 3>>> scc;
int sccid[15][15][3];
template <class CB>
void visit_neighbors(int r, int c, int l, CB cb) {
    for (int i = 0; i < 4; i++) {
        int r2 = r + dr[i];
        int c2 = c + dc[i];
        if (r2 < 0 || r2 >= R || c2 < 0 || c2 >= C || maze[r2][c2] == '#') {
            continue;
        }
        if (pdest[r2][c2].first != -1) {
            const auto p = pdest[r2][c2];
            r2 = p.first;
            c2 = p.second;
        }
        cb(r2, c2, l - (maze[r2][c2] == '!'));
    }
}
void dfs1(bool (&vis)[15][15][3], int r, int c, int l) {
    if (vis[r][c][l]) return;
    vis[r][c][l] = true;
    visit_neighbors(r, c, l,
                    [&vis] (int r2, int c2, int l2) {
                        if (l2 >= 0) dfs1(vis, r2, c2, l2);
                    });
}
void dfs2(int id, int r, int c, int l) {
    if (sccid[r][c][l] >= 0) return;
    sccid[r][c][l] = id;
    scc.back().push_back({r, c, l});
    visit_neighbors(r, c, l,
                    [id, r, c, l] (int r2, int c2, int l2) {
                        if (l2 >= 0 &&
                            vis[r][c][l][r2][c2][l2] &&
                            vis[r2][c2][l2][r][c][l]) {
                            dfs2(id, r2, c2, l2);
                        }
                    });
}
int main() {
    cin >> R >> C;
    // enumerate portals
    pair<int, int> pexit[26];
    for (int i = 0; i < 26; i++) pexit[i] = make_pair(-1, -1);
    for (int i = 0; i < R; i++) {
        cin >> maze[i];
        for (int j = 0; j < C; j++) {
            pdest[i][j] = make_pair(-1, -1);
            if (maze[i][j] >= 'A' && maze[i][j] <= 'Z') {
                pexit[maze[i][j] - 'A'] = make_pair(i, j);
            }
        }
    }
    int initr, initc;
    // create mapping from portal entrances to exits
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            if (maze[i][j] == '$') {
                initr = i;
                initc = j;
            } else if (maze[i][j] >= 'a' && maze[i][j] <= 'z') {
                pdest[i][j] = pexit[maze[i][j] - 'a'];
            }
        }
    }
    // find states reachable from initial state
    dfs1(vis[initr][initc][2], initr, initc, 2);
    // find states reachable from those states
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            for (int l = 0; l <= 2; l++) {
                if (vis[initr][initc][2][i][j][l]) {
                    dfs1(vis[i][j][l], i, j, l);
                }
            }
        }
    }
    // find strongly connected components
    memset(sccid, -1, sizeof(sccid));
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            for (int l = 0; l <= 2; l++) {
                if (!vis[initr][initc][2][i][j][l]) continue;
                if (sccid[i][j][l] >= 0) continue;
                scc.emplace_back();
                dfs2(scc.size() - 1, i, j, l);
            }
        }
    }
    // check for unbounded cases
    bool die[15][15][3] = {0};
    for (const auto& comp : scc) {
        bool have_monster = false;
        bool have_coins = false;
        bool have_outgoing = false;
        for (const auto& a : comp) {
            const int r = a[0];
            const int c = a[1];
            const int l = a[2];
            if (maze[r][c] == '!') have_monster = true;
            else if (maze[r][c] >= '1' && maze[r][c] <= '9') have_coins = true;
            visit_neighbors(r, c, l,
                            [&] (int r2, int c2, int l2) {
                                if (l2 < 0 ||
                                    sccid[r2][c2][l2] != sccid[r][c][l]) {
                                    have_outgoing = true;
                                }
                            });
        }
        if (have_outgoing) continue;
        if (!have_monster) {
            if (have_coins) { puts("-1"); return 0; }
            for (const auto& a : comp) die[a[0]][a[1]][a[2]] = true;
        }
    }

    // check for special case where Mario can't move ever (unnecessary, see
    // below)
    // if (scc.size() == 1 && scc[0].size() == 1) { puts("0"); return 0; }

    // assign unique indices
    vector<array<int, 3>> id_to_pos;
    int pos_to_id[15][15][3];
    for (const auto& comp : scc) {
        for (const auto& a : comp) {
            const int r = a[0];
            const int c = a[1];
            const int l = a[2];
            if ((maze[r][c] == '$' && l == 2) || !die[r][c][l]) {
                pos_to_id[a[0]][a[1]][a[2]] = id_to_pos.size();
                id_to_pos.push_back(a);
            }
        }
    }
    const int N = id_to_pos.size();
    const int init_id = pos_to_id[initr][initc][2];
    // build matrix
    long double A[675][676] = {0};
    for (int i = 0; i < N; i++) A[i][i] = 1;
    for (int i = 0; i < N; i++) {
        int nnbr = 0;
        int nbr[4];
        visit_neighbors(id_to_pos[i][0], id_to_pos[i][1], id_to_pos[i][2],
                        [&](int r2, int c2, int l2) {
                            if (l2 < 0 || die[r2][c2][l2]) {
                                nbr[nnbr++] = -1;
                            } else {
                                nbr[nnbr++] = pos_to_id[r2][c2][l2];
                            }
                        });
        // if there are no neighbors, that means we are stuck on the initial
        // square and it's correct to do nothing (this is equivalent to A
        // annihilating the vector)
        for (int j = 0; j < nnbr; j++) {
            if (nbr[j] >= 0) A[nbr[j]][i] -= 1.0/nnbr;
        }
    }
    A[init_id][N] = 1;
    // finally run Gaussian elimination
    for (int i = 0; i < N; i++) {
        int row = i;
        while (A[row][i] == 0) row++;
        if (row != i) for (int j = i; j <= N; j++) swap(A[i][j], A[row][j]);
        for (int j = i + 1; j <= N; j++) A[i][j] /= A[i][i];
        A[i][i] = 1;
        for (int j = 0; j < N; j++) {
            if (j == i) continue;
            long double coeff = -A[j][i];
            for (int k = i; k <= N; k++) A[j][k] += coeff*A[i][k];
        }
    }
    long double result = 0;
    for (int i = 0; i < N; i++) {
        const int r = id_to_pos[i][0];
        const int c = id_to_pos[i][1];
        if (isdigit(maze[r][c])) result += (maze[r][c] - '0') * A[i][N];
    }
    printf("%.8Lf\n", result);
}
