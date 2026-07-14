// 2026-07-14
// The naive O(N*2^M) DP doesn't pass, of course (I actually tried it, in case
// the test data were weak).  The solution is based on the principle of
// inclusion and exclusion: to find the number of combinations of sets whose
// union is the whole universe, we start with the number of total combinations
// (2^N), then for each item we subtract the number of combinations of sets
// whose union doesn't include that item (which is 2 to the power of the number
// of sets that don't contain that item), then to undo double-counting, for each
// subset of size 2, we add the number of combinations of sets whose union
// doesn't include either of the two elements, and so on.
//
// We thus reduce the problem to this one: for each subset S of the universe,
// compute the number of input sets that are a subset of S.  (Then, the number
// of input sets that are disjoint from a given set T is just the number of
// input sets that are a subset of the complement of T.)  How to do this in
// O(N + M*2^M) time is left as an exercise for the reader.
#include <stdio.h>
#include <vector>
using namespace std;
constexpr int MOD = 1000000007;
int pow2[1000001];
int get() {
    int c;
    do {
        c = getchar_unlocked();
    } while (c <= 32);
    int result = 0;
    do {
        result = 10*result + c - '0';
        c = getchar_unlocked();
    } while (c > 32);
    return result;
}
int main() {
    pow2[0] = 1;
    int N = get(), M = get();
    for (int i = 1; i <= N; i++) {
        pow2[i] = 2*pow2[i - 1];
        if (pow2[i] >= MOD) pow2[i] -= MOD;
    }
    // a[i] is initially the number of boxes whose toys are exactly the mask
    // `i`, and will later become the number of boxes whose toys form a subset
    // of the mask `i`
    vector<int> a(1 << M);
    while (N--) {
        int K = get();
        int mask = 0;
        while (K--) {
            int toy = get(); --toy;
            mask |= (1 << toy);
        }
        a[mask]++;
    }
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < (1 << M); j++) {
            if (j & (1 << i)) continue;
            a[j + (1 << i)] += a[j];
        }
    }
    int result = 0;
    for (int i = 0; i < (1 << M); i++) {
        int pc = __builtin_popcount(i);
        int term = pow2[a[(1 << M) - i - 1]] % MOD;
        if (pc % 2 == 0) {
            result += term; if (result >= MOD) result -= MOD;
        } else {
            result -= term; if (result < 0) result += MOD;
        }
    }
    printf("%d\n", result);
}
