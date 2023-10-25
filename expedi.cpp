// 2023-10-24
#include <algorithm>
#include <iostream>
#include <utility>
#include <queue>
#include <vector>
using namespace std;
void do_testcase() {
    int N; cin >> N;
    vector<pair<int, int>> stops;
    while (N--) {
        int pos, fuel; cin >> pos >> fuel;
        stops.emplace_back(pos, fuel);
    }
    int L, P; cin >> L >> P;
    for (auto& p : stops) p.first = L - p.first;
    sort(stops.begin(), stops.end());
    int x = 0;
    priority_queue<int> Q;
    int idx = 0;
    int result = 0;
    for (;;) {
        if (x + P >= L) {
            cout << result << '\n';
            return;
        }
        while (idx < stops.size() && stops[idx].first <= x + P) {
            Q.push(stops[idx].second);
            ++idx;
        }
        x += P;
        if (Q.empty()) {
            cout << "-1\n";
            return;
        }
        P = Q.top();
        Q.pop();
        ++result;
    }
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
