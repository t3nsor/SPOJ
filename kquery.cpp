// 2023-09-10
#include <algorithm>
#include <cstdio>
#include <iterator>
#include <vector>
using namespace std;
void build_tree(vector<vector<int>>& tree,
                int node,
                int left,
                int right,
                const vector<int>& array) {
    if (tree.size() < node) tree.resize(node + 1);
    if (right - left == 1) {
        tree[node].push_back(array[left]);
        return;
    }
    const int mid = (left + right + 1) / 2;
    build_tree(tree, 2*node, left, mid, array);
    build_tree(tree, 2*node + 1, mid, right, array);
    merge(tree[2*node].begin(), tree[2*node].end(),
          tree[2*node + 1].begin(), tree[2*node + 1].end(),
          back_inserter(tree[node]));
}
int query(const vector<vector<int>>& tree,
          int node,
          int left,
          int right,
          int bound,
          int k) {
    int result = 0;
    if (right <= bound) {
        result = tree[node].end() -
                 upper_bound(tree[node].begin(), tree[node].end(), k);
    } else if (left < bound) {
        const int mid = (left + right + 1) / 2;
        result += query(tree, 2*node, left, mid, bound, k);
        result += query(tree, 2*node + 1, mid, right, bound, k);
    }
    return result;
}
int main() {
    int n; scanf("%d", &n);
    vector<int> a(n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    vector<vector<int>> tree;
    build_tree(tree, 1, 0, n, a);
    int q; scanf("%d", &q);
    while (q--) {
        int i, j, k; scanf("%d %d %d", &i, &j, &k);
        i--; j--;
        printf("%d\n", query(tree, 1, 0, n, j+1, k) -
                       query(tree, 1, 0, n, i, k));
    }
}
