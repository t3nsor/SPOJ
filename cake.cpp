// 2024-10-11
// This looks like easy DP at first, but the state space is too large because of
// the constraint that both h and r can vary independently and must decrease
// while going up the cake. I don't know what the model solution is like.
// Recursion with memoization (i.e. to only visit the states we actually care
// about) is still too slow. This solution uses an additional trick. To find the
// optimal cake for a tuple (N, M, h, r) (where h, r are the max allowable
// height/radius at the current stage) we first check whether we've already
// computed the optimal cake for some (N, M, h1, r) where h1 > h. If that
// optimal cake uses h2 as the height of the current tranche, where h2 <= h,
// then that cake is optimal for this h as well. If this doesn't work, then
// instead look for the largest h3 < h and we resume the search from h3 + 1.
// Intuitively, it makes sense to use this optimization on the h dimension
// rather than the r dimension because the range of h is larger.
#include <cstring>
#include <iostream>
#include <string>
#include <map>
using namespace std;
int sqrt_table[10001];
struct MemoEntry {
    int hused;
    int result;
};
map<int, MemoEntry> memo[10001][11][101];
int rec(int N, int M, int h, int r) {
    if (N == 0) {
        return M == 0 ? 0 : 1e9;
    }
    auto& thismemo = memo[N][M][r];
    auto it = thismemo.lower_bound(h);
    int minh = M;
    int result = 1e9;
    int besth = h;
    if (it != thismemo.end()) {
        if (it->second.hused <= h) {
            // exact match
            return it->second.result;
        }
    }
    if (it != thismemo.begin()) {
        --it;
        minh = max(minh, it->first) + 1;
        besth = it->second.hused;
        result = it->second.result;
    }
    do {
        if (M == 1) {
            // special optimization for this case
            for (int i = 1; i <= h; i++) {
                if (N % i) continue;
                const int s = sqrt_table[N / i];
                if (s > 0 && s <= r && 2*i*s < result) {
                    result = 2*i*s;
                    besth = i;
                }
            }
            break;
        }
        // check for impossible cases
        int maxVol = 0;
        int minVol = 0;
        for (int i = 1; i <= M; i++) {
            maxVol += (h-i+1)*(r-i+1)*(r-i+1);
            minVol += i*i*i;
        }
        if (N > maxVol || N < minVol) break;
        for (int i = minh; i <= h; i++) {
            for (int j = M; j <= r; j++) {
                const int vol = i*j*j;
                if (vol <= N) {
                    const int r = 2*i*j + rec(N - vol, M - 1, i - 1, j - 1);
                    if (r < result) {
                        result = r;
                        besth = i;
                    }
                } else break;
            }
        }
    } while (false);
    MemoEntry entry;
    entry.hused = besth;
    entry.result = result;
    if (it != thismemo.begin() && result == it->second.result) {
        // This also seems to give a non-negligible performance improvement:
        // if we found out that going up to `h` didn't improve the result, then
        // adjust the existing entry (though we have to do this by deleting and
        // re-inserting it) instead of putting in an additional entry.
        thismemo.erase(it);
    }
    thismemo[h] = entry;
    return result;
}
int main() {
    for (int i = 1; i <= 100; i++) {
        sqrt_table[i*i] = i;
    }
    int T; cin >> T;
    while (T--) {
        int N, M; cin >> N >> M;
        int result = 1e9;
        for (int h = M; h <= N / 2; h++) {
            for (int r = M; h*r*r <= N; r++) {
                result = min(result,
                             r*r + 2*h*r + rec(N - h*r*r, M - 1, h - 1, r - 1));
            }
        }
        cout << result << '\n';
    }
}
