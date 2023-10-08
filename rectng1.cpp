// 2023-10-07
// The comments on the problem indicate that an O(N^2) solution can pass, but
// it's a good exercise to solve it in O(N log N) (rather than fiddling around
// to try to improve the constant factor). The classic algorithm for finding the
// connected components of the union of axis-aligned rectangles was given by
// L. Guibas and J. Saxe in 1983. The first step is to sweep a vertical line
// from left to right while maintaining two data structures that will be used to
// generate a list of graph edges that span the connected components; a trivial
// second step then computes the transitive closure. We will find it
// appropriate to discretize the plane, i.e., divide the plane into unit
// squares and label each square with a pair of integer coordinates, and
// transform each input rectangle into a set of unit squares. We want two such
// transformed rectangles to have a square in common only if the original
// rectangles have more than a single point in common. This can be done using
// the following transformation:
// <x1, y1, x2, y2> -> ([2x1, 2x2] x [y1, y2]) - {(2x1, y1), (2x2, y1)}
// For example, an input rectangle <0, 0, 2, 2> would be represented like this:
//
// 2*****
// 1*****
// 0 ***
//  01234
//
// This, for every one input rectangle, we actually generate two discrete
// rectangles, one of which always has height 1.
//
// The first step of the Guibas--Saxe algorithm uses one data structure that
// holds a collection of intervals representing the intersection of the sweep
// line with the rectangle set as is standard for this type of problem. Call
// this the "active set". The second data structure is called the "illuminator
// tree". For a point P on the sweep line, imagine that there is a ray cast
// leftward from P (i.e. toward decreasing x-coordinate). The illuminator of P
// is the rightmost left edge of all rectangles that intersect the ray. The
// illuminator tree represents the partition of the sweep line into a
// collection of intervals where all points in an interval have the same
// illuminator. When the sweep line reaches the left edge of a rectangle, the
// illuminator tree is updated to account for the fact that this new left edge
// is now the illuminator of its entire interval.  For example suppose the
// configuration of the illuminator tree is as follows:
//
// 7: 1
// 6: 2
// 5: 2
// 4: 3
// 3: 3
// 2: 2
// 1: 1
// 0: 1
//
// This illuminator tree consists of 5 separate intervals: the interval [0, 1]
// is mapped to 1, [2, 2] is mapped to 2, [3, 4] is mapped to 3, [5, 6] is
// mapped to 2, and [7, 7] is mapped to 1. If rectangle 4 is to be added, whose
// left edge is the interval [1, 5], the illuminator tree must be updated to
// look like this:
//
// 7: 1
// 6: 2
// 5: 4
// 4: 4
// 3: 4
// 2: 4
// 1: 4
// 0: 1
//
// The new configuration of the illuminator tree will contain only 4 intervals.
// When updating the illuminator tree, we will need to split the interval [5, 6]
// into two intervals [5, 5] and [6, 6], and update the illuminator of the
// former interval to 4, combining it with the rest of the new interval for 4.
// Similarly, the interval [0, 1] will need to be split, and [1, 1] combined
// with the rest of the new interval. Finally, the entirety of the intervals
// [2, 2] and [3, 4] will need to be remapped to 4. Consider these four
// subintervals, namely [1, 1], [2, 2], [3, 4], and [5, 5], that will make up
// the new interval, and their respective current illuminators (namely 1, 2, 3,
// and 2). For each such subinterval `I`, let `i` denote its current
// illuminator. Determine whether `I` intersects any interval that is in
// the active set (i.e., whether `I`, regarded as a line segment of the sweep
// line, intersects any rectangle before rectangle 4). If there is at least one
// such rectangle, then a graph edge is added between `i` and the new rectangle
// (rectangle 4). Note that we don't care *which* rectangles intersect `I`;
// only whether there are any. Finally, after these graph edges have been
// added, then both data structures are updated.
//
// There are various different options for representing the active set; the one
// we use in this implementation is the binary indexed tree with range updates
// (https://github.com/t3nsor/codebook/blob/master/BIT-range.cpp): when a
// rectangle is added, the range of y-coordinates that it occupies is updated by
// +1 (and similarly -1 on removal), and if the result of a query for a
// particular interval is nonzero, then it means the interval intersects at
// least one rectangle. (We'll have to increment all y-coordinates by 1 since
// the BIT is 1-based.) For the illuminator tree, a `std::map` is the obvious
// choice.

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <tuple>
#include <vector>
using namespace std;

