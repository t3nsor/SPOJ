// 2025-08-11
// This solution uses the Dreyfus--Wagner algorithm.  It also may possible to
// solve this problem by brute force.  If there are 8 distinct terminals, then
// the Steiner tree has up to 6 intermediate points which are to be selected
// from a set of up to 22, so the number of possibilities is bounded by (22
// choose 6) + ... + (22 choose 0), or about 110,000.  For each set of
// intermediate points, the candidate Steiner tree is just the MST, which can
// be found in O(V^2) time where V is at most 14, so this seems like it can run
// in time, though I've not tested it.  Note that if there are fewer than 8
// distinct terminals, there are more intermediate points to choose from, but
// the number of them that we can choose is smaller, so the overall running
// time will be much less (compare, e.g., 22 choose 6 with 23 choose 5).
#include <algorithm>
#include <iostream>
#include <string>
#include <string.h>
#include <unordered_map>
#include <vector>
using namespace std;
void do_testcase(int V, int E) {
    unordered_map<string, int> name_to_idx;
    int dist[30][30]; memset(dist, 0x10, sizeof(dist));
    for (int i = 0; i < V; i++) {
        string name; cin >> name; name_to_idx[name] = i;
        dist[i][i] = 0;
    }
    for (int i = 0; i < E; i++) {
        string s1, s2; int d;
        cin >> s1 >> s2 >> d;
        const int u = name_to_idx[s1];
        const int v = name_to_idx[s2];
        dist[v][u] = dist[u][v] = min(dist[u][v], d);
    }
    int term[8];
    for (int i = 0; i < 8; i++) {
        string s; cin >> s;
        term[i] = name_to_idx[s];
    }
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }
    int dp[1<<8][30]; memset(dp, 0x10, sizeof(dp));
    int steiner[1<<8]; memset(steiner, 0x10, sizeof(steiner));
    vector<int> submasks[1<<8];
    submasks[0].push_back(0);
    for (int mask = 1; mask < (1<<8); mask++) {
        const int lsb = __builtin_ctz(mask);
        for (const int submask : submasks[mask - (1 << lsb)]) {
            submasks[mask].push_back(submask);
            submasks[mask].push_back(submask + (1 << lsb));
        }
        if (mask == (1 << lsb)) {
            for (int u = 0; u < V; u++) {
                dp[mask][u] = dist[term[lsb]][u];
            }
            continue;
        }
        for (const int submask : submasks[mask]) {
            if (submask == 0 || submask == mask) continue;
            for (int u = 0; u < V; u++) {
                for (int v = 0; v < V; v++) {
                    dp[mask][u] = min(
                      dp[mask][u],
                      dist[u][v] + dp[submask][v] + dp[mask - submask][v]
                    );
                }
            }
        }
        for (int u = 0; u < V; u++) {
            steiner[mask] = min(steiner[mask], dp[mask][u]);
        }
    }
    // try all possible ways to partition the 4 pairs
    int dp2[16];
    for (int i = 1; i < 16; i++) {
        int mask = 0;
        for (int j = 0; j < 4; j++) {
            if (i & (1 << j)) {
                mask += 3 << (2*j);
            }
        }
        dp2[i] = steiner[mask];
        for (int j = 0; j < 16; j++) {
            if (0 != (j &~i) || 0 == j || i == j) continue;
            dp2[i] = min(dp2[i], dp2[j] + dp2[i - j]);
        }
    }
    cout << dp2[15] << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    for (;;) {
        int V, E; cin >> V >> E; if (V == 0) break;
        do_testcase(V, E);
    }
}
