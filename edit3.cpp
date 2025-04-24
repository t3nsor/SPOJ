// 2025-04-24
// We use e-maxx's implementation of the FHQ treap, which is in the public
// domain. A node holds a single character and the text is the inorder traversal
// of the tree.
// Note that this solution is too slow for https://dmoj.ca/problem/noi03p2 (and
// uses too much memory). A possible improvement is to have nodes represent
// substrings of the input file, instead of individual characters. This is left
// as an exercise for the reader.
#include <random>
#include <stdio.h>
using namespace std;
char buf[4000000];
namespace my { unsigned int random() {
    static mt19937 engine(597254518U);
    static uniform_int_distribution<unsigned> dist;
    return dist(engine);
} }
struct item {
    char key;
    int cnt = 1;
    unsigned prior = my::random();
    item *l = nullptr, *r = nullptr;
    item(char key) : key(key) {}
    ~item() { delete l; delete r; }
};
typedef item* pitem;
int cnt(pitem t) { return t ? t->cnt : 0; }
void upd(pitem t) {
    if (t) t->cnt = 1 + cnt(t->l) + cnt(t->r);
}
void merge(pitem & t, pitem l, pitem r) {
    if (!l || !r)
        t = l ? l : r;
    else if (l->prior > r->prior)
        merge (l->r, l->r, r), t = l;
    else
        merge (r->l, l, r->l), t = r;
    upd(t);
}
// split tree `t` into trees `l` and `r`, such that `l` holds the first `ofs`
// characters and `r` the remaining characters
void split(pitem t, int ofs, pitem & l, pitem & r) {
    if (!t)
        l = r = nullptr;
    else if (ofs <= cnt(t->l))
        split(t->l, ofs, l, t->l), r = t;
    else
        split(t->r, ofs - cnt(t->l) - 1, t->r, r), l = t;
    upd(t);
}
void ins(pitem& t, int pos, string text) {
    pitem l, r; split(t, pos, l, r);
    for (const char c : text) merge(l, l, new item(c));
    merge(t, l, r);
}
void del(pitem& t, int pos, int len) {
    pitem l, m, r; split(t, pos, l, m); split(m, len, m, r);
    merge(t, l, r); delete m;
}
void print(pitem t) {
    if (t) print(t->l), putchar(t->key), print(t->r);
}
void get(pitem t, int pos, int len) {
    pitem l, m, r; split(t, pos, l, r); split(r, len, m, r);
    print(m); merge(r, m, r); merge(t, l, r);
}
void do_testcase() {
    int N; scanf("%d", &N);
    pitem tree = nullptr;
    int cur = 0;
    while (N--) {
        int c;
        do { c = getchar(); } while (c <= 32);
        switch (c) {
          case 'M': {
            int pos; scanf("ove %d", &pos);
            cur = pos;
          } break;
          case 'I': {
            int len; scanf("nsert %d", &len);
            pitem l, r; split(tree, cur, l, r);
            while (len--) {
                do { c = getchar(); } while (c < 32);
                merge(l, l, new item(c));
            }
            merge(tree, l, r);
          } break;
          case 'D': {
            int len; scanf("elete %d", &len);
            del(tree, cur, len);
          } break;
          case 'G': {
            int len; scanf("et %d", &len);
            get(tree, cur, len);
            putchar('\n');
          } break;
          case 'P': {
            scanf("rev");
            --cur;
          } break;
          case 'N': {
            scanf("ext");
            ++cur;
          } break;
        }
    }
    delete tree;
}
int main() {
    int T; scanf("%d", &T); while (T--) do_testcase();
}
