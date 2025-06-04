// 2025-06-03
#include <math.h>
#include <stdio.h>
#include <vector>
using namespace std;
double adj[1000][1000];
double calc_dist(double x1, double y1, double l1,
                 double x2, double y2, double l2) {
    if (l1 < 0) {
        return calc_dist(y1, x1, -l1, y2, x2, -l2);
    }
    if (l2 >= 0) {
        // both are west-east
        double x3 = x1 + l1;
        double x4 = x2 + l2;
        if (x1 >= x2 && x1 <= x4 || x2 >= x1 && x2 <= x3) {
            return fabs(y1 - y2);
        }
        return hypot(min(fabs(x1 - x4), fabs(x2 - x3)), y1 - y2);
    } else {
        // 1 is west-east, 2 is north-south
        double x3 = x1 + l1;
        double y3 = y2 - l2;
        double xdist = x2 >= x1 && x2 <= x3
                       ? 0
                       : min(fabs(x1 - x2), fabs(x3 - x2));
        double ydist = y1 >= y2 && y1 <= y3
                       ? 0
                       : min(fabs(y1 - y2), fabs(y1 - y3));
        return hypot(xdist, ydist);
    }
}
void do_testcase(int V) {
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            adj[i][j] = 1e6;
        }
    }
    vector<int> X(V);
    vector<int> Y(V);
    vector<int> L(V);
    for (int i = 0; i < V; i++) {
        scanf("%d %d %d", &X[i], &Y[i], &L[i]);
        for (int j = 0; j < i; j++) {
            adj[i][j] = adj[j][i] = calc_dist(X[i], Y[i], L[i],
                                              X[j], Y[j], L[j]);
        }
    }
    // run Dijkstra's
    vector<int> active(V, 0);
    vector<double> dist(V, 1e6);
    active[0] = 1;
    dist[0] = 0;
    for (;;) {
        int nexti = -1;
        double nextdist = 1e7;
        for (int i = 0; i < V; i++) {
            if (active[i] && dist[i] < nextdist) {
                nexti = i;
                nextdist = dist[i];
            }
        }
        if (nexti == 1) {
            printf("%.2f\n", dist[nexti]);
            return;
        }
        active[nexti] = 0;
        for (int j = 0; j < V; j++) {
            if (j == nexti) continue;
            double d = max(dist[nexti], adj[nexti][j]);
            if (d < dist[j]) {
                dist[j] = d;
                active[j] = 1;
            }
        }
    }
}
int main() {
    for (;;) {
        int V; scanf("%d", &V);
        if (V == 0) break;
        do_testcase(V);
    }
}
