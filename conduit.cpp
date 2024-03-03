// 2024-03-02
#include <algorithm>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <utility>
#include <vector>
#include <unordered_map>
using namespace std;
using ll = long long;
struct Line {
    int A;
    int B;
    ll C;
};
bool operator==(Line l1, Line l2) {
    return l1.A == l2.A && l1.B == l2.B && l1.C == l2.C;
}
namespace std {
template <> struct hash<Line> {
    size_t operator()(Line l) const {
        static_assert(sizeof(Line) == 16);
        string s(16, 0);
        memcpy(&s[0], &l, 16);
        return hash<string>()(s);
    }
};
}
int count_components(const vector<pair<ll, ll>>& v) {
    // a begin sorts before an end at the same point
    vector<ll> v2;
    for (const auto& p : v) {
        v2.push_back(2*p.first);
        v2.push_back(2*p.second + 1);
    }
    sort(v2.begin(), v2.end());
    int result = 0;
    int depth = 0;
    for (const ll x : v2) {
        if (x%2 == 0) {
            if (0 == depth++) result++;
        } else {
            depth--;
        }
    }
    return result;
}
ll gcd(ll x, ll y) {
    if (x == 0) return y;
    return gcd(y % x, x);
}
Line canonicalize(Line l) {
    const ll g = gcd(abs(l.A), abs(l.B));
    l.A /= g;
    l.B /= g;
    l.C /= g;
    if (l.A < 0 || l.A == 0 && l.B < 0) {
        l.A *= -1;
        l.B *= -1;
        l.C *= -1;
    }
    return l;
}
void do_testcase(int n) {
    unordered_map<Line, vector<pair<ll, ll>>> M;
    for (int i = 0; i < n; i++) {
        double x1f, y1f, x2f, y2f; cin >> x1f >> y1f >> x2f >> y2f;
        const int x1 = 100*x1f + 0.5;
        const int y1 = 100*y1f + 0.5;
        const int x2 = 100*x2f + 0.5;
        const int y2 = 100*y2f + 0.5;
        const Line line =
          canonicalize({y1 - y2, x2 - x1, ll(x1)*y2 - ll(x2)*y1});
        ll begin = line.B*ll(x1) - line.A*ll(y1);
        ll end = line.B*ll(x2) - line.A*ll(y2);
        if (end < begin) swap(begin, end);
        M[line].emplace_back(begin, end);
    }
    int result = 0;
    for (const auto& kv : M) {
        result += count_components(kv.second);
    }
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int n;
    for (;;) {
        cin >> n; if (n == 0) return 0;
        do_testcase(n);
    }
}
