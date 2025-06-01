// 2025-05-31
// Construct the directed graph where each node represents a person, and there
// is an edge from u to v if u accuses v. The problem is to find the maximum
// possible set of "innocent" nodes. An innocent node does not accuse another
// innocent node. In fact, we see that this is really an undirected graph
// problem. In the undirected version of the graph, we want to find the maximum
// independent set. Maximum independent set is NP-hard in general but in this
// case the graph has a special structure.
//
// Consider an individual connected component in the undirected graph. It
// consists of a cycle with zero or more trees attached. The maximum number of
// nodes we can select from a tree depends on whether the cycle node is chosen:
// call the node's "white value" the maximum number of tree nodes that can be
// selected from the trees if the cycle node is chosen, and the "black value"
// the maximum when the cycle node is not chosen. The black and white values can
// easily be computed recursively. This reduces the problem to that of colouring
// the cycle nodes black or white, such that no two white nodes are adjacent,
// and we want to maximize the total value, where the value of a node is the
// black value if coloured black and white otherwise. This is easily solved
// using DP.

#include <algorithm>
#include <stdio.h>
#include <vector>
using namespace std;
void find_component(const vector<int>& dedge, int u,
                    vector<int>& vis,
                    vector<int>& is_cycle,
                    vector<vector<int>>& components) {
    if (vis[u] == 0) {
        // node not yet seen
        vis[u] = 1;  // awaiting component assignment
        find_component(dedge, dedge[u], vis, is_cycle, components);
        if (vis[u] == 1) {
            vis[u] = 2;  // component assigned
            components.back().push_back(u);
        }
    } else if (vis[u] == 1) {
        int v = u;
        components.emplace_back();
        do {
            components.back().push_back(v);
            is_cycle[v] = 1;
            vis[v] = 2;
            v = dedge[v];
        } while (v != u);
    }
}
void calc_tree_value(int u, const vector<vector<int>>& treechild,
                     vector<int>& black, vector<int>& white) {
    if (white[u] == 0) {
        white[u] = 1;
        for (const auto v : treechild[u]) {
            calc_tree_value(v, treechild, black, white);
            white[u] += black[v];
            black[u] += max(black[v], white[v]);
        }
    }
}
int dp(const vector<int>& black, const vector<int>& white) {
    // assume first node coloured white
    const int N = black.size();
    int b = -1e9;
    int w = white[0];
    for (int i = 1; i < N; i++) {
        const int b2 = black[i] + max(b, w);
        const int w2 = white[i] + b;
        b = b2;
        w = w2;
    }
    int result = b;  // last node must be black
    // now assume first node coloured black
    b = black[0];
    w = -1e9;
    for (int i = 1; i < N; i++) {
        const int b2 = black[i] + max(b, w);
        const int w2 = white[i] + b;
        b = b2;
        w = w2;
    }
    return max({result, b, w});
}
void do_testcase() {
    int N; scanf("%d", &N);
    vector<int> dedge(N);
    for (int i = 0; i < N; i++) {
        scanf("%d", &dedge[i]); --dedge[i];
    }
    vector<int> vis(N);
    vector<int> is_cycle(N);
    vector<vector<int>> components;
    for (int i = 0; i < N; i++) {
        find_component(dedge, i, vis, is_cycle, components);
    }
    vector<vector<int>> treechild(N);
    for (int i = 0; i < N; i++) {
        if (!is_cycle[i]) {
            treechild[dedge[i]].push_back(i);
        }
    }
    vector<int> black(N), white(N);
    for (int i = 0; i < N; i++) {
        calc_tree_value(i, treechild, black, white);
    }
    int result = N;
    for (const auto& component : components) {
        for (const auto u : component) {
            if (is_cycle[u]) {
                vector<int> b, w;
                int v = u;
                do {
                    b.push_back(black[v]);
                    w.push_back(white[v]);
                    v = dedge[v];
                } while (v != u);
                result -= dp(b, w);
                break;
            }
        }
    }
    printf("%d\n", result);
}
int main() {
    int T; scanf("%d", &T); while (T--) do_testcase();
}
