// 2014-05-02
#include <cstdio>
#include <vector>
#include <queue>
using namespace std;
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        int V, E, s, t;
        scanf("%d %d %d %d", &V, &E, &s, &t); s--; t--;
        vector<vector<pair<int, int> > > adj(V);
        vector<int> dist(V, 1e9);
        vector<bool> vis(V, false);
        while (E--) {
            int x, y, d; scanf("%d %d %d", &x, &y, &d);
            adj[x-1].push_back(make_pair(d, y-1));
            adj[y-1].push_back(make_pair(d, x-1));
        }
        priority_queue<pair<int, int>, vector<pair<int, int> >,
                       greater<pair<int, int> > > Q;
        Q.push(make_pair(0, s));
        dist[s] = 0;
        while (!Q.empty()) {
            pair<int, int> P = Q.top(); Q.pop();
            int v = P.second;
            if (vis[v]) continue; vis[v] = true;
            if (v == t) break;
            for (int i = 0; i < adj[v].size(); i++) {
                int newdist = P.first + adj[v][i].first;
                if (newdist < dist[adj[v][i].second]) {
                    dist[adj[v][i].second] = newdist;
                    Q.push(make_pair(newdist, adj[v][i].second));
                }
            }
        }
        if (dist[t] < 1e9) {
            printf("%d\n", dist[t]);
        } else {
            puts("NONE");
        }
    }
}
