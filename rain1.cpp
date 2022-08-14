// 2022-08-13
#include <algorithm>
#include <iostream>
#include <map>
#include <stack>
#include <vector>
using namespace std;
struct event {
    int x;
    int y;
    enum { BEGIN, END } type;
    int idx;
    friend bool operator<(event e1, event e2) {
        if (e1.x != e2.x) return e1.x < e2.x;
        if (e1.type == BEGIN) {
            return e2.type == END || e1.y < e2.y;
        } else {
            return e2.type == END && e1.y > e2.y;
        }
    }
};
struct segment {
    int x1;
    int y1;
    int x2;
    int y2;
};
void do_testcase() {
    int n; cin >> n;
    vector<segment> segments;
    vector<event> events;
    for (int i = 0; i < n; i++) {
        segment s;
        cin >> s.x1 >> s.y1 >> s.x2 >> s.y2;
        if (s.x1 > s.x2) {
            swap(s.x1, s.x2);
            swap(s.y1, s.y2);
        }
        segments.push_back(s);
        events.push_back(event{s.x1, s.y1, event::BEGIN, i});
        events.push_back(event{s.x2, s.y2, event::END, i});
    }
    sort(events.begin(), events.end());
    vector<int> result(n, 0);
    vector<int> edge(n, -1);
    vector<int> indeg(n, 0);
    // first, calculate the water that falls onto each segment from the sky
    int x = -1;  // current x-coordinate of sweep
    struct Comp {
        Comp(int& x) : x(x) {}
        bool operator()(segment s1, segment s2) const {
            // check which segment is higher at this point
            using ll = long long;
            const ll dx1 = s1.x2 - s1.x1;
            const ll dy1 = s1.y2 - s1.y1;
            const ll dx2 = s2.x2 - s2.x1;
            const ll dy2 = s2.y2 - s2.y1;
            // real y1 = s1.y1 + dy1 * (x - s1.x1) / dx1;
            // real y2 = s2.y1 + dy2 * (x - s2.x1) / dx2;
            // return y1 > y2;
            return (s1.y1 * dx1 * dx2 + dy1 * (x - s1.x1) * dx2) >
                   (s2.y1 * dx2 * dx1 + dy2 * (x - s2.x1) * dx1);
        }
        int& x;
    } comp(x);  // comparator for M
    // It's a bit weird that `comp.x` changes over the course of the sweep, but
    // this isn't undefined behaviour because it never changes the relative
    // ordering between two segments that are in the map at the same time (if
    // it did, it would mean those two segments intersect). An alternative
    // implementation involving a stateless comparator is as follows: given
    // two segments, choose some arbitrary x-coordinate that is common to both,
    // and do the y-coordinate comparison there (the choice of x-coordinate
    // can't affect the result). If they have no x-coordinate in common, then
    // return `false` (they are incomparable).
    map<segment, int, Comp> M(comp);  // segment -> its index in `segments`
    int last_x;
    for (event e : events) {
        last_x = x;
        x = e.x;
        if (last_x >= 0) {
            auto it = M.begin();
            if (it != M.end()) {
                // the segment at the current top catches the rain
                result[it->second] += (x - last_x);
            }
        }
        if (e.type == event::BEGIN) {
            auto r = M.emplace(segments[e.idx], e.idx);
            if (segments[e.idx].y1 < segments[e.idx].y2) {
                // check if there's anything after (i.e. immediately below) it 
                ++r.first;
                if (r.first != M.end()) {
                    edge[e.idx] = r.first->second;
                    indeg[r.first->second]++;
                }
            }
        } else {
            auto it = M.find(segments[e.idx]);
            if (segments[e.idx].y2 < segments[e.idx].y1) {
                // check if there's anything after (i.e. immediately below) it 
                ++it;
                if (it != M.end()) {
                    edge[e.idx] = it->second;
                    indeg[it->second]++;
                }
                --it;
            }
            M.erase(it);
        }
    }
    // topological sort
    stack<int> S;
    for (int i = 0; i < n; i++) if (indeg[i] == 0) S.push(i);
    while (!S.empty()) {
        const int i = S.top(); S.pop();
        if (edge[i] >= 0) {
            result[edge[i]] += result[i];
            if (0 == --indeg[edge[i]]) S.push(edge[i]);
        }
    }
    for (int i = 0; i < n; i++) {
        cout << result[i] << '\n';
    }
}
int main() {
    int t; cin >> t;
    while (t--) do_testcase();
}
