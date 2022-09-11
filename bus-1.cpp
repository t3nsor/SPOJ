// 2022-08-30
// Direct simulation approach using bit vectors
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;
constexpr int MAX_ITERATIONS = 48 * 49 * 48;
void do_testcase() {
    int n; cin >> n;
    if (n == 0) exit(0);
    vector<vector<int>> routes(n);
    vector<int> all_stop_numbers;
    for (int i = 0; i < n; i++) {
        int s; cin >> s;
        while (s--) {
            int x; cin >> x;
            routes[i].push_back(x);
            all_stop_numbers.push_back(x);
        }
    }
    sort(all_stop_numbers.begin(), all_stop_numbers.end());
    for (auto& route : routes) {
        for (auto& s : route) {
            const auto it = lower_bound(all_stop_numbers.begin(),
                                        all_stop_numbers.end(),
                                        s);
            s = it - all_stop_numbers.begin();
        }
    }
    
    using ll = unsigned long long;
    const ll target = (1ULL << n) - 1;
    int knows_all = 0;
    vector<ll> knows(n);
    for (int i = 0; i < n; i++) {
        knows[i] = 1ULL << i;
    }
    int current_time = 0;
    for (; current_time < MAX_ITERATIONS; ++current_time) {
        unordered_map<int, vector<int>> who;
        for (int i = 0; i < n; i++) {
            const int pos = current_time % routes[i].size();
            who[routes[i][pos]].push_back(i);
        }
        for (const auto& kv : who) {
            const auto& list = kv.second;
            ll mask = 0;
            for (int j = 0; j < list.size(); j++) {
                mask |= knows[list[j]];
            }
            for (int j = 0; j < list.size(); j++) {
                if (knows[list[j]] != target) {
                    if (target == (knows[list[j]] = mask)) {
                        ++knows_all;
                    }
                }
            }
        }
        if (knows_all == n) break;
    }
    if (knows_all == n) {
        cout << current_time << '\n';
    } else {
        cout << "NEVER\n";
    }
}
int main() {
    ios::sync_with_stdio(false);
    for (;;) {
        do_testcase();
    }
}
