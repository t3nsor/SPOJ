// 2025-05-23
// see https://codeforces.com/blog/entry/49446
#include <algorithm>
#include <queue>
#include <stdio.h>
#include <unordered_map>
#include <vector>
using namespace std;
struct SegtreeNode {
    int cnt;
    SegtreeNode* left = nullptr;
    SegtreeNode* right = nullptr;
    // ~SegtreeNode() { delete left; delete right; }
};
struct Segtree {
    int N;
    SegtreeNode root;
    Segtree(int N, int singleton) : N(N) {
        int l = 0, r = N;
        SegtreeNode* cur = &root;
        for (;;) {
            cur->cnt = 1;
            if (r - l == 1) return;
            const int m = (l + r) / 2;
            if (singleton < m) {
                r = m;
                cur = cur->left = new SegtreeNode;
            } else {
                l = m;
                cur = cur->right = new SegtreeNode;
            }
        }
    }

    int get(int k) {
        int l = 0, r = N;
        SegtreeNode* cur = &root;
        for (;;) {
            if (r - l == 1) return l;
            const int m = (l + r) / 2;
            if (cur->left && k < cur->left->cnt) {
                r = m;
                cur = cur->left;
            } else {
                l = m;
                k -= cur->left ? cur->left->cnt : 0;
                cur = cur->right;
            }
        }
    }
};
void merge(SegtreeNode*& left, SegtreeNode*& right) {
    if (!right) return;
    if (!left) { left = right; return; }
    left->cnt += right->cnt;
    merge(left->left, right->left);
    merge(left->right, right->right);
}
void merge(Segtree& left, Segtree&& right) {
    SegtreeNode* lroot = &left.root;
    SegtreeNode* rroot = &right.root;
    merge(lroot, rroot);
}
void dfs(int p, int u, const vector<vector<int>>& adj,
         const vector<int>& queries,
         const vector<vector<int>>& queries_for_node,
         vector<Segtree>& trees,
         vector<int>& answers) {
    for (const auto v : adj[u]) {
        if (v == p) continue;
        dfs(u, v, adj, queries, queries_for_node, trees, answers);
        merge(trees[u], move(trees[v]));
    }

    for (const auto i : queries_for_node[u]) {
        const int k = queries[i] - 1;
        answers[i] = trees[u].get(k);
    }
}
int main() {
    vector<int> id_to_label;
    vector<int> sorted_labels;
    int V; scanf("%d", &V);
    for (int i = 0; i < V; i++) {
        int label; scanf("%d", &label);
        id_to_label.push_back(label);
        sorted_labels.push_back(label);
    }
    sort(sorted_labels.begin(), sorted_labels.end());
    vector<int> label_to_id(V);
    vector<Segtree> trees;
    for (int i = 0; i < V; i++) {
        id_to_label[i] = lower_bound(sorted_labels.begin(),
                                     sorted_labels.end(),
                                     id_to_label[i])
                         - sorted_labels.begin();
        label_to_id[id_to_label[i]] = i;
        trees.emplace_back(V, id_to_label[i]);
    }
    vector<vector<int>> adj(V);
    for (int i = 0; i < V - 1; i++) {
        int u, v; scanf("%d %d", &u, &v); --u; --v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    int m; scanf("%d", &m);
    vector<int> queries(m);
    vector<vector<int>> queries_for_node(V);
    vector<int> answers(m);
    for (int i = 0; i < m; i++) {
        int x; scanf("%d %d", &x, &queries[i]);
        queries_for_node[x - 1].push_back(i);
    }
    dfs(-1, 0, adj, queries, queries_for_node, trees, answers);
    for (int i = 0; i < m; i++) {
        printf("%d\n", label_to_id[answers[i]] + 1);
    }
}
