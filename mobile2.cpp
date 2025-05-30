// 2025-05-29
#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <vector>
using namespace std;
struct Node {
    int l;
    int r;
    int d;
    int td1;
    int td2;
};
void do_testcase() {
    int n; scanf("%d", &n);
    vector<Node> v(n);
    for (int i = 0; i < n; i++) {
        v[i].td1 = v[i].td2 = -1;
    }
    v[0].d = 0;
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &v[i].l, &v[i].r);
        if (v[i].l > 0) {
            --v[i].l;
            v[v[i].l].d = v[i].d + 1;
        }
        if (v[i].r > 0) {
            --v[i].r;
            v[v[i].r].d = v[i].d + 1;
        }
    }
    for (int i = n - 1; i >= 0; i--) {
        const auto insert = [&](int d) {
            if (d == -1) return true;
            if (v[i].td1 == d || v[i].td2 == d) return true;
            if (v[i].td1 == -1) {
                v[i].td1 = d;
                return true;
            }
            if (v[i].td2 == -1) {
                v[i].td2 = d;
                if (v[i].td2 < v[i].td1) {
                    swap(v[i].td2, v[i].td1);
                }
                return true;
            }
            return false;
        };
        if (v[i].l == -1) {
            insert(v[i].d);
        } else {
            insert(v[v[i].l].td1);
            insert(v[v[i].l].td2);
        }
        if (v[i].r == -1) {
            if (!insert(v[i].d)) {
                puts("-1"); return;
            }
        } else {
            if (!insert(v[v[i].r].td1) || !insert(v[v[i].r].td2)) {
                puts("-1"); return;
            }
        }
    }
    if (v[0].td2 != -1 && v[0].td2 - v[0].td1 != 1) {
        puts("-1"); return;
    }
    int steps = 0;
    int cur = 0;
    for (;;) {
        if (v[cur].td2 == -1) {
            printf("%d\n", steps); return;
        }
        if (v[cur].l >= 0 && v[cur].r >= 0) {
            if (v[v[cur].l].td2 >= 0 && v[v[cur].r].td2 >= 0) {
                puts("-1"); return;
            }
            if (v[v[cur].l].td2 >= 0) {
                if (v[v[cur].l].td2 == v[v[cur].r].td1) ++steps;
                cur = v[cur].l;
            } else if (v[v[cur].r].td2 >= 0) {
                if (v[v[cur].r].td1 == v[v[cur].l].td1) ++steps;
                cur = v[cur].r;
            } else {
                if (v[v[cur].l].td1 < v[v[cur].r].td1) {
                    ++steps;
                }
                printf("%d\n", steps);
                return;
            }
        } else if (v[cur].l >= 0) {
            cur = v[cur].l;
        } else {
            ++steps;
            cur = v[cur].r;
        }
    }
}
int main() {
    int T; scanf("%d", &T);
    while (T--) do_testcase();
}
