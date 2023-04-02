// 2023-04-02
// The entire grid configuration has a period that divides 5040 (which is twice
// the LCM of 1, 2, ..., 9), so we only need to care about, for each `i` in the
// range 0, ..., 5039, the soonest we can reach each grid square at a time that
// equals `i` (mod 5040).
#include <climits>
#include <cstring>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;
struct Node {
    int time;
    int r;
    int c;
};
constexpr int P = 5040;
int dist[25][25][P];
constexpr int dr[] = {0, 1, 0, -1, 0};
constexpr int dc[] = {0, 0, 1, 0, -1};
void do_testcase() {
    int N; cin >> N;
    vector<string> grid(N);
    vector<string> period(N);
    for (int i = 0; i < N; i++) {
        cin >> grid[i];
    }
    for (int i = 0; i < N; i++) {
        cin >> period[i];
    }
    memset(dist, 0xff, sizeof(dist));
    queue<Node> Q;
    Q.push({0, 0, 0});
    while (!Q.empty()) {
        Node node = Q.front(); Q.pop();
        if (dist[node.r][node.c][node.time % P] >= 0) continue;
        // check whether the cell is open
        if (period[node.r][node.c] == '0') {
            if (grid[node.r][node.c] == '*') continue;
        } else {
            const int p = period[node.r][node.c] - '0';
            if ((grid[node.r][node.c] == '*' &&
                 (node.time % (2 * p)) < p) ||
                (grid[node.r][node.c] == '.' &&
                 (node.time % (2 * p)) >= p)) continue;
        }
        dist[node.r][node.c][node.time % P] = node.time;
        for (int k = 0; k < 5; k++) {
            const int newr = node.r + dr[k];
            const int newc = node.c + dc[k];
            if (newr >= 0 && newr < N && newc >= 0 && newc < N) {
                Q.push({node.time + 1, newr, newc});
            }
        }
    }
    int result = INT_MAX;
    for (int i = 0; i < P; i++) {
        if (dist[N-1][N-1][i] >= 0) {
            result = min(result, dist[N-1][N-1][i]);
        }
    }
    if (result == INT_MAX) puts("NO"); else cout << result << '\n';
}
int main() {
    int T; cin >> T;
    while (T--) do_testcase();
}