struct IlluminatedInterval {
    int begin;
    int end;  // half-open
    int id;
};

struct IlluminatorTree {
    // The illuminator tree starts with a "dummy" interval that illuminates the
    // entire sweep line, indicated by an ID of -1. Since all coordinates in the
    // problem are nonnegative, the lower bound of the dummy interval can be 0.
    map<int, int> data = {{0, -1}};

    // Returns all subintervals that constitute the new interval, as described
    // above.
    vector<IlluminatedInterval> update(int begin, int end, int id) {
        vector<IlluminatedInterval> result;
        if (end == begin) return result;
        auto it = prev(data.upper_bound(begin));
        if (it->first < begin) {
            data[begin] = it->second;
        }
        it = prev(data.upper_bound(end));
        if (it->first < end) {
            data[end] = it->second;
        }
        it = data.find(begin);
        while (it->first != end) {
            auto next_it = next(it);
            if (it->second >= 0) {
                result.push_back({it->first, next_it->first, it->second});
            }
            it->second = id;
            it = next_it;
        }
        return result;
    }
};

struct BIT {
    int n;
    vector<int> slope;
    vector<long long> intercept;
    // BIT can be thought of as having entries f[1], ..., f[n]
    // which are 0-initialized
    BIT(int n): n(n), slope(n+1), intercept(n+1) {}
    // returns f[1] + ... + f[idx-1]
    // precondition idx <= n+1
    int query(int idx) {
        int m = 0; long long b = 0;
        for (int i = idx-1; i > 0; i -= i&-i) {
            m += slope[i];
            b += intercept[i];
        }
        return m*idx + b;
    }
    // adds amt to f[i] for i in [idx1, idx2)
    // precondition 1 <= idx1 <= idx2 <= n+1 (you can't update element 0)
    void update(int idx1, int idx2, int amt) {
        for (int i = idx1; i <= n; i += i&-i) {
            slope[i] += amt;
            intercept[i] -= idx1*amt;
        }
        for (int i = idx2; i <= n; i += i&-i) {
            slope[i] -= amt;
            intercept[i] += idx2*amt;
        }
    }
};

struct Event {
    int x;
    enum EventType {
        END,
        BEGIN
    } type;
    int id;
};
bool operator<(Event e1, Event e2) {
    return tie(e1.x, e1.type, e1.id) < tie(e2.x, e2.type, e2.id);
}

void dfs(const vector<vector<int>>& adj, int v, vector<int>& visited) {
    visited[v] = true;
    for (const auto w : adj[v]) {
        if (!visited[w]) {
            dfs(adj, w, visited);
        }
    }
}

void do_testcase() {
    // Other than the implementations of the data structures, all the logic is
    // in this function. The algorithm is hard to explain, but it's not actually
    // very complicated!
    BIT active(10001);
    IlluminatorTree ill;
    int n; cin >> n;
    vector<int> X1, Y1, X2, Y2;
    for (int i = 0; i < n; i++) {
        int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
        x1 *= 2; x2 *= 2; y1++; y2++;
        X1.push_back(x1);
        X2.push_back(x2 + 1);
        Y1.push_back(y1 + 1);
        Y2.push_back(y2 + 1);
        X1.push_back(x1 + 1);
        X2.push_back(x2);
        Y1.push_back(y1);
        Y2.push_back(y1 + 1);
    }
    vector<Event> events;
    for (int i = 0; i < X1.size(); i++) {
        events.push_back({X1[i], Event::BEGIN, i});
        events.push_back({X2[i], Event::END, i});
    }
    sort(events.begin(), events.end());
    vector<vector<int>> adj(n);
    for (const auto e : events) {
        if (e.type == Event::BEGIN) {
            const auto subintervals = ill.update(Y1[e.id], Y2[e.id], e.id);
            for (const auto I : subintervals) {
                if (active.query(I.end) > active.query(I.begin)) {
                    adj[e.id / 2].push_back(I.id / 2);
                    adj[I.id / 2].push_back(e.id / 2);
                }
            }
            active.update(Y1[e.id], Y2[e.id], 1);
        } else {
            active.update(Y1[e.id], Y2[e.id], -1);
        }
    }
    vector<int> visited(n);
    int ccnt = 0;
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            ++ccnt;
            dfs(adj, i, visited);
        }
    }
    cout << ccnt << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
