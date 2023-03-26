// 2023-03-26
#include <cstdio>
#include <vector>
using namespace std;
void do_testcase() {
    int V, E; scanf("%d %d", &V, &E);
    vector<int> id(V);
    for (int i = 0; i < V; i++) id[i] = i;
    int result = 0;
    while (E--) {
        int p, q;
        scanf("%d %d", &p, &q); --p; --q;
        while (p != id[p]) p = id[p] = id[id[p]];
        while (q != id[q]) q = id[q] = id[id[q]];
        if (p == q) ++result; else id[p] = q;
    }
    printf("%d\n", result);
}
int main() {
    int T; scanf("%d", &T);
    while (T--) do_testcase();
}
