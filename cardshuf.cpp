// In order to support the "delete from given position" and "insert at given
// position" operations we need to use a balanced BST structure. The idea is
// that we ignore the numerical order of the node keys, and solely perform
// insertions and deletions based on rank. Finally, at the end we have to
// determine the number of elements that need to be moved in order to restore
// the deck to its original order. This is just n minus the length of the
// longest increasing subsequence.
// Splay tree copied from orderset-2.cpp, with modifications to support insert
// by rank.
#include <iterator>
#include <stack>
#include <stddef.h>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
struct Node {
    int value;
    int size = 0;
    Node* p;
    Node* c[2] = {0, 0};
};
struct Tree {
    Node head;
    Tree() {
        head.p = 0;
        head.size = 1;
    }
    static int get_size(Node* n) {
        return n ? n->size : 0;
    }
    static void lrot(Node* n) {
        Node* right = n->c[1];
        n->c[1] = right->c[0];
        if (n->c[1]) n->c[1]->p = n;
        right->c[0] = n;
        right->p = n->p;
        if (n == n->p->c[0]) {
            n->p->c[0] = right;
        } else {
            n->p->c[1] = right;
        }
        n->p = right;
        right->size = n->size;
        n->size = 1 + get_size(n->c[0]) + get_size(n->c[1]);
    }
    static void rrot(Node* n) {
        Node* left = n->c[0];
        n->c[0] = left->c[1];
        if (n->c[0]) n->c[0]->p = n;
        left->c[1] = n;
        left->p = n->p;
        if (n == n->p->c[0]) {
            n->p->c[0] = left;
        } else {
            n->p->c[1] = left;
        }
        n->p = left;
        left->size = n->size;
        n->size = 1 + get_size(n->c[0]) + get_size(n->c[1]);
    }
    void splay(Node* n) {
        for (;;) {
            if (n == &head || n == head.c[0]) return;
            Node* root = head.c[0];
            if (n == root->c[0]) {
                rrot(root);
                return;
            } else if (n == root->c[1]) {
                lrot(root);
                return;
            }
            Node* parent = n->p;
            Node* grandparent = parent->p;
            if (n == parent->c[0]) {
                if (parent == grandparent->c[0]) {
                    rrot(grandparent);
                    rrot(parent);
                } else {
                    rrot(parent);
                    lrot(grandparent);
                }
            } else {
                if (parent == grandparent->c[0]) {
                    lrot(parent);
                    rrot(grandparent);
                } else {
                    lrot(grandparent);
                    lrot(parent);
                }
            }
        }
    }
    void ins(int rank, Node* n) {
        n->size = 0;
        n->c[0] = 0;
        n->c[1] = 0;
        Node* cur = &head;
        for (;;) {
            const int lsize = get_size(cur->c[0]);
            if (rank <= lsize) {
                if (cur->c[0]) {
                    cur = cur->c[0];
                } else {
                    cur->c[0] = n;
                    break;
                }
            } else if (cur->c[1]) {
                rank -= 1 + lsize;
                cur = cur->c[1];
            } else {
                cur->c[1] = n;
                break;
            }
        }
        n->p = cur;
        for (Node* q = n; q; q = q->p) {
            q->size++;
        }
        splay(n);
    }
    Node* del(int rank) {
        Node* cur = &head;
        for (;;) {
            const int lsize = get_size(cur->c[0]);
            if (rank < lsize) {
                cur = cur->c[0];
            } else if (rank == lsize) {
                break;
            } else {
                rank -= 1 + lsize;
                cur = cur->c[1];
            }
        }
        if (cur->c[0]) {
            if (cur->c[1]) {
                Node* pred = cur->c[0];
                while (pred->c[1]) pred = pred->c[1];
                swap(cur->value, pred->value);
                cur = pred;
            }
            if (cur == cur->p->c[0]) {
                cur->p->c[0] = cur->c[0];
            } else {
                cur->p->c[1] = cur->c[0];
            }
            if (cur->c[0]) cur->c[0]->p = cur->p;
        } else {
            if (cur == cur->p->c[0]) {
                cur->p->c[0] = cur->c[1];
            } else {
                cur->p->c[1] = cur->c[1];
            }
            if (cur->c[1]) cur->c[1]->p = cur->p;
        }
        for (Node* q = cur->p; q; q = q->p) {
            --q->size;
        }
        splay(cur->p);
        return cur;
    }
    template <class It>
    void inorder(It out) {
        // a splay tree can be very deep, so use a custom stack for the
        // traversal
        stack<pair<Node*, int>> cont;
        pair<Node*, int> cur(head.c[0], 0);
        for (;;) {
            Node* node = cur.first;
            int step = cur.second;
            if (node) {
                if (step == 0) {
                    cont.emplace(node, 1);
                    cur.first = node->c[0];
                } else if (step == 1) {
                    *out++ = node->value;
                    cur.second++;
                } else {
                    cur.first = node->c[1];
                    cur.second = 0;
                }
            } else {
                if (cont.empty()) return;
                cur = cont.top();
                cont.pop();
            }
        }
    }
};
long long gcd(long long x, long long y) {
    if (x == 0) return y;
    return gcd(y % x, x);
}
int main() {
    int n, x; scanf("%d %d", &n, &x);
    Tree tree;
    vector<Node> pool(n);
    for (int i = 0; i < n; i++) {
        pool[i].value = i;
        tree.ins(i, &pool[i]);
    }
    for (int i = 0; i < x; i++) {
        int y, z; scanf("%d %d", &y, &z);
        Node* n = tree.del(y - 1);
        tree.ins(z - 1, n);
    }
    vector<int> perm;
    tree.inorder(back_inserter(perm));
    vector<int> LIS(n + 1, 1e9);
    LIS[0] = -1;
    int lis = -1;
    for (auto value : perm) {
        const int j = lower_bound(LIS.begin(), LIS.end(), value) - LIS.begin();
        LIS[j] = min(LIS[j], value);
        lis = max(lis, j);
    }
    printf("%d\n", n - lis);
}
