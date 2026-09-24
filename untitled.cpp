// 2026-09-23
// Before solving this problem, you should solve CPATTERN or at least understand
// the solution.  Since that problem is solved using KMP and now we have
// multiple patterns, naturally we should use Aho–Corasick (AC), which is the
// generalization of KMP to multiple patterns.  The bookkeeping is tedious, but
// ultimately there is nothing especially clever required.
#include <algorithm>
#include <list>
#include <queue>
#include <stdio.h>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;
using LL = long long;
/// Node in the AC automaton.
struct Node {
    /// The ID of the pattern, and the index into that pattern, corresponding to
    /// the current node.  If this node is shared by multiple patterns then it
    /// doesn't matter which one is recorded here since they're all isomorphic
    /// up to this point.
    int pid;
    int idx;

    int suffix = -1;

    /// A list of pairs (r, i) where `i` identifies a child node and `r` is the
    /// rank of the pattern element represented by that child node relative to
    /// all preceding elements in the pattern.  This will be sorted during the
    /// suffix arc generation step.
    vector<pair<int, int>> children;

    bool term = false;
};
vector<Node> nodes;
// `pred` and `succ` are the same as the `lt` and `gt` tables in cpattern.cpp
vector<vector<int>> pattern, pred, succ;
// During the first phase where the automaton is populated with nodes, we need
// to be able to check whether a child edge with a particular rank exists for a
// given node, and if not then insert one.  Since having one hash map per node
// would probably have terrible performance, we have one global hash map that is
// indexed by the pair (node ID, rank) compressed into a 64-bit integer.
unordered_map<LL, int> child;

/// Insert the pattern with index `pid` into the trie and compute its `pred` and
/// `succ` arrays using the linked list trick.
void insert(int pid) {
    const int N = pattern[pid].size();
    vector<pair<int, int>> sorted(N);
    for (int i = 0; i < N; i++) sorted[i] = make_pair(pattern[pid][i], i);
    sort(sorted.begin(), sorted.end());
    vector<int> id(N);
    for (int i = 0; i < N; i++) id[sorted[i].second] = i;
    vector<int> BIT(N + 1);
    int cur = 0;
    for (int i = 0; i < N; i++) {
        int rank = 0;
        for (int j = id[i]; j > 0; j -= j & -j) rank += BIT[j];
        const LL key = (LL(cur) << 32) + rank;
        int& c = child[key];
        if (c == 0) {
            c = nodes.size();
            nodes.emplace_back();
            nodes.back().pid = pid;
            nodes.back().idx = i;
            nodes[cur].children.emplace_back(rank, c);
        }
        cur = c;
        for (int j = id[i] + 1; j <= N; j += j & -j) BIT[j] += 1;
    }
    nodes[cur].term = true;
    list<int> L;
    vector<list<int>::iterator> lookup(N);
    for (int i = 0; i < N; i++) lookup[i] = L.insert(L.end(), sorted[i].second);
    pred[pid].assign(N, -1); succ[pid].assign(N, -1);
    for (int i = N - 1; i >= 0; i--) {
        auto it = lookup[id[i]], orig_it = it;
        if (it != L.begin()) pred[pid][i] = *prev(it);
        if (++it != L.end()) succ[pid][i] = *it;
        L.erase(orig_it);
    }
}

/// Return the index in `nodes[suffix].children` of the child edge labelled by
/// the rank that equals the rank of `seq[pos]` compared to the N immediately
/// preceding elements of `seq` where `N` is the prefix length represented by
/// `nodes[suffix]`, or -1 if there is no such index.
int do_search(int suffix, int* seq, int pos) {
    const auto& node = nodes[suffix];
    int l = 0, r = node.children.size();
    while (r > l) {
        const int m = (l + r) / 2;
        const auto& child = nodes[node.children[m].second];
        const auto s = succ[child.pid][child.idx];
        // is `seq[pos]` "less than or equal to" this child?
        if (s >= 0 && seq[pos] > seq[pos - child.idx + s]) {
            l = m + 1;  // no
        } else {
            r = m;  // yes
        }
    }
    if (l == node.children.size()) return -1;
    const auto& child = nodes[node.children[l].second];
    const auto p = pred[child.pid][child.idx], s = succ[child.pid][child.idx];
    if ((p < 0 || seq[pos] > seq[pos - child.idx + p]) &&
        (s < 0 || seq[pos] < seq[pos - child.idx + s])) {
        return l;
    } else return -1;
}
/// Sort the `children` array for each node in the trie by rank, and generate
/// all the suffix links.
void ac() {
    queue<int> Q; Q.push(0);
    while (!Q.empty()) {
        const auto parent = Q.front(); Q.pop();
        sort(nodes[parent].children.begin(), nodes[parent].children.end());
        for (const auto& cp : nodes[parent].children) {
            const auto rank = cp.first;
            const auto child = cp.second;
            nodes[child].suffix = 0;
            for (int suffix = nodes[parent].suffix;
                 suffix >= 0;
                 suffix = nodes[suffix].suffix) {
                const auto r = do_search(suffix,
                                         pattern[nodes[child].pid].data(),
                                         nodes[child].idx);
                if (r >= 0) {
                    nodes[child].suffix = nodes[suffix].children[r].second;
                    break;
                }
            }
            Q.push(child);
        }
    }
}
void do_testcase() {
    int N; scanf("%d", &N); N--;
    int tl; scanf("%d", &tl);
    vector<int> text(tl);
    for (int i = 0; i < tl; i++) scanf("%d", &text[i]);
    pattern.assign(N, {});
    pred.assign(N, {});
    succ.assign(N, {});
    nodes.assign(1, {});
    child.clear();  // no longer needed
    for (int i = 0; i < N; i++) {
        int pl; scanf("%d", &pl); if (pl == 0) throw;
        pattern[i].resize(pl);
        for (int j = 0; j < pl; j++) scanf("%d", &pattern[i][j]);
        insert(i);
    }
    ac();
    int cur = 0;
    for (int i = 0; i < tl; i++) {
        for (; cur >= 0; cur = nodes[cur].suffix) {
            const auto r = do_search(cur, text.data(), i);
            if (r >= 0) {
                cur = nodes[cur].children[r].second;
                break;
            }
        }
        if (cur < 0) {
            cur = 0;
        } else if (nodes[cur].term) {
            printf("%d\n", i + 1);
        }
    }
}
int main() {
    int T; scanf("%d", &T); while (T--) do_testcase();
}
