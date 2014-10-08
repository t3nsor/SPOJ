// 2014-04-30
#include <iostream>
#include <vector>
using namespace std;
struct BIT {
    int n;
    vector<int> bit;
    BIT(int n):n(n), bit(n+1) {}
    int read(int idx) {
        idx--;
        int res = 0;
        while (idx > 0) {
            res += bit[idx];
            idx -= idx & -idx;
        }
        return res;
    }
    int read2(int idx1, int idx2) {
        return read(idx2) - read(idx1);
    }
    void update(int idx, int val) {
        while (idx <= n) {
            bit[idx] += val;
            idx += idx & -idx;
        }
    }
    int lower_bound(int target) {
        if (target <= 0) return 1;
        int pwr = 1; while (2*pwr <= n) pwr *= 2;
        int idx = 0; int tot = 0;
        for (; pwr; pwr >>= 1) {
            if (idx+pwr > n) continue;
            if (tot + bit[idx+pwr] < target) {
                tot += bit[idx += pwr];
            }
        }
        return idx+2;
    }
    int upper_bound(int target) {
        if (target < 0) return 1;
        int pwr = 1; while (2*pwr <= n) pwr *= 2;
        int idx = 0; int tot = 0;
        for (; pwr; pwr >>= 1) {
            if (idx+pwr > n) continue;
            if (tot + bit[idx+pwr] <= target) {
                tot += bit[idx += pwr];
            }
        }
        return idx+2;
    }
};
int main() {
    for (;;) {
        int N, D; cin >> N >> D; if (N == 0) return 0;
        BIT bit(N);
        for (int i = 1; i <= N; i++) {
            bit.update(i, 1);
        }
        int target = 1;
        for (int i = N; i >= 1; i--) {
            target = (target + D-1) % i;
            if (target == 0) target = i;
            int pos = bit.lower_bound(target) - 1;
            bit.update(pos, -1);
            if (i == 1) {
                cout << N << ' ' << D << ' ' << pos << endl;
            }
        }
    }
}
