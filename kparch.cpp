// 2026-08-20
// This problem looks like it should be solvable in linear time, but I don't
// know a linear time algorithm.  This solution runs in O(L log L) (L being the
// length of the input string) using divide and conquer.  Suppose the first half
// of the input string is called A, and the second half B.  We first recursively
// solve the subproblems where A and B are each considered an entire string.
// Then, we have to consider the cases that cross the boundary between A and B.
// We have to count these in O(L) time to get an overall O(L log L) algorithm;
// anything slower will probably not pass.
//
// We expect the possibly-quadratic count to be expressible as a sum of a linear
// number of terms of the form max(...) involving Z-functions of A, B, and
// their reverses, because the condition for a repeating substring to exist in
// a particular position can be expressed in terms of each part of the copy
// that crosses the boundary, i.e., prefixes of B and of Ar, match other parts
// of A/B/Ar/Br.
//
// Concretely: ignore for now the special case where a suffix of A is equal to
// a prefix of B.  In the general case, either the first or second copy of the
// repeating substring must cross the boundary between A and B for example,
// suppose it's the first copy.  Let `i` be the number of characters of the
// first copy that lie in A, and `j` the number that lie in `B`, with l = i +
// j.  Then, it must be the case that B[0,j) = B[l,l+j) and Ar[0,i) =
// Br[n-l,n-l+i), where `n` is the length of B, and Ar and Br stand for the
// reverses of A and B, respectively.  For a given `l`, the first condition
// will hold when `j` is less than or equal to the longest common prefix of B
// and B[l...], and the second condition will hold when `i` is less than or
// equal to the longest common prefix of Ar and Br[n-l...].  Thus, knowing how
// B matches suffixes of itself and how Ar matches suffixes of Br lets us count
// the number of valid pairs (i,j) for given `l`.  The case where the second
// copy crosses the boundary is handled analogously.

#include <algorithm>
#include <iterator>
#include <stdio.h>
#include <string.h>
using namespace std;
/// Compute the Z-function of `P` concatenated with `T` where `P` has length
/// `m`, `T` has length `n`, and a substring is not allowed to cross the
/// boundary between `P` and `T`.  Store the first `m` values in the result in
/// `Z` and the last `n` values in `E`.  `P` and `T` must be random access
/// iterators.
template <class I1, class I2>
void calcz(I1 P, int m, I2 T, int n, int* Z, int* E) {
    Z[0] = m;
    for (int i = 1, l = 0, r = 0; i < m; i++) {
        if (i < r) Z[i] = min(r - i, Z[i - l]); else Z[i] = 0;
        while (i + Z[i] < m && P[Z[i]] == P[i + Z[i]]) ++Z[i];
        if (i + Z[i] > r) { l = i; r = i + Z[i]; }
    }
    for (int i = 0, l = 0, r = 0; i < n; i++) {
        if (i < r) E[i] = min(r - i, Z[i - l]); else E[i] = 0;
        while (E[i] < m && i + E[i] < n && P[E[i]] == T[i + E[i]]) ++E[i];
        if (i + E[i] > r) { l = i; r = i + E[i]; }
    }
}
/// Return the number of pairs (i, j) such that i + j = sum with
/// imin <= i <= imax
/// jmin <= j <= jmax
inline int num_pairs(int sum, int imin, int imax, int jmin, int jmax) {
    if (imin > imax || jmin > jmax || imin + jmin > sum || imax + jmax < sum) {
        return 0;
    }
    return min(imax, sum - jmin) - max(imin, sum - jmax) + 1;
}
constexpr int CUTOFF = 8;
long long rec(const char* buf, int L) {
    if (L <= CUTOFF) {
        int result = 0;
        for (int l = 1; 2*l <= L; l++) {
            for (int i = 0; i + 2*l <= L; i++) {
                result += !memcmp(buf + i, buf + i + l, l);
            }
        }
        return result;
    }
    const char* A = buf;
    const int m = L / 2;
    const char* B = buf + m;
    const int n = L - m;
    long long result = rec(A, m) + rec(B, n);
    static int Z1[100000], E1[100000];
    calcz(B, n, A, m, Z1, E1);
    static int Z2[100000], E2[100000];
    calcz(make_reverse_iterator(A + m), m,
          make_reverse_iterator(B + n), n,
          Z2, E2);
    // Case 1: first copy contained within A
    for (int l = 1; 2*l <= m + n; l++) {
        const int imin = 0;
        const int imax = (l >= m ? 0 : Z2[l]);
        const int jmin = 1;
        const int jmax = E1[m - l];
        result += num_pairs(l, imin, imax, jmin, jmax);
    }
    // Case 2: first copy spills into B (second copy contained within B).  Note
    // that when the first copy is a suffix of A and the second a prefix of B,
    // that falls under case 1 and not 2 (so we don't double-count it).
    for (int l = 1; 2*l <= m + n; l++) {
        const int imin = 1;
        const int imax = E2[n - l];
        const int jmin = 1;
        const int jmax = (l >= n ? 0 : Z1[l]);
        result += num_pairs(l, imin, imax, jmin, jmax);
    }
    return result;
}
int main() {
    char buf[200100];
    fgets(buf, sizeof(buf), stdin);
    int L = strlen(buf);
    // we don't know whether the line ends with \n or \r\n
    while (L > 0 && buf[L - 1] <= 32) --L;
    printf("%lld\n", rec(buf, L));
}
