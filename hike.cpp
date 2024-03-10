// 2024-03-09
#include <iostream>
#include <queue>
#include <string>
#include <string.h>
#include <tuple>
#include <utility>
#include <vector>
using namespace std;
void sort3(int& x, int& y, int& z) {
    if (x > y) swap(x, y);
    if (y > z) swap(y, z);
    if (x > y) swap(x, y);
}
void do_testcase(int V) {
    int p1, p2, p3; cin >> p1 >> p2 >> p3; --p1; --p2; --p3;
    sort3(p1, p2, p3);
    static int dist[50][50][50];
    memset(dist, -1, sizeof(dist));
    int color[50][50];
    vector<int> adj[50][26];
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            string c; cin >> c;
            color[i][j] = c[0] - 'a';
            adj[i][color[i][j]].push_back(j);
        }
    }
    if (p1 == p2 && p2 == p3) {
        cout << "0\n";
        return;
    }
    queue<tuple<int, int, int>> Q;
    Q.emplace(p1, p2, p3);
    dist[p1][p2][p3] = 0;
    while (!Q.empty()) {
        auto P = Q.front();
        Q.pop();
        const int p1 = get<0>(P);
        const int p2 = get<1>(P);
        const int p3 = get<2>(P);
        for (const auto v : adj[p1][color[p2][p3]]) {
            int q1 = v;
            int q2 = p2;
            int q3 = p3;
            sort3(q1, q2, q3);
            if (dist[q1][q2][q3] >= 0) continue;
            dist[q1][q2][q3] = dist[p1][p2][p3] + 1;
            if (q1 == q2 && q2 == q3) {
                cout << dist[q1][q2][q3] << '\n';
                return;
            }
            Q.emplace(q1, q2, q3);
        }
        for (const auto v : adj[p2][color[p3][p1]]) {
            int q1 = p1;
            int q2 = v;
            int q3 = p3;
            sort3(q1, q2, q3);
            if (dist[q1][q2][q3] >= 0) continue;
            dist[q1][q2][q3] = dist[p1][p2][p3] + 1;
            if (q1 == q2 && q2 == q3) {
                cout << dist[q1][q2][q3] << '\n';
                return;
            }
            Q.emplace(q1, q2, q3);
        }
        for (const auto v : adj[p3][color[p1][p2]]) {
            int q1 = p1;
            int q2 = p2;
            int q3 = v;
            sort3(q1, q2, q3);
            if (dist[q1][q2][q3] >= 0) continue;
            dist[q1][q2][q3] = dist[p1][p2][p3] + 1;
            if (q1 == q2 && q2 == q3) {
                cout << dist[q1][q2][q3] << '\n';
                return;
            }
            Q.emplace(q1, q2, q3);
        }
    }
    cout << "impossible\n";
}
int main() {
    ios::sync_with_stdio(false);
    int V;
    for (;;) {
        cin >> V; if (V == 0) return 0;
        do_testcase(V);
    }
}
