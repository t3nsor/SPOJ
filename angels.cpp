// 2024-09-07
// This is a bipartite matching problem: an empty cell is considered to be an
// edge from its row to its column. Because angels disrupt visibility across a
// row or a column, we consider two parts of a row that are separated by an
// angel to be two different rows, and similarly for columns.
#include <algorithm>
#include <ctype.h>
#include <queue>
#include <stdio.h>
#include <vector>
using namespace std;

// Max flow implementation taken from fastflow.cpp
struct st {
    int v;
    int cap;
    int back;
};
long long get_max_flow(vector<vector<st>>&& adj, int s, int t) {
    const int V = adj.size();
    vector<long long> excess(V, 0);
    vector<int> active(V, 0);
    vector<int> height(V, 0);
    vector<int> hcnt(2*V + 1, 0);
    queue<int> Q;
    height[s] = V;
    hcnt[V] = 1;
    hcnt[0] = V - 1;
    for (int i = 0; i < V; i++) {
        if (i == s || i == t) continue;
        active[i] = 1;
        Q.push(i);
    }
    for (auto& edge : adj[s]) {
        excess[edge.v] += edge.cap;
        adj[edge.v][edge.back].cap = edge.cap;
        edge.cap = 0;
    }
    while (!Q.empty()) {
        const int u = Q.front();
        Q.pop();
        active[u] = 0;
        int best = 1e9;
        for (auto& edge : adj[u]) {
            if (!edge.cap) continue;
            if (height[u] == height[edge.v] + 1) {
                const int x = min<long long>(excess[u], edge.cap);
                excess[u] -= x;
                excess[edge.v] += x;
                edge.cap -= x;
                adj[edge.v][edge.back].cap += x;
                if (edge.v != s && edge.v != t && !active[edge.v]) {
                    active[edge.v] = 1;
                    Q.push(edge.v);
                }
                if (excess[u] == 0) break;
            } else {
                best = min(best, height[edge.v]);
            }
        }
        if (excess[u] > 0) {
            const int oldh = height[u];
            height[u] = best + 1;
            hcnt[best + 1]++;
            if (0 == --hcnt[oldh] && oldh < V) {
                for (int i = 0; i < V; i++) {
                    if (i != s && i != t &&
                        height[i] > oldh && height[i] <= V) {
                        hcnt[height[i]]--;
                        height[i] = V + 1;
                    }
                }
            }
            if (!active[u]) {
                active[u] = 1;
                Q.push(u);
            }
        }
    }
    return excess[t];
}

void do_testcase() {
    int R, C; scanf("%d %d", &R, &C);
    vector<vector<char>> grid(R);
    for (int i = 0; i < R; i++) {
        grid[i].resize(C);
        for (int j = 0; j < C; j++) {
            do {
                grid[i][j] = getchar();
            } while (isspace(grid[i][j]));
        }
    }
    vector<vector<int>> row(R, vector<int>(C, 1));
    vector<vector<int>> col(R, vector<int>(C, 1));
    int nrow = 0;
    for (int i = 0; i < R; i++) {
        ++nrow;
        for (int j = 0; j < C; j++) {
            if (grid[i][j] == 'A') {
                ++nrow;
            } else if (grid[i][j] == 'H') {
                row[i][j] = nrow - 1;
            }
        }
    }
    int ncol = 0;
    for (int i = 0; i < C; i++) {
        ++ncol;
        for (int j = 0; j < R; j++) {
            if (grid[j][i] == 'A') {
                ++ncol;
            } else if (grid[j][i] == 'H') {
                col[j][i] = ncol - 1;
            }
        }
    }
    vector<vector<st>> adj(nrow + ncol + 2);
    for (int i = 0; i < nrow; i++) {
        adj[0].push_back(st{i + 1, 1, 0});
        adj[i + 1].push_back(st{0, 0, i});
    }
    for (int i = 0; i < ncol; i++) {
        adj[nrow + i + 1].push_back(st{nrow + ncol + 1, 1, i});
        adj[nrow + ncol + 1].push_back(st{nrow + i + 1, 0, 0});
    }
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            if (grid[i][j] == 'H') {
                const int r = row[i][j];
                const int c = col[i][j];
                adj[r + 1].push_back(st{nrow + 1 + c, 1,
                                        (int)adj[nrow + 1 + c].size()});
                adj[nrow + 1 + c].push_back(st{r + 1, 0,
                                               (int)adj[r + 1].size() - 1});
            }
        }
    }
    printf("%lld\n", get_max_flow(move(adj), 0, nrow + ncol + 1));
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
