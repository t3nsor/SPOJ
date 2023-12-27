// 2023-12-26
// Standard coordinate compression + BIT approach. Note that we can optimize
// this by creating a compressed coordinate only for each *inserted* value, not
// for any arguments of D and C operations. But I'm too lazy.
// For a BBST-based approach, see orderset-2.cpp.
#include <algorithm>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
// Binary indexed tree supporting binary search.
struct BIT {
    int n;
    vector<int> bit;
    // BIT can be thought of as having entries f[1], ..., f[n]
    // which are 0-initialized
    BIT(int n):n(n), bit(n+1) {}
    // returns f[1] + ... + f[idx-1]
    // precondition idx <= n+1
    int read(int idx) {
        idx--;
        int res = 0;
        while (idx > 0) {
            res += bit[idx];
            idx -= idx & -idx;
        }
        return res;
    }
    // returns f[idx1] + ... + f[idx2-1]
    // precondition idx1 <= idx2 <= n+1
    int read2(int idx1, int idx2) {
        return read(idx2) - read(idx1);
    }
    // adds val to f[idx]
    // precondition 1 <= idx <= n (there is no element 0!)
    void update(int idx, int val) {
        while (idx <= n) {
            bit[idx] += val;
            idx += idx & -idx;
        }
    }
    // returns smallest positive idx such that read(idx) >= target
    int lower_bound(int target) {
        if (target <= 0) return 1;
        int pwr = 1; while (2*pwr <= n) pwr*=2;
        int idx = 0; int tot = 0;
        for (; pwr; pwr >>= 1) {
            if (idx+pwr > n) continue;
            if (tot + bit[idx+pwr] < target) {
                tot += bit[idx+=pwr];
            }
        }
        return idx+2;
    }
    // returns smallest positive idx such that read(idx) > target
    int upper_bound(int target) {
        if (target < 0) return 1;
        int pwr = 1; while (2*pwr <= n) pwr*=2;
        int idx = 0; int tot = 0;
        for (; pwr; pwr >>= 1) {
            if (idx+pwr > n) continue;
            if (tot + bit[idx+pwr] <= target) {
                tot += bit[idx+=pwr];
            }
        }
        return idx+2;
    }
};

char get_op() {
    char c;
    do {
        c = getchar_unlocked();
    } while (c <= 32);
    return c;
}

int main() {
    int Q; scanf("%d", &Q);
    vector<pair<int, int>> input(Q);
    vector<pair<int, int>> xs;
    for (int i = 0; i < Q; i++) {
        input[i].first = get_op();
        scanf("%d", &input[i].second);
        if (input[i].first != 'K') {
            xs.emplace_back(input[i].second, i);
        }
    }
    int xunique = 0;
    vector<int> xcompress(Q);
    vector<int> xuncompress;
    sort(xs.begin(), xs.end());
    for (int i = 0; i < xs.size(); i++) {
        if (i == 0 || xs[i].first > xs[i - 1].first) {
            xuncompress.push_back(xs[i].first);
            ++xunique;
        }
        xcompress[xs[i].second] = xunique - 1;
    }
    BIT bit(xunique + 1);
    vector<int> underlying(xunique, 0);
    int cnt = 0;
    for (int i = 0; i < Q; i++) {
        switch (input[i].first) {
          case 'I': {
            const int idx = xcompress[i];
            if (underlying[idx] == 0) {
                ++cnt;
                underlying[idx] = 1;
                bit.update(idx + 1, 1);
            }
          } break;
          case 'D': {
            const int idx = xcompress[i];
            if (underlying[idx] == 1) {
                --cnt;
                underlying[idx] = 0;
                bit.update(idx + 1, -1);
            }
          } break;
          case 'K': {
            if (input[i].second > cnt) {
                puts("invalid");
                continue;
            }
            const int idx = bit.lower_bound(input[i].second) - 2;
            printf("%d\n", xuncompress[idx]);
          } break;
          case 'C': {
            const int idx = xcompress[i];
            printf("%d\n", bit.read(idx + 1));
          } break;
        }
    }
}
