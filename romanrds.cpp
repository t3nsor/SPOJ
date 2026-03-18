// 2026-03-18
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
int E;
int em[500][500];
vector<pair<int, int>> ends(500, make_pair(-1, -1));
bool vis[500];
int nextv = 2;
bool dfs1(int e) {
    vis[e] = true;
    int e2 = -1;
    for (int i = 0; i < E; i++) {
        if (i == e || !em[e][i]) continue;
        if (e2 == -1 || (e2 >= 0 && !vis[e2] && vis[i])) e2 = i;
    }
    if (e2 == -1) return false;  // an edge must connect to at least one edge
    int eshared;
    if (vis[e2]) {
        if (ends[e].first == ends[e2].first ||
            ends[e].first == ends[e2].second) {
            eshared = ends[e].first;
        } else if (ends[e].second == ends[e2].first ||
                   ends[e].second == ends[e2].second) {
            eshared = ends[e].second;
        } else {
            return false;
        }
    } else {
        ends[e2].first = ends[e].first;
        eshared = ends[e].first;
        ends[e2].second = nextv++;
    }
    const int eother = ends[e].first + ends[e].second - eshared;
    for (int e3 = 0; e3 < E; e3++) {
        if (e3 == e || !em[e][e3]) continue;
        if (e3 != e2) {
            const int expect = em[e2][e3] ? eshared : eother;
            if (ends[e3].first < 0) {
                ends[e3].first = expect;
                ends[e3].second = nextv++;
            } else if (ends[e3].first != expect && ends[e3].second != expect) {
                return false;
            }
        }
        if (!vis[e3]) {
            if (!dfs1(e3)) return false;
        }
    }
    return true;
}
bool dfs2(const vector<vector<int>>& adjlist, vector<int>& vis, int p, int u) {
    if (vis[u]) return false;
    vis[u] = 1;
    for (const auto v : adjlist[u]) {
        if (v == p) continue;
        if (!dfs2(adjlist, vis, u, v)) return false;
    }
    return true;
}
int main() {
    scanf("%d", &E);
    for (int i = 0; i < E; i++) {
        int d; scanf("%d", &d);
        while (d--) {
            int x; scanf("%d", &x); --x;
            if (x < 0 || x >= E || x == i) {
                puts("NO");
                return 0;
            }
            em[i][x] = 1;
        }
    }
    if (E == 1) {
        puts("YES");
        puts("1 2");
        return 0;
    }
    ends[0].first = 0;
    ends[0].second = 1;
    if (!dfs1(0) || nextv != E + 1 /* possible? */) {
        puts("NO");
        return 0;
    }
    // check for tree (not clear if necessary)
    vector<vector<int>> adjlist(E + 1);
    for (int i = 0; i < E; i++) {
        adjlist[ends[i].first].push_back(ends[i].second);
        adjlist[ends[i].second].push_back(ends[i].first);
    }
    vector<int> vis2(E + 1, 0);
    for (int i = 0; i < E + 1; i++) {
        if (!vis2[i]) {
            if (!dfs2(adjlist, vis2, -1, i)) { puts("NO"); return 0; }
        }
    }
    // consistency check
    for (int i = 1; i < E; i++) {
        if (ends[i].first == ends[i].second) { puts("NO"); return 0; }
        for (int j = 0; j < i; j++) {
            if (!em[i][j]) {
                if (ends[i].first == ends[j].first ||
                    ends[i].first == ends[j].second ||
                    ends[i].second == ends[j].first ||
                    ends[i].second == ends[j].second) {
                    puts("NO");
                    return 0;
                }
            } else {
                if (ends[i].first == ends[j].first) {
                    if (ends[i].second == ends[j].second) {
                        puts("NO");
                        return 0;
                    }
                } else if (ends[i].first == ends[j].second) {
                    if (ends[i].second == ends[j].first) {
                        puts("NO");
                        return 0;
                    }
                } else if (ends[i].second == ends[j].first) {
                    if (ends[i].first == ends[j].second) {
                        puts("NO");
                        return 0;
                    }
                } else if (ends[i].second == ends[j].second) {
                    if (ends[i].first == ends[j].first) {
                        puts("NO");
                        return 0;
                    }
                } else {
                    puts("NO");
                    return 0;
                }
            }
        }
    }
    puts("YES");
    for (int i = 0; i < E; i++) {
        printf("%d %d\n", ends[i].first + 1, ends[i].second + 1);
    }
}
