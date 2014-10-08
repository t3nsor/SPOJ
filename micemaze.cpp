// 2014-05-01
#include <iostream>
#include <queue>
#include <vector>
#include <cstring>
using namespace std;
int main() {
    int N, E, T, M;
    vector<pair<int, int> > adj[100];
    int dist[100]; memset(dist, 0x3f, sizeof dist);
    cin >> N >> E >> T >> M; E--;
    while (M--) {
        int a, b, t; cin >> a >> b >> t; a--; b--;
        adj[b].push_back(make_pair(t, a));
    }
    priority_queue<pair<int, int>, vector<pair<int, int> >,
                   greater<pair<int, int> > > Q;
    Q.push(make_pair(0, E));
    while (!Q.empty()) {
        pair<int, int> P = Q.top(); Q.pop();
        if (dist[P.second] < 0x3f3f3f3f) continue;
        dist[P.second] = P.first;
        for (int i = 0; i < adj[P.second].size(); i++) {
            Q.push(make_pair(P.first + adj[P.second][i].first,
                             adj[P.second][i].second));
        }
    }
    int cnt = 0;
    for (int i = 0; i < N; i++) {
        if (dist[i] <= T) cnt++;
    }
    cout << cnt << endl;
    return 0;
}
