// 2024-08-04
// An assumption we make is that if it's possible to partition the array into
// K_1 or K_2 subarrays, then it's also possible to partition the array into K
// subarrays for all K between K_1 and K_2. I don't know how to prove this,
// though :P
//
// To check feasibility for a given M, we use dynamic programming: for each
// prefix of the sequence, the min and max number of subarrays into which we can
// partition that prefix are equal to 1 plus, respectively, the best min and
// best max numbers of subarrays into which we can partition some smaller prefix
// such that the prefix sum of the larger prefix minus that of the smaller
// prefix is at most M. To compute this transition we use BITs indexed on the
// (compressed) prefix sum.
#include <algorithm>
#include <functional>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
constexpr int offset = 30000;
struct MinBIT {
    int n;
    vector<int> bit;
    MinBIT(int n) : n(n), bit(n + 1, 1e6) {}
    int query(int idx) {
        int result = 1e6;
        if (idx > n) idx = n;
        while (idx > 0) {
            result = min(result, bit[idx]);
            idx -= idx & -idx;
        }
        return result;
    }
    void update(int idx, int val) {
        while (idx <= n) {
            bit[idx] = min(bit[idx], val);
            idx += idx & -idx;
        }
    }
};
struct MaxBIT {
    int n;
    vector<int> bit;
    MaxBIT(int n) : n(n), bit(n + 1, -1e6) {}
    int query(int idx) {
        int result = -1e6;
        if (idx > n) idx = n;
        while (idx > 0) {
            result = max(result, bit[idx]);
            idx -= idx & -idx;
        }
        return result;
    }
    void update(int idx, int val) {
        while (idx <= n) {
            bit[idx] = max(bit[idx], val);
            idx += idx & -idx;
        }
    }
};
pair<int, int> need(int m, const vector<int>& a, const vector<int>& psum,
                    const vector<int>& spsum) {
    MinBIT min_bit(spsum.size());
    MaxBIT max_bit(spsum.size());
    const int zeropos = lower_bound(spsum.begin(), spsum.end(),
                                    0, greater<int>()) - spsum.begin();
    min_bit.update(zeropos + 1, 0);
    max_bit.update(zeropos + 1, 0);
    int result_min;
    int result_max;
    for (int i = 0; i < a.size(); i++) {
        const int s = psum[i + 1];
        const int j = upper_bound(spsum.begin(), spsum.end(),
                                  s - m, greater<int>()) - spsum.begin();
        if (j == 0) {
            result_min = 1e6;
            result_max = -1e6;
            continue;
        }
        result_min = 1 + min_bit.query(j);
        result_max = 1 + max_bit.query(j);
        const int spos = lower_bound(spsum.begin(), spsum.end(),
                                     s, greater<int>()) - spsum.begin();
        min_bit.update(spos + 1, result_min);
        max_bit.update(spos + 1, result_max);
    }
    return make_pair(result_min, result_max);
}
void do_testcase() {
    int n, k; scanf("%d %d", &n, &k);
    vector<int> a(n);
    vector<int> psum(n + 1);
    int negsum = 0, possum = 0;
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
        if (a[i] < 0) negsum += a[i];
        if (a[i] > 0) possum += a[i];
        psum[i + 1] = psum[i] + a[i];
    }
    vector<int> spsum = psum;
    sort(spsum.begin(), spsum.end(), greater<int>());
    spsum.erase(unique(spsum.begin(), spsum.end()), spsum.end());
    int l = negsum, r = possum;
    while (l < r) {
        int m = l + (r - l) / 2;
        const auto p = need(m, a, psum, spsum);
        if (k >= p.first && k <= p.second) {
            r = m;
        } else {
            l = m + 1;
        }
    }
    printf("%d\n", l);
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
