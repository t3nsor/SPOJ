// 2025-06-07
// According to a 2010 thread on the forum, "rooms do appear to overlap. Just
// pretend they don't." Seems like this was never fixed, so the obvious
// O(n log n) algorithm gets WA and we have to use the even more obvious O(n^2).
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;
long long calc(int w, int h) {
    const int s = min(w, h);
    const int m = w + h - 2*s;
    long long r1 = s;
    r1 = (r1 * (r1 + 1) * (2*r1 + 1)) / 6;
    long long r2 = s;
    r2 = (r2 * (r2 + 1)) / 2;
    return r1 + m*r2;
}
long long calc2(int o, int w1, int w2) {
    return calc(o, w1 + w2) - calc(o, w1) - calc(o, w2);
}
struct range {
    int begin;
    int end;
    int width;
};
int algo(const unordered_map<int, vector<range>>& left,
         const unordered_map<int, vector<range>>& right) {
    int result = 0;
    for (const auto& kv : left) {
        const int x = kv.first;
        const auto& lv = kv.second;
        const auto& rv = right.at(x);
        for (const auto& r1 : lv) {
            for (const auto& r2 : rv) {
                const int o1 = max(r1.begin, r2.begin);
                const int o2 = min(r1.end, r2.end);
                if (o2 - o1 >= 3) {
                    result += calc2(o2 - o1 - 2, r1.width, r2.width);
                }
            }
        }
    }
    return result;
}
void do_testcase(int cs, int N) {
    unordered_map<int, vector<range>> left, right, top, bottom;
    int result = 0;
    while (N--) {
        int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        left[x2].push_back({y1, y2, x2 - x1}); right[x2];
        right[x1].push_back({y1, y2, x2 - x1}); left[x1];
        bottom[y2].push_back({x1, x2, y2 - y1}); top[y2];
        top[y1].push_back({x1, x2, y2 - y1}); bottom[y1];
        result += calc(x2 - x1, y2 - y1);
    }
    result += algo(left, right);
    result += algo(bottom, top);
    cout << "Case " << cs << ": " << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    for (int cs = 1;; cs++) {
        int N; cin >> N;
        if (N == 0) break;
        do_testcase(cs, N);
    }
}
