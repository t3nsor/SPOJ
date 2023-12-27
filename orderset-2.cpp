// 2023-12-27
// I use a splay tree because it's simple to code (even so, I had to track down
// multiple bugs before this solution got accepted...)
#include <stddef.h>
#include <stdio.h>
#include <utility>
using namespace std;
struct Node {
    int value;
    int size = 0;
    Node* p;
    Node* c[2] = {0, 0};
    void* operator new(size_t);
    void operator delete(void*) {}
};
char pool[222222 * sizeof(Node)];
int pool_next = 0;
void* Node::operator new(size_t) {
    return pool + (pool_next++) * sizeof(Node);
}
struct Tree {
    Node head;
    Tree() {
        head.value = 2e9;
        head.p = 0;
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
    void try_insert(int value) {
        Node* cur = &head;
        Node* result;
        bool inserted = false;
        for (;;) {
            if (value < cur->value) {
                if (cur->c[0]) {
                    cur = cur->c[0];
                } else {
                    result = cur->c[0] = new Node;
                    result->p = cur;
                    result->value = value;
                    inserted = true;
                    break;
                }
            } else if (value > cur->value) {
                if (cur->c[1]) {
                    cur = cur->c[1];
                } else {
                    result = cur->c[1] = new Node;
                    result->p = cur;
                    result->value = value;
                    inserted = true;
                    break;
                }
            } else {
                result = cur;
                break;
            }
        }
        if (inserted) {
            for (Node* n = result; n; n = n->p) {
                ++n->size;
            }
        }
        splay(result);
    }
    void try_delete(int value) {
        Node* cur = &head;
        for (;;) {
            if (value < cur->value) {
                if (cur->c[0]) {
                    cur = cur->c[0];
                } else {
                    splay(cur);
                    return;
                }
            } else if (value > cur->value) {
                if (cur->c[1]) {
                    cur = cur->c[1];
                } else {
                    splay(cur);
                    return;
                }
            } else {
                break;
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
        for (Node* n = cur->p; n; n = n->p) {
            --n->size;
        }
        splay(cur->p);
        delete cur;
    }
    // note: k is 0-indexed in this function
    Node* get_kth(int k) {
        if (k >= head.size) return 0;
        Node* cur = &head;
        for (;;) {
            const int lsize = get_size(cur->c[0]);
            if (k < lsize) {
                cur = cur->c[0];
            } else if (k == lsize) {
                splay(cur);
                return cur;
            } else {
                k -= lsize + 1;
                cur = cur->c[1];
            }
        }
    }
    int count(int value) {
        Node* cur = &head;
        int left = 0;
        for (;;) {
            if (value == cur->value) {
                int result = left + get_size(cur->c[0]);
                splay(cur);
                return result;
            } else if (value < cur->value) {
                if (cur->c[0]) {
                    cur = cur->c[0];
                } else {
                    splay(cur);
                    return left;
                }
            } else {
                left += 1 + get_size(cur->c[0]);
                if (cur->c[1]) {
                    cur = cur->c[1];
                } else {
                    splay(cur);
                    return left;
                }
            }
        }
    }
};
char get_op() {
    char c;
    do {
        c = getchar_unlocked();
    } while (c <= 32);
    return c;
}
int main() {
    int Q; scanf("%d", &Q);
    Tree tree;
    while (Q--) {
        const char op = get_op();
        int value; scanf("%d", &value);
        switch (op) {
          case 'I': {
            tree.try_insert(value);
          } break;
          case 'D': {
            tree.try_delete(value);
          } break;
          case 'K': {
            Node* n = tree.get_kth(value - 1);
            if (n) {
                printf("%d\n", n->value);
            } else {
                puts("invalid");
            }
          } break;
          case 'C': {
            printf("%d\n", tree.count(value));
          } break;
        }
    }
}
