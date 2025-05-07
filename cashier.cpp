// 2025-05-06
// We can use a segment tree after compressing the wages in the input. To handle
// wage changes, we keep an `offset` variable such that each employee's actual
// wage is the sum of the "stored" wage and the current offset. When a new
// employee is hired at wage W, the value that should be "stored" in the tree is
// then W - offset. We can probably speed this up by using lazy propagation
// when a number of lowest-paid employees leave the company, but this
// optimization isn't necessary.
#include <algorithm>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
enum Kind {
    HIRE,
    RAISE,
    QUERY,
};
struct Command {
    Kind kind;
    int val;
};
int zero_prefix(vector<int>& tree, int node, int l, int r, int idx) {
    if (l >= idx) return 0;
    if (r == l + 1) {
        return exchange(tree[node], 0);
    }
    const int m = (l + r + 1) / 2;
    const int result = zero_prefix(tree, 2*node, l, m, idx) +
                       zero_prefix(tree, 2*node + 1, m, r, idx);
    tree[node] = tree[2*node] + tree[2*node + 1];
    return result;
}
void do_testcase() {
    int M, mw; scanf("%d %d", &M, &mw);
    vector<Command> commands;
    vector<int> wageVals;
    int offset = 0;
    for (int i = 0; i < M; i++) {
        Command cmd; char c; scanf(" %c %d", &c, &cmd.val);
        if (c == 'I') {
            cmd.kind = HIRE;
            if (cmd.val >= mw) {
                wageVals.push_back(cmd.val - offset);
            }
        } else if (c == 'A') {
            cmd.kind = RAISE;
            offset += cmd.val;
        } else if (c == 'S') {
            cmd.kind = RAISE;
            cmd.val *= -1;
            offset += cmd.val;
        } else {
            cmd.kind = QUERY;
        }
        commands.push_back(cmd);
    }
    offset = 0;
    sort(wageVals.begin(), wageVals.end());
    wageVals.erase(unique(wageVals.begin(), wageVals.end()), wageVals.end());
    const int N = wageVals.size();
    vector<int> tree(4*N);
    int totalQuit = 0;
    int numWorkers = 0;
    for (Command cmd : commands) {
        if (cmd.kind == HIRE) {
            if (cmd.val < mw) continue;
            ++numWorkers;
            const int idx = lower_bound(wageVals.begin(), wageVals.end(),
                                        cmd.val - offset) - wageVals.begin();
            for (int l = 0, r = N, node = 1;;) {
                tree[node]++;
                if (r == l + 1) break;
                const int m = (l + r + 1) / 2;
                if (idx < m) {
                    node = 2*node;
                    r = m;
                } else {
                    node = 2*node + 1;
                    l = m;
                }
            }
        } else if (cmd.kind == RAISE) {
            offset += cmd.val;
            if (cmd.val >= 0) continue;
            // The end of the removed range is the smallest `idx` such that
            // `wageVals[idx] + offset` is at least `mw`, or in other words
            // `wageVals[idx] >= mw - offset`
            const int idx = lower_bound(wageVals.begin(), wageVals.end(),
                                        mw - offset) - wageVals.begin();
            const int numQuit = zero_prefix(tree, 1, 0, N, idx);
            totalQuit += numQuit;
            numWorkers -= numQuit;
        } else {
            if (cmd.val > numWorkers) {
                puts("-1");
                continue;
            }
            int k = numWorkers - cmd.val;
            for (int l = 0, r = N, node = 1;;) {
                if (r == l + 1) {
                    printf("%d\n", wageVals[l] + offset);
                    break;
                }
                const int m = (l + r + 1) / 2;
                if (k < tree[2*node]) {
                    r = m;
                    node = 2*node;
                } else {
                    k -= tree[2*node];
                    l = m;
                    node = 2*node + 1;
                }
            }
        }
    }
    printf("%d\n", totalQuit);
}
int main() {
    int T; scanf("%d", &T);
    while (T--) do_testcase();
}
