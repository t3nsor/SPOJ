// 2025-06-04
// This could probably be optimized a bit by not calculating the beginning and
// end points of transmission within the same pairs of segments multiple times.
#include <algorithm>
#include <iostream>
#include <math.h>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
struct Segment {
    int tbegin;
    int tend;
    int xbegin;
    int ybegin;
    int vx;
    int vy;
};
enum EventKind {
    XMIT_BEGIN,
    XMIT_END,
};
struct Event {
    double time;
    EventKind kind;
    int r1, r2;  // the IDs of the robots involved
    friend bool operator<(const Event& e1, const Event& e2) {
        // begin sorts before end so that in case of a tangent we get the right
        // answer
        return e1.time < e2.time ||
               (e1.time == e2.time &&
                e1.kind == XMIT_BEGIN &&
                e2.kind == XMIT_END);
    }
};
void dfs(int N, bool (&have)[100], bool (&adj)[100][100], int cur) {
    if (have[cur]) return;
    have[cur] = true;
    for (int i = 0; i < N; i++) {
        if (adj[cur][i]) {
            dfs(N, have, adj, i);
        }
    }
}
void add_events(int R, const bool (&adj)[100][100], vector<Event>& events,
                int t1, int t2, int id1, int id2,
                const Segment& s1, const Segment& s2) {
    // |rx|, |ry| <= 1000
    int rx = (s2.xbegin + s2.vx * (t1 - s2.tbegin)) -
             (s1.xbegin + s1.vx * (t1 - s1.tbegin)); 
    int ry = (s2.ybegin + s2.vy * (t1 - s2.tbegin)) -
             (s1.ybegin + s1.vy * (t1 - s1.tbegin)); 
    // |rvx|, |rvy| <= 20
    int rvx = s2.vx - s1.vx;
    int rvy = s2.vy - s1.vy;
    int dt = t2 - t1;  // 0 <= dt <= 1000
    // 0 <= a <= 800
    int a = rvx*rvx + rvy*rvy;
    // |b| <= 8e4
    int b = 2*rx*rvx + 2*ry*rvy;
    // |c| <= 2e6
    int c = rx*rx + ry*ry - R*R;
    long long disc = b*(long long)b - 4*a*c;
    if (disc < 0 || a == 0) return;  // no events
    // at^2 + bt + c = 0 -> intersection
    double it1 = (-b - sqrt(disc))/(2.0*a);
    double it2 = (-b + sqrt(disc))/(2.0*a);
    // Check for exact matches. Could be paranoia given that the problem
    // description promises that certain close calls won't happen.
    if (c == 0) {
        if (a*dt*dt + b*dt + c == 0) {
            // both the beginning and end of this interval are intersection
            // points, but we don't count the beginning (since it was processed
            // at the end of the last interval)
            it1 = -1e6;
            it2 = dt;
        } else {
            // the beginning of this interval is an intersection point, we need
            // to check whether it1 or it2 is zero
            if (b >= 0) {
                it2 = -1e6;
            }
            if (b <= 0) {
                it1 = -1e6;
            }
        }
    } else if (a*dt*dt + b*dt + c == 0) {
        // the end of this interval is an intersection point, we need to check
        // whether it1 or it2 is dt
        int d = 2*a*dt + b;
        if (d >= 0 && d*(long long)d == disc) {
            it2 = dt;
        }
        if (d <= 0 && d*(long long)d == disc) {
            it1 = dt;
        }
    }
    // tangent, make sure to leave no gap
    if (0 == disc) {
        double avg = (it1 + it2) / 2;
        it1 = it2 = avg;
    }
    double it[2] = {min(it1, it2), max(it1, it2)};
    bool curadj = adj[id1][id2];
    for (int i = 0; i < 2; i++) {
        if (it[i] >= 0 && it[i] <= dt) {
            events.push_back({it[i] + t1, curadj ? XMIT_END : XMIT_BEGIN,
                              id1, id2});
            curadj = !curadj;
        }
    }
}
void do_testcase(int N, int T, int R) {
    bool have[100] = {0};
    vector<vector<Segment>> route(N);
    unordered_map<string, int> id;
    vector<string> nickname(N);
    for (int i = 0; i < N; i++) {
        string n; cin >> n;
        id[n] = i; 
        nickname[i] = n;
        int t = 0, x, y;
        int t0; cin >> t0 >> x >> y;  // t0 is 0
        for (;;) {
            Segment segment;
            segment.tbegin = t;
            segment.xbegin = x;
            segment.ybegin = y;
            cin >> t;
            segment.tend = t;
            cin >> segment.vx >> segment.vy;
            route[i].push_back(segment);
            x += segment.vx * (segment.tend - segment.tbegin);
            y += segment.vy * (segment.tend - segment.tbegin);
            if (t == T) break;
        }
    }
    bool adj[100][100] = {{0}};
    for (int i = 0; i < N; i++) {
        int x1 = route[i][0].xbegin;
        int y1 = route[i][0].ybegin;
        for (int j = 0; j < i; j++) {
            int x2 = route[j][0].xbegin;
            int y2 = route[j][0].ybegin;
            if (hypot(x1 - x2, y1 - y2) <= R) {
                adj[i][j] = adj[j][i] = true;
            }
        }
    }
    dfs(N, have, adj, 0);
    vector<int> curseg(N, 0);
    int t = 0;
    while (t < T) {
        // find the next time at which some robot changes direction
        int nextt = 10000;
        for (int i = 0; i < N; i++) {
            nextt = min(nextt, route[i][curseg[i]].tend);
        }
        // find all events that will happen from `t` until `nextt`
        vector<Event> events;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < i; j++) {
                add_events(R, adj, events, t, nextt, i, j,
                           route[i][curseg[i]],
                           route[j][curseg[j]]);
            }
        }
        sort(events.begin(), events.end());
        for (const auto& event : events) {
            if (event.kind == XMIT_BEGIN) {
                if (adj[event.r1][event.r2]) throw;
                adj[event.r1][event.r2] = adj[event.r2][event.r1] = true;
                if (have[event.r1] && !have[event.r2]) {
                    dfs(N, have, adj, event.r2);
                } else if (!have[event.r1] && have[event.r2]) {
                    dfs(N, have, adj, event.r1);
                }
            } else {
                if (!adj[event.r1][event.r2]) throw;
                adj[event.r1][event.r2] = adj[event.r2][event.r1] = false;
            }
        }
        for (int i = 0; i < N; i++) {
            if (route[i][curseg[i]].tend == nextt) ++curseg[i];
        }
        t = nextt;
    }
    vector<string> result;
    for (int i = 0; i < N; i++) {
        if (have[i]) result.push_back(nickname[i]);
    }
    sort(result.begin(), result.end());
    for (const auto& n : result) cout << n << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    for (;;) {
        int N, T, R; cin >> N >> T >> R;
        if (N == 0) break;
        do_testcase(N, T, R);
    }
}
