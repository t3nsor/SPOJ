// 2024-01-04
// This is an ideal application for A*. Let h[i][j] be the minimum number of
// moves required to swap a 0, which is initially at position `i`, into position
// `j`. Now notice that for each `x` other than 0, a move can decrease the value
// h[i][x] by at most 1, where `i` is the position of `x`. Furthermore each move
// can decrease this value for at most one `x`. This suggests that our heuristic
// should be the sum of h[i][x] for x = 1, ..., 11. This heuristic satisfies the
// consistency criterion, which results in good performance.
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>
using namespace std;
using ull = unsigned long long;
struct State {
    ull rep = 0;
    int get(int index) const {
        return (rep & (15ull << (4 * index))) >> (4 * index);
    }
    void set(int index, int value) {
        rep = rep & ~(15ull << (4 * index)) | (ull(value) << (4 * index));
    }
    struct Hash {
        size_t operator()(State s) const {
            return std::hash<ull>{}(s.rep);
        };
    };
};
bool operator==(State s1, State s2) {
    return s1.rep == s2.rep;
}
const int h[12][12] = {
    {0, 1, 2, 1, 2, 3, 1, 2, 3, 2, 3, 4},
    {1, 0, 1, 2, 1, 2, 2, 1, 2, 3, 2, 3},
    {2, 1, 0, 3, 2, 1, 3, 2, 1, 4, 3, 2},
    {1, 2, 3, 0, 1, 2, 2, 3, 4, 1, 2, 3},
    {2, 1, 2, 1, 0, 1, 3, 2, 3, 2, 1, 2},
    {3, 2, 1, 2, 1, 0, 4, 3, 2, 3, 2, 1},
    {1, 2, 3, 2, 3, 4, 0, 1, 2, 1, 2, 3},
    {2, 1, 2, 3, 2, 3, 1, 0, 1, 2, 1, 2},
    {3, 2, 1, 4, 3, 2, 2, 1, 0, 3, 2, 1},
    {2, 3, 4, 1, 2, 3, 1, 2, 3, 0, 1, 2},
    {3, 2, 3, 2, 1, 2, 2, 1, 2, 1, 0, 1},
    {4, 3, 2, 3, 2, 1, 3, 2, 1, 2, 1, 0}
};
const State dest = [] {
    State s;
    for (int i = 1; i <= 11; i++) {
        s.set(i, i);
    }
    return s;
}();
struct Entry {
    State s;
    int d;
    int h;
};
bool operator<(const Entry& e1, const Entry& e2) {
    return e2.d + e2.h < e1.d + e1.h;
}
int do_testcase() {
    Entry e;
    e.d = 0;
    e.h = 0;
    for (int i = 0; i < 12; i++) {
        int x; cin >> x;
        e.s.set(i, x);
        if (x != 0) e.h += h[x][i];
    }
    unordered_set<State, State::Hash> vis;
    priority_queue<Entry> Q;
    Q.push(e);
    for (;;) {
        Entry e = Q.top();
        Q.pop();
        if (e.s == dest) return e.d;
        if (!vis.insert(e.s).second) {
            // it was already there
            continue;
        }
        int zeropos = 0;
        while (e.s.get(zeropos) != 0) ++zeropos;
        for (int otherpos = 0; otherpos < 12; otherpos++) {
            if (h[zeropos][otherpos] != 1) continue;
            const int otherval = e.s.get(otherpos);
            Entry e2 = e;
            e2.s.set(zeropos, otherval);
            e2.s.set(otherpos, 0);
            e2.d++;
            e2.h += h[zeropos][otherval] - h[otherpos][otherval];
            Q.push(e2);
        }
    }
}
int main() {
    int t; cin >> t;
    while (t--) {
        cout << do_testcase() << '\n';
    }
}
