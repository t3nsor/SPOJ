// 2026-04-16
// A Steiner tree for 4 nodes has up to 2 intermediate points.  Note that if the
// given regions were guaranteed to be disconnected from each other, we could
// assume that we always need to buy at least 2 parcels, so we could obtain a
// minimal solution by assuming that we pick exactly 2 intermediate points.
// However, I peeked at the test data before I started coding, and apparently
// they do NOT actually provide this guarantee, so we need to make sure we
// handle the cases of 0 and 1 intermediate points.  It IS guaranteed that each
// letter appears at least once (which is, I suppose, the reason why H is always
// at least 2).
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

constexpr int dr[6] = {1, 1, 0, -1, -1,  0};
constexpr int dc[6] = {0, 1, 1,  0, -1, -1};

char get() {
    char c;
    do { c = getchar(); } while (c <= 32);
    return c;
}

// return the distance between two hexes
int hdist(int r1, int c1, int r2, int c2) {
    int dr = r2 - r1;
    int dc = c2 - c1;
    if (dr <= 0 && dc <= 0) {
        dr = -dr; dc = -dc;
    }
    if (dr >= 0 && dc >= 0) return max(dr, dc);
    return abs(dr) + abs(dc);
}

template <class Graph>
int mst(int V, Graph graph) {
    int result = 0;
    bool vis[6] = {false};
    int dist[6]; memset(dist, 0x3f, sizeof(dist)); dist[0] = 0;
    int treecnt = 0;
    while (treecnt < V) {
        int bestv = -1, bestd = 10000;
        for (int i = 0; i < V; i++) {
            if (vis[i]) continue;
            if (dist[i] < bestd) { bestd = dist[i]; bestv = i; }
        }
        dist[bestv] = bestd;
        vis[bestv] = true;
        result += bestd;
        for (int i = 0; i < V; i++) {
            if (!vis[i]) dist[i] = min(dist[i], graph[bestv][i]);
        }
        ++treecnt;
    }
    return result;
}

int do_testcase(int H) {
    char grid[39][39] = {0};
    for (int i = 0; i < 2*H - 1; i++) {
        for (int j = max(0, i - H + 1); j < min(i + H, 2*H - 1); j++) {
            grid[i][j] = get();
        }
    }
    int dist[4][39][39];
    memset(dist, 0x3f, sizeof(dist));
    int graph[6][6];
    memset(graph, 0x3f, sizeof(graph));
    for (int r1 = 0; r1 < 2*H - 1; r1++) {
        for (int c1 = max(0, r1 - H + 1); c1 < min(r1 + H, 2*H - 1); c1++) {
            const int id1 = grid[r1][c1] - 'A';
            if (id1 < 0 || id1 >= 4) continue;
            for (int r2 = 0; r2 < 2*H - 1; r2++) {
                for (int c2 = max(0, r2 - H + 1);
                     c2 < min(r2 + H, 2*H - 1);
                     c2++) {
                    const int d = hdist(r1, c1, r2, c2);
                    dist[id1][r2][c2] = min(dist[id1][r2][c2], d);
                    const int id2 = grid[r2][c2] - 'A';
                    if (id2 >= 0 && id2 < 4) {
                        graph[id1][id2] = min(graph[id1][id2], max(d - 1, 0));
                    }
                }
            }
        }
    }
    graph[4][4] = graph[5][5] = 0;  // probably unnecessary
    int ans = mst(4, graph);
    for (int r1 = 0; r1 < 2*H - 1; r1++) {
        for (int c1 = max(0, r1 - H + 1); c1 < min(r1 + H, 2*H - 1); c1++) {
            if (grid[r1][c1] != '.') continue;
            // try with one intermediate node
            for (int i = 0; i < 4; i++) {
                graph[i][4] = graph[4][i] = dist[i][r1][c1] - 1;
            }
            ans = min(ans, 1 + mst(5, graph));
            for (int r2 = 0; r2 < 2*H - 1; r2++) {
                for (int c2 = max(0, r2 - H + 1);
                     c2 < min(r2 + H, 2*H - 1);
                     c2++) {
                    // try with two intermediate nodes
                    if (grid[r2][c2] != '.') continue;
                    if (make_pair(r1, c1) >= make_pair(r2, c2)) continue;
                    for (int i = 0; i < 4; i++) {
                        graph[i][5] = graph[5][i] = dist[i][r2][c2] - 1;
                    }
                    graph[4][5] = graph[5][4] = hdist(r1, c1, r2, c2) - 1;
                    ans = min(ans, mst(6, graph) + 2);
                }
            }
        }
    }
    return ans;
}

int main() {
    for (;;) {
        int H; scanf("%d", &H);
        if (H == 0) return 0;
        printf("You have to buy %d parcels.\n", do_testcase(H));
    }
}
