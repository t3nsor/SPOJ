// 2024-02-07
// Apparently the greedy solution works for this problem (repeatedly find the
// peak that would be cheapest to remove, then remove it) but I don't know how
// to prove it.
#include <stdio.h>
#include <vector>
using namespace std;
int do_testcase() {
    int N, K; scanf("%d %d", &N, &K);
    vector<int> a(N + 2);
    a[0] = a[N + 1] = 0;
    for (int i = 1; i <= N; i++) scanf("%d", &a[i]);
    int dirt_removed = 0;
    for (;;) {
        vector<pair<int, int>> peaks;  // half-open interval
        {
            int i = 1;
            // scan for equal ranges
            while (i <= N) {
                int j = i + 1;
                while (a[j] == a[i]) {
                    ++j;
                }
                if (a[i] > a[i - 1] && a[j - 1] > a[j]) {
                    peaks.emplace_back(i, j);
                }
                i = j;
            }
        }
        if (peaks.size() <= K) return dirt_removed;
        vector<pair<int, int>> limits;
        int cheapest_peak;
        int lowest_cost = 2e9;
        for (const auto& peak : peaks) {
            // check how far to the left we have to go to remove this peak, and
            // same for the right
            int i = peak.first - 1;
            while (i >= 1 && a[i] >= a[i - 1]) i--;
            int j = peak.second;
            while (j <= N && a[j] >= a[j + 1]) j++;
            const int height_after = max(a[i], a[j]);
            int cost = 0;
            for (int k = i; k <= j; k++) {
                cost += max(0, a[k] - height_after);
            }
            if (cost < lowest_cost) {
                lowest_cost = cost;
                cheapest_peak = limits.size();
            }
            limits.emplace_back(i, j);
        }
        // remove the cheapest peak
        const int i = limits[cheapest_peak].first;
        const int j = limits[cheapest_peak].second;
        const int height_after = max(a[i], a[j]);
        dirt_removed += lowest_cost;
        for (int k = i; k <= j; k++) {
            a[k] = min(a[k], height_after);
        }
    }
}
int main() {
    int t; scanf("%d", &t);
    while (t--) {
        printf("%d\n", do_testcase());
    }
}
