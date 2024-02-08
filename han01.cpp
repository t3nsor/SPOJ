// 2024-02-27
#include <iostream>
#include <vector>
using namespace std;
void do_testcase() {
    int n; cin >> n;
    long long k; cin >> k;
    vector<int> loc(n);
    int src = 0, dest = 1;
    for (int cur_disk = n - 1; cur_disk >= 0; --cur_disk) {
        const int other = 3 - src - dest;
        if (k >= (1LL << cur_disk)) {
            k -= (1LL << cur_disk);
            loc[cur_disk] = dest;
            src = other;
        } else {
            loc[cur_disk] = src;
            dest = other;
        }
    }
    vector<int> pin[3];
    for (int i = 0; i < n; i++) {
        pin[loc[i]].push_back(i);
    }
    for (int i = 0; i < 3; i++) {
        cout << i + 1 << ": ";
        for (int j = pin[i].size() - 1; j >= 0; j--) {
            cout << pin[i][j] + 1;
            if (j > 0) cout << '|';
        }
        cout << '\n';
    }
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
