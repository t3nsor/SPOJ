// 2026-03-14
// A straightforward O(N^4) algorithm is as follows: for every possible pair of
// of values `r1`, `r2`, consider the rectangle that covers rows [r1, r2) and
// slides from initially occupying the first `w` columns where `w` is as large
// as possible without exceeding the maximum allowed area `A`.  Maintain a
// running maximum number of days `d` that the cow can survive in either the
// current window position or any previous window position; when we slide the
// window, do not attempt to decrease `d` since that would be inefficient.
// Instead, when updating the content of the window by removing a column, if any
// removed cells are needed for days other than the last day the cow is known to
// be able to survive, simply update a counter indicating how many such days are
// missing.  When that counter reaches zero, we may be able to increment `d`.
// When the optimal values (days, r1, c1, r2) have been determined at the end of
// the algorithm, try to shrink `c2` as much as possible without reducing the
// number of days.
//
// Unfortunately, I could not figure out how to get that approach to pass the
// time limit, so I gave up and looked at the official solution.  For reasons
// that I don't totally understand, it's much faster to do it this way: starting
// with an empty window, repeatedly try to increment the number of days the cow
// can survive, as follows: extend the window (increment `c2`) until there are
// enough of the drug needed for the next day (if possible), then, while the
// window area is larger than `A`, increment `c1`, but every time we remove a
// cell during this step, if it causes the number of units of the drug to fall
// below the quantity needed by either the current day or any previous day,
// increment `c2` as needed until we once again have enough (if possible).
#include <algorithm>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>
using namespace std;
void do_testcase(int R, int C, int A) {
    vector<int> orig_med; 
    for (;;) {
        int m; cin >> m;
        if (m == -1) break;
        orig_med.push_back(m);
    }

    // compress medication types
    vector<int> uniq_med = orig_med;
    sort(uniq_med.begin(), uniq_med.end());
    uniq_med.erase(unique(uniq_med.begin(), uniq_med.end()), uniq_med.end());
    vector<int> med(orig_med.size());
    for (int i = 0; i < orig_med.size(); i++) {
        med[i] = lower_bound(uniq_med.begin(), uniq_med.end(), orig_med[i])
                 - uniq_med.begin();
    }
    vector<int> cnt(uniq_med.size(), 0);
    vector<int> req(med.size());
    for (int i = 0; i < med.size(); i++) {
        req[i] = ++cnt[med[i]];
    }

    vector<vector<int>> grid(R, vector<int>(C));
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            cin >> grid[i][j];
            const auto it =
              lower_bound(uniq_med.begin(), uniq_med.end(), grid[i][j]);
            if (it == uniq_med.end() || *it != grid[i][j]) {
                grid[i][j] = -1;
            } else {
                grid[i][j] = it - uniq_med.begin();
            }
        }
    }

    int bestr1 = 0, bestr2 = 1, bestc1 = 0, bestc2 = 1, bestd = 0;
    const auto upd = [&](int d, int r1, int c1, int r2, int c2) {
        if (make_tuple(d, -r1, -c1, -r2, -c2) >
            make_tuple(bestd, -bestr1, -bestc1, -bestr2, -bestc2)) {
            tie(bestd, bestr1, bestc1, bestr2, bestc2) =
              make_tuple(d, r1, c1, r2, c2);
        }
    };
    for (int r1 = 0; r1 < R; r1++) {
        for (int r2 = r1 + 1; r2 <= R; r2++) {
            if (r2 - r1 > A) continue;
            const int maxh = A / (r2 - r1);
            vector<int> need(uniq_med.size(), 0);
            int c1 = 0, c2 = 0, d = 0;
            // try to increase `d`
            while (d < med.size()) {
                ++need[med[d]];
                while (need[med[d]] > 0 && c2 < C) {
                    for (int i = r1; i < r2; i++) {
                        if (grid[i][c2] >= 0) --need[grid[i][c2]];
                    }
                    ++c2;
                }
                if (need[med[d]] > 0) break;
                bool fail = false;
                while (c2 - c1 > maxh) {
                    for (int i = r1; i < r2; i++) {
                        if (grid[i][c1] < 0) continue;
                        ++need[grid[i][c1]];
                        while (need[grid[i][c1]] > 0 && c2 < C) {
                            for (int j = r1; j < r2; j++) {
                                if (grid[j][c2] >= 0) --need[grid[j][c2]];
                            }
                            ++c2;
                        }
                        if (need[grid[i][c1]] > 0) {
                            fail = true;
                            break;
                        }
                    }
                    if (fail) break;
                    ++c1;
                }
                if (!fail) {
                    ++d;
                    upd(d, r1, c1, r2, c2);
                } else break;
            }
        }
    }
    cout << bestd << ' ' << bestr1 + 1 << ' ' << bestc1 + 1 << ' '
         << bestr2 << ' ' << bestc2 << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    for (;;) {
        int R, C, A; cin >> R >> C >> A;
        if (R == 0 && C == 0 && A == 0) break;
        do_testcase(R, C, A);
    }
}
