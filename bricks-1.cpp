// 2022-07-31
// This is a direct simulation approach, swapping each letter from the first
// string into where it needs to be, and updating the positions of all letters
// in the first string. The way to do this efficiently is using a BIT for each
// letter (a ... z), representing the difference arrays of the positions at
// which that letter is found in the first string. For a simpler approach,
// see bricks-2.cpp.
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
// BIT implementation taken from github.com/t3nsor/codebook
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
int main() {
    ios::sync_with_stdio(false);
    int T;  cin >> T;
    while (T--) {
        string a, b; cin >> a >> b;
        // index the strings from 1 to avoid any 0s in the difference array
        a.insert(a.begin(), 0);
        b.insert(b.begin(), 0);
        int m; cin >> m;
        vector<vector<int>> is_allowed(26, vector<int>(26, 0));
        while (m--) {
            string p; cin >> p;
            is_allowed[p[0] - 'a'][p[1] - 'a'] = 1;
            is_allowed[p[1] - 'a'][p[0] - 'a'] = 1;
        }
        const auto n = int(a.length()) - 1;
        if (n != (int)b.length() - 1) {
            cout << "-1\n"; continue;
        }
        vector<int> a_cnts(26, 0);
        vector<int> b_cnts(26, 0);
        for (int i = 1; i <= n; i++) {
            a[i] -= 'a';
            b[i] -= 'a';
            a_cnts[a[i]]++;
            b_cnts[b[i]]++;
        }
        if (!equal(begin(a_cnts), end(a_cnts), begin(b_cnts))) {
            cout << "-1\n"; continue;
        }
        vector<BIT> BITs;
        for (int i = 0; i < 26; i++) {
            BITs.emplace_back(a_cnts[i] + 1);
        }
        vector<int> last(26, 0);
        a_cnts.assign(26, 0);
        // fill BITS[i] with the differences in the indices of character i
        // e.g. if 'a' is found at positions 3, 4, 5, 8 then
        // BITS[0] will represent the array 3, 1, 1, 3
        for (int i = 1; i <= n; i++) {
            const auto c = a[i];
            BITs[c].update(a_cnts[c] + 1, i - last[c]);
            last[c] = i;
            ++a_cnts[c];
        }
        // insert a sentinel at position n + 1 (it's ok that all the
        // sentinels are on top of each other)
        for (int i = 0; i < 26; i++) {
            BITs[i].update(a_cnts[i] + 1, (n + 1) - last[i]);
        }
        long long result = 0;
        for (int i = 1; i <= n; i++) {
            const auto c = b[i];
            // We have to find the first occurrence of c in a[i] starting at
            // position i or later (the first i characters of `a` and `b`
            // already match at this point).
            const auto lb = BITs[c].lower_bound(i);
            // lb >= 2; f[1] + ... + f[lb-1] >= i
            const auto pos = BITs[c].read(lb);
            if (pos == i) continue;
            for (int c2 = 0; c2 < 26; c2++) {
                if (is_allowed[c][c2]) continue;
                // we are not allowed to swap c with c2, so check if there
                // are any c2s in [i, pos) (in the current `a`). To do this,
                // we find the first c2 at position i or later
                // (we could cache these positions and increment as necessary
                // instead of just doing BIT queries all the time, but t his
                // optimization is not necessary for AC)
                const auto lb2 = BITs[c2].lower_bound(i);
                if (BITs[c2].read(lb2) < pos) {
                    cout << "-1\n"; goto end;  // continue 3
                }
            }
            result += pos - i;
            // Now swap pos into i, and shift everything over.
            for (int c2 = 0; c2 < 26; c2++) {
                if (c == c2) {
                    // the c we need to move all the way to the left does not
                    // need to be swapped with any other copies of itself; we
                    // just have to adjust lb-1 and lb
                    BITs[c].update(lb-1, i - pos);
                    BITs[c].update(lb, pos - i);
                } else {
                    // all copies of this character in [i, pos) need to be
                    // shifted to the right
                    const auto lb2 = BITs[c2].lower_bound(i);
                    const auto lb3 = BITs[c2].lower_bound(pos);
                    BITs[c2].update(lb2-1, 1);
                    BITs[c2].update(lb3-1, -1);
                }
            }
        }
        cout << result % (1LL << 32) << '\n';
end:;
    }
}
