// 2025-07-02
// I replace each label with its rank among all labels in order to avoid
// repetitive string comparisons. (I don't know whether that optimization is
// necessary to get AC, but it seemed prudent.) Then, just simulate inserting
// nodes into a binary search tree one by one in order of decreasing priority,
// using a `std::map` to efficiently find the insertion point.
#include <algorithm>
#include <functional>
#include <map>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>
using namespace std;
struct Tree {
    int prio;
    int rank;
    ~Tree() { delete left; delete right; }
    Tree* left = nullptr;
    Tree* right = nullptr;
};
void print(Tree* node, const vector<pair<string, int>>& label_and_prio) {
    putchar('(');
    if (node->left) print(node->left, label_and_prio);
    printf("%s/%d", label_and_prio[node->rank].first.c_str(), node->prio);
    if (node->right) print(node->right, label_and_prio);
    putchar(')');
}
void do_testcase(int n) {
    vector<pair<string, int>> label_and_prio;
    while (n--) {
        string label;
        int c;
        do {
            c = getchar_unlocked();
        } while (c <= 32);
        do {
            label.push_back(c);
            c = getchar_unlocked();
        } while (c != '/');
        int prio;
        scanf("%d", &prio);
        label_and_prio.emplace_back(move(label), prio);
    }
    sort(label_and_prio.begin(), label_and_prio.end());
    vector<pair<int, int>> prio_and_rank;
    for (int i = 0; i < label_and_prio.size(); i++) {
        prio_and_rank.emplace_back(label_and_prio[i].second, i);
    }
    sort(prio_and_rank.begin(), prio_and_rank.end(), greater<>());
    Tree* root = new Tree{prio_and_rank[0].first, prio_and_rank[0].second};
    map<int, Tree*> M = {{prio_and_rank[0].second, root}};
    for (int i = 1; i < prio_and_rank.size(); i++) {
        Tree* new_node = new Tree{prio_and_rank[i].first,
                                  prio_and_rank[i].second};
        auto it = M.lower_bound(prio_and_rank[i].second);
        if (it == M.end()) {
            // this value is larger than all values already in the tree
            --it;
            M[prio_and_rank[i].second] = it->second->right = new_node;
            continue;
        }
        // `it` points to the first node of the tree that's greater; check
        // whether we can add this node as its left child
        if (!it->second->left) {
            M[prio_and_rank[i].second] = it->second->left = new_node;
        } else {
            // otherwise, we must be the right child of the predecessor
            --it;
            M[prio_and_rank[i].second] = it->second->right = new_node;
        }
    }
    print(root, label_and_prio);
    putchar('\n');
    delete root;
}
int main() {
    for (;;) {
        int n; scanf("%d", &n);
        if (0 == n) return 0;
        do_testcase(n);
    }
}
