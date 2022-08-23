// 2022-08-22
#include <algorithm>
#include <iostream>
#include <map>
using namespace std;
void do_testcase() {
    int n, m; cin >> n >> m;
    // The structure of the map `dp` is as follows:
    // The entry (k, v) means that using `v` sorters, we can guarantee that the
    // largest value among the first `k` wires ends up on the `k`th wire. For
    // example, {{1, 0}, {4, 1}, {6, 2}} would mean that one sorter is 
    // sufficient for 2, 3, or 4 wires, while two sorters are needed for 5 or 6
    // wires. (Notice that dp[1] is always 0.)
    map<int, int> dp = {{1, 0}, {n + 1, 1000000000}};
    while (m--) {
        int i, j; cin >> i >> j;
        // the sorter (i, j) tells us to do:
        //     for (int k = i + 1; k <= j; k++) {
        //         a[k] = min(a[k], a[i] + 1);
        //     }
        // where `a` is the array represented compactly by `dp`.
        auto it = dp.lower_bound(i);
        const int new_val = it->second + 1;
        it = dp.lower_bound(j);
        if (it->second <= new_val) continue;
        dp[j] = new_val;
        // remove anything worse before it
        it = dp.find(j);
        for (it = dp.find(j);
             --it, it->second >= new_val;
             it = dp.erase(it));
    }
    cout << dp[n] << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
