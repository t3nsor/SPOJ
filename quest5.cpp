// 2023-01-17
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
void do_testcase() {
    int n; cin >> n;
    vector<pair<int, int>> planks;
    for (int i = 0; i < n; i++) {
        int a, b; cin >> a >> b;
        planks.emplace_back(a, b);
    }
    sort(planks.begin(), planks.end());
    int result = 0;
    const int INF = 1e9;
    int end = INF;
    for (int i = 0; i < planks.size(); i++) {
        if (planks[i].first > end) {
            ++result;
            end = INF;
        }
        end = min(end, planks[i].second);
    }
    if (n > 0) ++result;
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
