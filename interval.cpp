// 2023-01-28
//
// Idea: sweep from left to right, choose integers as late as possible. The
// "natural" time at which we would have to start selecting integers to satisfy
// the ith interval is b[i] - c[i] + 1. So first we find `i` that minimizes this
// time, and then select the integers b[i] - c[i] + 1, ..., b[i]. In doing so,
// we delay by c[i] the time for any other interval whose start point we have
// already seen. If we keep these times in a heap, then we can't actually go in
// and postpone them all by c[i] (since this would make our algorithm take
// quadratic time) but we instead remember to prepone all future insertions by
// c[i] in order to maintain the correct relative ordering. We can encapsulate
// this logic in a data structure to make it easier to use.
//
// Applied to the sample input, we would choose the following integers:
//     3 4 5 6 7           (3)
//               8 9 10    (3)
//           6 7 8         (1)
// 1 2 3                   (1)
//                   10 11 (1)
// ---------------------------
//     3     6 7 8 9 10
//
// We use the following convention: each integer (which we may or may not pick)
// is represented by a box, and time t means the left edge of the box that
// contains the integer t:
//
// +---+---+
// | 0 | 1 | ...
// +---+---+
// 0   1   2 ...
//
// Note that this solution always runs in O(n log n) time where n is the number
// of intervals. One of the official solutions is a bit simpler and uses the
// fact that the coordinates in the problem do not exceed 50,000. See file `b.c`
// in https://hsin.hr/cepc2002/zadaci/rjesenja.zip

#include <algorithm>
#include <cstdio>
#include <functional>
#include <queue>
#include <vector>
using namespace std;

template <class T>
struct postpone_heap {
    postpone_heap() = default;
    bool empty() const {
        return q_.empty();
    }
    T top() const {
        return postpone(q_.top(), offset_);
    }
    void pop() {
        q_.pop();
    }
    void push(const T& x) {
        q_.push(postpone(x, -offset_));
    }
    void postpone_all(int offset) {
        offset_ += offset;
    }

    int offset_ = 0;
    priority_queue<T, vector<T>, greater<T>> q_;
};

struct event {
    int time;
    int id;
    friend bool operator<(event e1, event e2) {
        return e1.time < e2.time;
    }
    friend bool operator>(event e1, event e2) {
        return e2 < e1;
    }
};

event postpone(event e, int offset) {
    return {e.time + offset, e.id};
}

void do_testcase() {
    int n; scanf("%d", &n);
    postpone_heap<event> H;
    vector<int> a(n);
    vector<int> b(n);
    vector<int> c(n);
    vector<event> v;
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &a[i], &b[i], &c[i]);
        v.push_back({a[i], i});
    }
    sort(v.begin(), v.end());
    int result = 0;
    int last = -1;
    for (int i = 0; i < v.size() || !H.empty();) {
        if (i < v.size() && (H.empty() || v[i].time < H.top().time)) {
            const int idx = v[i++].id;
            // We might have already chosen one or more integers that belong to
            // this interval; this means we don't have as many integers
            // remaining, and we can delay the time at which this interval
            // forces us to select all remaining integers it contains
            const int offset = max(0, last - a[idx] + 1);
            const event e = {b[idx] - c[idx] + 1 + offset, idx};
            H.push(e);
        } else {
            const auto e = H.top();
            H.pop();
            if (e.time > b[e.id]) continue;
            // According to our greedy approach, in order to satisfy this
            // interval, we must choose all integers from the current time to
            // the last value in the interval:
            const int dist = b[e.id] - e.time + 1;
            result += dist;
            H.postpone_all(dist);
            last = b[e.id];
        }
    }
    printf("%d\n", result);
}
int main() {
    int t; scanf("%d", &t);
    while (t--) {
        do_testcase();
    }
}
