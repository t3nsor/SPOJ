// 2016-04-29
#include <cstdio>
#include <algorithm>
using namespace std;
struct Node {
    Node* left = nullptr;
    Node* right = nullptr;
    int cnt = 0;
} root;
const int root_l = 1;
const int root_r = 1000000001;
int query(int ql, int qr, int tl = root_l, int tr = root_r, Node* n = &root) {
    if (n->cnt == 0) return 0;
    if (n->cnt == tr-tl) return qr-ql;
    int tm = (tl + tr)/2;
    int res = 0;
    if (ql < tm) res += query(ql, min(qr, tm), tl, tm, n->left);
    if (qr > tm) res += query(max(tm, ql), qr, tm, tr, n->right);
    return res;
}
void update(int ql, int qr, int tl = root_l, int tr = root_r,
            Node* n = &root) {
    if (n->cnt == tr-tl) return;
    if (ql == tl && qr == tr) { n->cnt = tr-tl; return; }
    int tm = (tl + tr)/2;
    if (!n->left) n->left = new Node;
    if (!n->right) n->right = new Node;
    if (ql < tm) update(ql, min(qr, tm), tl, tm, n->left);
    if (qr > tm) update(max(tm, ql), qr, tm, tr, n->right);
    n->cnt = n->left->cnt + n->right->cnt;
}
int main() {
    int M; scanf("%d", &M);
    int C = 0;
    while (M--) {
        int op, x, y;
        scanf("%d %d %d", &op, &x, &y);
        if (op == 1) {
            // query
            printf("%d\n", C = query(x+C, y+C+1));
        } else {
            // update
            update(x+C, y+C+1);
        }
    }
}
