// 2024-09-01
//
// This is a pretty straightforward implementation problem; the problem
// statement is just a bit unclear. This solution assumes that:
// * No two terminals coincide.
// * Within-island walking times should *always* be rounded up. (The problem
//   statement appears to imply that if John arrives at a ferry at a non-integer
//   time, he has to wait until the next integer, at which time the ferry will
//   depart. But this solution also rounds up the time after e.g. John arrives
//   at the final island and then has to walk to the destination terminal, or
//   when the initial and final terminals are on the same island.)
// * Redundant intermediate points are allowed (i.e. points that lie on the line
//   segment between the point before and after).
// * Walking along the edge of a restricted area is allowed. Note that if it
//   were not allowed to walk along the edge of a restricted area, John could
//   still walk arbitrarily close to restricted areas as necessary to approach
//   the infimum walking time. But as user weathervane pointed out in the
//   comments, if two restricted areas could touch at one point, then there
//   would be the question of whether John could walk through that point. I
//   interpreted the problem statement like this: it says that restricted areas
//   are disjoint, so if they could touch at a corner, then it implies that the
//   corner isn't actually part of either restricted area (i.e. only the
//   interiors of the rectangles are restricted), which means John is allowed to
//   pass through such a point.
//
// It appears that sometimes a terminal can be located at the corner of a
// restricted area (my first attempt got MLE because it didn't take this
// possibility into account).
#include <iostream>
#include <math.h>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;
struct terminal {
    string name;
    int x;
    int y;
    int id;
};
struct area {
    int x1;
    int y1;
    int x2;
    int y2;
};
struct ferry {
    int t1;
    int t2;
    int time;
};
struct entry {
    int v;
    int p;
    double d;
    bool operator>(entry other) const {
        return d > other.d;
    }
};
bool intersectH(int x1, int y1, int x2, int y2,
                int X1, int X2, int Y) {
    if (Y <= min(y1, y2) || Y >= max(y1, y2)) return false;
    if (y1 == y2) return false;
    if (y2 < y1) {
        swap(x1, x2);
        swap(y1, y2);
    }
    return X1 * (y2 - y1) < x1 * (y2 - y1) + (Y - y1) * (x2 - x1) &&
           X2 * (y2 - y1) > x1 * (y2 - y1) + (Y - y1) * (x2 - x1);
}
bool intersectV(int x1, int y1, int x2, int y2,
                int X, int Y1, int Y2) {
    return intersectH(y1, x1, y2, x2, Y1, Y2, X);
}
void do_testcase() {
    int n; cin >> n;
    vector<string> itoname(n);
    unordered_map<string, int> nametoi;
    vector<int> X(n), Y(n);
    vector<vector<terminal>> iterm(n);
    vector<pair<int, int>> allterm;
    vector<vector<area>> areas(n);
    vector<vector<vector<pair<int, double>>>> sptree(n);
    int nterm = 0;
    for (int i = 0; i < n; i++) {
        cin >> itoname[i] >> X[i] >> Y[i];
        nametoi[itoname[i]] = i;
        int b; cin >> b; iterm[i].resize(b);
        for (int j = 0; j < b; j++) {
            cin >> iterm[i][j].name
                >> iterm[i][j].x
                >> iterm[i][j].y;
            iterm[i][j].id = nterm++;
            allterm.emplace_back(i, j);
        }
        int F; cin >> F; areas[i].resize(F);
        for (int j = 0; j < F; j++) {
            cin >> areas[i][j].x1 >> areas[i][j].y1
                >> areas[i][j].x2 >> areas[i][j].y2;
        }
        int V = b + 4*F;
        vector<vector<pair<int, double>>> adj(V);
        for (int j = 0; j < V; j++) {
            for (int k = 0; k < j; k++) {
                int x1, y1;
                int j2 = -1, k2 = -1;
                if (j < b) {
                    x1 = iterm[i][j].x;
                    y1 = iterm[i][j].y;
                } else {
                    j2 = j - b;
                    if (j2 % 4 == 0) {
                        x1 = areas[i][j2 / 4].x1;
                        y1 = areas[i][j2 / 4].y1;
                    } else if (j2 % 4 == 1) {
                        x1 = areas[i][j2 / 4].x1;
                        y1 = areas[i][j2 / 4].y2;
                    } else if (j2 % 4 == 2) {
                        x1 = areas[i][j2 / 4].x2;
                        y1 = areas[i][j2 / 4].y1;
                    } else {
                        x1 = areas[i][j2 / 4].x2;
                        y1 = areas[i][j2 / 4].y2;
                    }
                }
                int x2, y2;
                if (k < b) {
                    x2 = iterm[i][k].x;
                    y2 = iterm[i][k].y;
                } else {
                    k2 = k - b;
                    if (k2 % 4 == 0) {
                        x2 = areas[i][k2 / 4].x1;
                        y2 = areas[i][k2 / 4].y1;
                    } else if (k2 % 4 == 1) {
                        x2 = areas[i][k2 / 4].x1;
                        y2 = areas[i][k2 / 4].y2;
                    } else if (k2 % 4 == 2) {
                        x2 = areas[i][k2 / 4].x2;
                        y2 = areas[i][k2 / 4].y1;
                    } else {
                        x2 = areas[i][k2 / 4].x2;
                        y2 = areas[i][k2 / 4].y2;
                    }
                }
                if (x1 == x2 && y1 == y2) {
                    // terminal located at the corner of a field
                    continue;
                }
                if (j2 / 4 == k2 / 4 && (j2 % 4) + (k2 % 4) == 3) {
                    // diagonally cutting across a field
                    continue;
                }
                bool fail = false;
                for (int m = 0; m < 4*F; m++) {
                    const auto& area = areas[i][m / 4];
                    if (m % 4 == 0) {
                        if (intersectH(x1, y1, x2, y2,
                                       area.x1, area.x2, area.y1)) {
                            fail = true;
                            break;
                        }
                    } else if (m % 4 == 1) {
                        if (intersectV(x1, y1, x2, y2,
                                       area.x1, area.y1, area.y2)) {
                            fail = true;
                            break;
                        }
                    } else if (m % 4 == 2) {
                        if (intersectH(x1, y1, x2, y2,
                                       area.x1, area.x2, area.y2)) {
                            fail = true;
                            break;
                        }
                    } else {
                        if (intersectV(x1, y1, x2, y2,
                                       area.x2, area.y1, area.y2)) {
                            fail = true;
                            break;
                        }
                    }
                }
                if (!fail) {
                    adj[j].emplace_back(k, hypot(x1 - x2, y1 - y2));
                    adj[k].emplace_back(j, hypot(x1 - x2, y1 - y2));
                }
            }
        }
        // run Dijkstra's
        sptree[i].resize(b);
        for (int j = 0; j < b; j++) {
            auto& tr = sptree[i][j];
            tr.assign(V, make_pair(-1, 1e9));
            priority_queue<entry, vector<entry>, greater<entry>> Q;
            Q.push({j, -1, 0.0});
            while (!Q.empty()) {
                const auto e = Q.top();
                Q.pop();
                if (tr[e.v].second < e.d) continue;
                tr[e.v] = make_pair(e.p, e.d);
                for (const auto& edge : adj[e.v]) {
                    Q.push({edge.first, e.v, edge.second + e.d});
                }
            }
        }
    }
    int m; cin >> m;
    vector<vector<pair<int, double>>> adj(nterm);
    for (int i = 0; i < m; i++) {
        string tn1, in1, tn2, in2; cin >> tn1 >> in1 >> tn2 >> in2;
        const int isl1 = nametoi[in1];
        const int isl2 = nametoi[in2];
        int t1, t2;
        for (int j = 0; j < iterm[isl1].size(); j++) {
            if (iterm[isl1][j].name == tn1) {
                t1 = iterm[isl1][j].id;
                break;
            }
        }
        for (int j = 0; j < iterm[isl2].size(); j++) {
            if (iterm[isl2][j].name == tn2) {
                t2 = iterm[isl2][j].id;
                break;
            }
        }
        int time; cin >> time;
        adj[t1].emplace_back(t2, time);
        adj[t2].emplace_back(t1, time);
    }
    // add the within-island connections
    for (int isl = 0; isl < n; isl++) {
        for (int i = 0; i < iterm[isl].size(); i++) {
            for (int j = 0; j < i; j++) {
                const int u = iterm[isl][i].id;
                const int v = iterm[isl][j].id;
                const int t = ceil(sptree[isl][i][j].second);
                adj[u].emplace_back(v, t);
                adj[v].emplace_back(u, t);
            }
        }
    }
    string tn1, in1, tn2, in2; cin >> tn1 >> in1 >> tn2 >> in2;
    const int isl1 = nametoi[in1];
    const int isl2 = nametoi[in2];
    int t1, t2;
    for (int j = 0; j < iterm[isl1].size(); j++) {
        if (iterm[isl1][j].name == tn1) {
            t1 = iterm[isl1][j].id;
            break;
        }
    }
    for (int j = 0; j < iterm[isl2].size(); j++) {
        if (iterm[isl2][j].name == tn2) {
            t2 = iterm[isl2][j].id;
            break;
        }
    }
    vector<pair<int, double>> T(nterm, make_pair(-1, 1e9));
    priority_queue<entry, vector<entry>, greater<entry>> Q;
    Q.push({t1, -1, 0.0});
    for (;;) {
        const auto e = Q.top();
        Q.pop();
        if (T[e.v].second < e.d) continue;
        T[e.v] = make_pair(e.p, e.d);
        if (e.v == t2) break;
        for (const auto& edge : adj[e.v]) {
            Q.push({edge.first, e.v, edge.second + e.d});
        }
    }
    cout << int(ceil(T[t2].second)) << '\n';
    vector<string> path;
    path.push_back(tn2 + " " + in2);
    while (t2 != t1) {
        const int isl = allterm[t2].first;
        const int it = allterm[t2].second;
        const int nextid = T[t2].first;
        const int nextisl = allterm[nextid].first;
        const int nextit = allterm[nextid].second;
        if (isl == nextisl) {
            // retrace our path through the island...
            int p1 = it;
            const int p2 = nextit;
            for (;;) {
                const int nextp = sptree[isl][p2][p1].first;
                if (nextp == p2) break;
                if (nextp < iterm[isl].size()) {
                    // We should not mention any terminals we happen to pass
                    // through, they just happen to be on the line
                    p1 = nextp;
                    continue;
                }
                const int ap = nextp - iterm[isl].size();
                int x, y;
                if (ap % 4 == 0) {
                    x = areas[isl][ap / 4].x1;
                    y = areas[isl][ap / 4].y1;
                } else if (ap % 4 == 1) {
                    x = areas[isl][ap / 4].x1;
                    y = areas[isl][ap / 4].y2;
                } else if (ap % 4 == 2) {
                    x = areas[isl][ap / 4].x2;
                    y = areas[isl][ap / 4].y1;
                } else {
                    x = areas[isl][ap / 4].x2;
                    y = areas[isl][ap / 4].y2;
                }
                path.push_back(to_string(x) + " " + to_string(y));
                p1 = nextp;
            }
        }
        path.push_back(iterm[nextisl][nextit].name + " " + itoname[nextisl]);
        t2 = nextid;
    }
    for (int i = path.size() - 1; i >= 0; i--) {
        cout << path[i] << '\n';
    }
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    for (int i = 1; i <= t; i++) {
        cout << "case " << i << " Y\n";
        do_testcase();
        cout << '\n';
    }
}
