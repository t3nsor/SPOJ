// 2026-04-26
// Being able to get from one grid configuration to another using legal moves is
// an equivalence relation, so one obvious approach is to define a canonical
// element for each equivalence class and convert each of the two given grids to
// its canonical form; the answer is "YES" iff the canonical forms match.  An
// obvious choice for canonical form is the lexicographically smallest
// configuration in the equivalence class.  We can actually compute this in
// O(NM^2) time.  Here's how: go through one row at a time, and for both the
// original row and its flipped version, permute the columns so as to move all
// the zeroes to the left.  If this is not the topmost row, then we are
// constrained to column permutations that preserve all blocks belonging to
// previous rows (where a block is a maximal contiguous subarray consisting of
// all ones or all zeroes).  Take whichever of the two resulting permuted rows
// is lexicographically smaller; if they're equal, then recursively try both
// options.  To make this efficient, we must compute each row lazily: when
// permuting the columns in order to minimize a particular row, we don't
// actually manipulate the following rows in the matrix; we just keep track of
// the permutation that must be applied to them once we get to them.  The reason
// why this algorithm runs in O(NM^2) time is that if 2^e is the greatest power
// of 2 that divides M, then we can split only e times, so the total running
// time is actually O(NM 2^e), which is O(NM^2).
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;
vector<vector<vector<int>>> cands;
const vector<vector<int>>& getmin(const vector<vector<int>>& grid,
                                  vector<vector<int>>& cand,
                                  int r,
                                  vector<int> perm,
                                  vector<int> endpoint) {
    const int R = grid.size();
    const int C = grid[0].size();
    if (r == R) return cand;
    vector<int> row1(C), row2(C), perm1 = perm, perm2 = perm;
    for (int i = 0; i < C; i++) {
        row1[i] = grid[r][perm[i]];
        row2[i] = !row1[i];
    }
    int b = 0;
    while (b < C) {
        int e = b + 1;
        while (e < C && !endpoint[e]) ++e;
        int next01 = b, next02 = b;
        for (int j = b; j < e; j++) {
            if (row1[j] == 0) {
                swap(perm1[j], perm1[next01]);
                swap(row1[j], row1[next01]);
                ++next01;
            }
            if (row2[j] == 0) {
                swap(perm2[j], perm2[next02]);
                swap(row2[j], row2[next02]);
                ++next02;
            }
        }
        b = e;
    }
    if (row1 < row2) {
        cand[r] = row1;
        for (int i = 1; i < C; i++) if (row1[i] != row1[i - 1]) endpoint[i] = 1;
        return getmin(grid, cand, r + 1, perm1, endpoint);
    } else if (row2 < row1) {
        cand[r] = row2;
        for (int i = 1; i < C; i++) if (row2[i] != row2[i - 1]) endpoint[i] = 1;
        return getmin(grid, cand, r + 1, perm2, endpoint);
    } else {
        cands.push_back(cands.back());
        auto& cand2 = cands.back();
        cand[r] = row1;
        cand2[r] = row2;
        vector<int> endpoint2 = endpoint;
        for (int i = 1; i < C; i++) {
            if (row1[i] != row1[i - 1]) endpoint[i] = 1;
            if (row2[i] != row2[i - 1]) endpoint2[i] = 1;
        }
        return min(getmin(grid, cand, r + 1, perm1, endpoint),
                   getmin(grid, cand2, r + 1, perm2, endpoint2));
    }
}
void do_testcase() {
    int R, C; cin >> R >> C;
    vector<vector<vector<int>>> grids(2,
                                      vector<vector<int>>(R, vector<int>(C)));
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < R; j++) {
            for (int k = 0; k < C; k++) {
                string word; cin >> word;
                grids[i][j][k] = (word == "BLUE");
            }
        }
        cands.clear();
        cands.push_back(grids[i]);
        vector<int> perm(C);
        for (int j = 0; j < C; j++) perm[j] = j;
        grids[i] = getmin(grids[i], cands[0], 0, perm, vector<int>(C, 0));
    }
    cout << (grids[0] == grids[1] ? "YES" : "NO") << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    cands.reserve(64);
    int T; cin >> T; while (T--) do_testcase();
}
