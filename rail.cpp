// 2026-09-28
// Another one of those problems where the hardest part is understanding the
// problem.  The set of stations to remove is determined at the beginning before
// removing any stations.  That is, in the following example:
//
// 1 --- 2 --- 3 --- +
//       |           |
//       + --- 4 --- 5 --- 6
//
// stations 2 and 5 should not be removed, even though after removing stations 3
// and 4 the resulting network has 2 connected to only 1 and 5, and 5 connected
// to only 1 and 2.  When doing the actual removal we must be careful.  Look at
// the second sample case, where we have a loop 2, 3, 4 where stations 3 and 4
// must be removed.  If we remove station 3 and rearrange the graph immediately
// then we have a bunch of edges between 2 and 4, some of which were originally
// between 2 and 4, some of which were created by the removal of station 3.
// When removing station 4 we must create edges from 2 to itself that are the
// sum of an edge from one group and an edge from the other.  Instead of trying
// to keep the groups separate somehow, my solution identifies all chains of
// stations that must be removed and handles one chain at a time.
#include <algorithm>
#include <map>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
void do_testcase() {
    int V, E; scanf("%d %d", &V, &E);
    vector<map<int, vector<int>>> adj(V);
    while (E--) {
        int u, v, d; scanf("%d %d %d", &u, &v, &d); --u; --v;
        adj[u][v].push_back(d);
        adj[v][u].push_back(d);
    }
    vector<vector<int>> chains;
    vector<char> must_remove(V, 0);
    for (int i = 0; i < V; i++) {
        if (must_remove[i] || adj[i].size() != 2) continue;
        vector<int> halfchain[2];
        for (int j = 0; j < 2; j++) {
            int u = i, v;
            if (j == 0) {
                v = adj[i].begin()->first;
            } else {
                v = next(adj[i].begin())->first;
            }
            halfchain[j].push_back(v);
            while (adj[v].size() == 2) {
                must_remove[v] = 1;
                const int w1 = adj[v].begin()->first;
                const int w2 = next(adj[v].begin())->first;
                u = exchange(v, (w1 != u) ? w1 : w2);
                halfchain[j].push_back(v);
            }
        }
        vector<int> chain = halfchain[0];
        reverse(chain.begin(), chain.end());
        chain.push_back(i);
        chain.insert(chain.end(), halfchain[1].begin(), halfchain[1].end());
        chains.push_back(move(chain));
    }
    for (const auto& chain : chains) {
        vector<int> dist = adj[chain[0]][chain[1]];
        for (int i = 1; i + 1 < chain.size(); i++) {
            vector<int> dist2;
            for (int j = 0; j < dist.size(); j++) {
                for (int k = 0; k < adj[chain[i]][chain[i + 1]].size(); k++) {
                    dist2.push_back(dist[j] + adj[chain[i]][chain[i + 1]][k]);
                }
            }
            dist = move(dist2);
        }
        adj[chain[0]].erase(chain[1]);
        adj[chain[chain.size() - 1]].erase(chain[chain.size() - 2]);
        for (int i = 1; i + 1 < chain.size(); i++) adj[chain[i]].clear();
        adj[chain.front()][chain.back()].insert(
          adj[chain.front()][chain.back()].end(),
          dist.begin(),
          dist.end()
        );
        if (chain.front() != chain.back()) {
            adj[chain.back()][chain.front()].insert(
              adj[chain.back()][chain.front()].end(),
              dist.begin(),
              dist.end()
            );
        }
    }
    vector<int> out1;
    vector<int> out2;
    vector<int> out3;
    for (int i = 0; i < V; i++) {
        for (auto& kv : adj[i]) {
            if (kv.first < i) continue;
            sort(kv.second.begin(), kv.second.end());
            for (const auto d : kv.second) {
                out1.push_back(i);
                out2.push_back(kv.first);
                out3.push_back(d);
            }
        }
    }
    printf("%d\n", (int)out1.size());
    for (int i = 0; i < out1.size(); i++) {
        printf("%d %d %d\n", out1[i] + 1, out2[i] + 1, out3[i]);
    }
}
int main() {
    int T; scanf("%d", &T);
    for (int i = 0; i < T; i++) {
        do_testcase();
        if (i < T - 1) putchar('\n');
    }
}
