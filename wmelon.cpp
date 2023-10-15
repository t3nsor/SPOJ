// 2023-10-15
// This problem is pretty easy once you realize that an O(n^3) time solution can
// pass. In order to avoid doing 1000^3 operations per test case, we use
// coordinate compression (I hate it when problems force me to optimize smaller
// inputs by bundling lots of test cases into a single file).
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
void do_testcase() {
    int n, K; cin >> n >> K;
    vector<int> xs, ys;
    vector<int> x(n);
    vector<int> y(n);
    vector<int> cnt(n);
    for (int i = 0; i < n; i++) {
        cin >> x[i] >> y[i] >> cnt[i];
        xs.push_back(x[i]);
        ys.push_back(y[i]);
    }
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    vector<vector<int>> a(xs.size(), vector<int>(ys.size()));
    for (int i = 0; i < n; i++) {
        const int xi = lower_bound(xs.begin(), xs.end(), x[i]) - xs.begin();
        const int yi = lower_bound(ys.begin(), ys.end(), y[i]) - ys.begin();
        a[xi][yi] += cnt[i];
    }
    int best = 1e9;
    for (int i = 0; i < xs.size(); i++) {
        vector<int> b(ys.size());
        for (int j = i; j < xs.size(); j++) {
            const int width = xs[j] - xs[i];
            for (int k = 0; k < ys.size(); k++) {
                b[k] += a[j][k];
            }
            int m = 0;
            int v = 0;
            for (int k = 0; k < ys.size(); k++) {
                v += b[k];
                while (v - b[m] >= K) {
                    v -= b[m];
                    ++m;
                }
                if (v >= K) best = min(best, width * (ys[k] - ys[m]));
            }
        }
    }
    cout << best << '\n';
}
int main() {
    int t; cin >> t;
    while (t--) do_testcase();
}
