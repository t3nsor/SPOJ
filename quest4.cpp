// 2014-04-27
#include <vector>
#include <cstdio>
using namespace std;
// This max bipartite matching code is copied from the Stanford ACM team
// notebook, the most recent version of which may be found at:
// http://web.stanford.edu/~liszt90/acm/notebook_official.pdf
typedef vector<int> VI;
typedef vector<VI> VVI;

bool FindMatch(int i, const VVI &w, VI &mr, VI &mc, VI &seen) {
    for (int j = 0; j < w[i].size(); j++) {
        if (w[i][j] && !seen[j]) {
            seen[j] = true;
            if (mc[j] < 0 || FindMatch(mc[j], w, mr, mc, seen)) {
                mr[i] = j;
                mc[j] = i;
                return true;
            }
        }
    }
    return false;
}

int BipartiteMatching(const VVI &w, VI &mr, VI &mc) {
    mr = VI(w.size(), -1);
    mc = VI(w[0].size(), -1);

    int ct = 0;
    for (int i = 0; i < w.size(); i++) {
        VI seen(w[0].size());
        if (FindMatch(i, w, mr, mc, seen)) ct++;
    }
    return ct;
}

int main() {
    int T; scanf("%d", &T);
    while (T--) {
        VVI adj(120, VI(120, 0));
        VI foo(120), bar(120);
        int n; scanf("%d", &n);
        while (n--) {
            int x, y;
            scanf("%d %d", &x, &y);
            adj[x][y] = 1;
        }
        printf("%d\n", BipartiteMatching(adj, foo, bar));
    }
    return 0;
}
