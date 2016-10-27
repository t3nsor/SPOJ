// 2016-10-26
#include <climits>
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;
bool ok(const vector<int>& a, int range, int days, int target) {
    vector<long long> ex(a.size());
    long long cur = 0;
    long long used = 0;
    for (int i = 0; i < a.size(); i++) {
        cur -= ex[i];
        long long increment = target - a[i] - cur;
        if (increment > 0) {
            used += increment;
            cur += increment;
            if (i + range < a.size()) {
                ex[i + range] += increment;
            }
        }
    }
    return used <= days;
}
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        int N, M, K; scanf("%d %d %d", &N, &M, &K);
        vector<int> a(N);
        for (int& x : a) scanf("%d", &x);
        if (M == 0) {
            // evil special case
            printf("%d\n", *min_element(a.begin(), a.end()));
        } else {
            int l = 0, r = 2000000000;
            while (r > l) {
                int m = l + (r - l + 1)/2;
                if (ok(a, M, K, m)) {
                    l = m;
                } else {
                    r = m - 1;
                }
            }
            printf("%d\n", l);
        }
    }
}
