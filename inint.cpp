// 2026-07-29
// Define d(x) as the pair (s(x), w(x)), where s is the number of steps required
// to reach a given integer, and w is the number of ways to reach it.  Define
// v(x) as the column vector (d(x), d(x + 1), ..., d(x + 8)).  Now here's
// the key insight: we can write
//
//   v(x + 1) = A(x)*v(x)
//
// where A(x) is an appropriate 9 x 9 matrix, and the matrix multiplication is
// done over the following semiring:
//
//   (s_1, w_1) ⊕ (s_2, w_2) = (min(s_1, s_2),
//                              w_1        if s_1 < s_2
//                              w_1 + w_2  if s_1 = s_2
//                              w_2        if s_1 > s_2)
//
//   (s_1, w_1) ⊗ (s_2, w_2) = (s_1 + s_2, w_1 w_2)
//
// (ChatGPT tells me this is called the "min-plus ℕ-layered tropical semiring".)
// Note that ⊕ is just how we combine two possible predecessors for a given
// number (take the smaller number of steps, combine the ways if they are equal)
// and ⊗ is defined to make the math work out (we need a semiring so that we can
// apply matrix multiplication).
//
// Thus, d(K) can be computed by first computing the matrix product A(K - 1) *
// ... * A(1) * A(0), multiplying it by v(0) to get v(K), and reading off the
// first entry.
//
// The entry A(x)_{ij} (where i, j start at zero) is given by
//   (0, 1)   if j = i + 1
//   (1, 1)   if i = 8 and
//               the decimal representation of x + j contains the digit 9 - j
//   (+∞, ?)  otherwise
// where ? represents an arbitrary value.
//
// Since A(x) is not constant with respect to x, we still have some work to do
// to compute the necessary products.  Observe that we can memoize blocks of
// powers of 10: for example, in the product A(10x + 9) * ... * A(10x), the
// value of A(10x + k) (where 0 <= k <= 9) depends only on the digit sets of
// 10x + k, ..., 10x + k + 8.  If k + m (0 <= m <= 8) is less than or equal to
// 9, then the digit set of 10x + k + m depends on x only through the set of
// digits that occur in x, not how many times each digit occurs.  If k + m is
// greater than or equal to 10, then the digit set of 10x + k + m depends on x
// only through the digit set of x + 1.  Thus, if x' is such that x' has the
// same digit set as x and x' + 1 also has the same digit set as x + 1, then
// A(10x' + 9) * ... * A(10x') will be equal to A(10x + 9) * ... * A(10x).  This
// extends to higher powers of 10, e.g., A(100x + 99) * ... * A(100x) depends
// only on the digit sets of x and x + 1.
//
// The actual number of possible ordered pairs (S_0, S_1) where S_0 is the digit
// set of a natural number x and S_1 is the digit set of x + 1 is small enough
// to make a memoized recursive approach feasible.  We don't care about the
// digit 0, so a loose upper bound is as follows: there are 512 possible digit
// sets for x.  When x is incremented, the least significant digit other than 9
// is incremented; there are 9 possibilities for the value of that digit (call
// it d) and it may or may not still be present in x + 1 (depending on whether
// there were other occurrences of it in x).  If there were any 9's to the right
// of that digit, then x' may be lacking 9's compared with x (if there weren't
// any to the left), which multiplies the number of possibilities by 2.  So in
// total, there are not more than 512*9*2*2 = 18432 possible values for (S_0,
// S_1).  Let A(e, x) denote A((x + 1)*10^e - 1) * ... * A(x*10^e), which can
// be recursively calculated as A(e - 1, 10*x + 9) * ... * A(e - 1, 10*x).  We
// just saw that for a given value of e (1 <= e <= 8), there are at most 18432
// different values of A(e, x).  In actual fact there are fewer, since d is
// constrained to be one of the digits that is already in x, and 9 can be taken
// away only if it's present in the first place; moreover, the larger the value
// of e, the fewer digit values could possibly be represented for values of x
// that we actually care about: for example, for e = 5, we only care about x
// having up to 4 digits.
//
// Thus, for a given K we can compute the required matrix product by advancing
// powers of 10 in the usual way, e.g. for K = 5291, A(5290) * ... * A(0) can be
// expressed as A(0, 5290) * A(1, 528) * A(1, 527) * ... * A(1, 520) * A(2, 51)
// * A(2, 50) * A(3, 4) * ... * A(3, 0).
//
// (This code gets 0.65s on SPOJ; most accepted solutions run in less than half
// that, and the fastest solution is 0.08s.  I have no idea what those people
// did; maybe there's some clever way to exploit the structure directly without
// having to construct and multiply a bunch of matrices.)

#include <algorithm>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <unordered_map>
using namespace std;

constexpr int MOD = 1000000007;

// larger than any number of steps required for any input in range
const int INF = 2e8;

// an element of the semiring
struct SR {
    int s;
    int w;
    friend SR operator+(SR x, SR y) {
        SR result;
        result.s = min(x.s, y.s);
        if (x.s < y.s) {
            result.w = x.w;
        } else if (x.s > y.s) {
            result.w = y.w;
        } else {
            result.w = x.w + y.w;
            if (result.w >= MOD) result.w -= MOD;
        }
        return result;
    }
    friend SR operator*(SR x, SR y) {
        return {x.s + y.s, int((x.w * (long long)y.w) % MOD)};
    }
    // returns an additive "identity"
    static SR zero() { return {INF, 0}; }
    // returns a multiplicative identity
    static SR one() { return {0, 1}; }
};

struct Matrix : vector<SR> {
    SR* operator[](int i) { return data() + 9*i; }
    const SR* operator[](int i) const { return data() + 9*i; }

    Matrix() : vector(81, SR::zero()) {}

    static Matrix identity() {
        Matrix result;
        for (int i = 0; i < 9; i++) {
            result[i][i] = SR::one();
        }
        return result;
    }

    static Matrix single(int x) {
        Matrix result;
        char buf[11];
        for (int i = 0; i < 8; i++) result[i][i + 1] = SR::one();
        for (int i = 0; i < 9; i++) {
            int y = x + i;
            bool have = false;
            while (y > 0 && !have) {
                if (y % 10 == 9 - i) have = true;
                else y /= 10;
            }
            if (have) result[8][i] = {1, 1};
        }
        return result;
    }

    friend Matrix operator*(const Matrix& A, const Matrix& B) {
        Matrix result;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                for (int k = 0; k < 9; k++) {
                    result[i][j] = result[i][j] + A[i][k]*B[k][j];
                }
            }
        }
        return result;
    }
};

unordered_map<int, Matrix> memo[9];

Matrix A(int e, int x) {
    if (e == 0) return Matrix::single(x);
    int xmask = 0;
    int xcopy = x;
    while (xcopy > 0) {
        const int d = xcopy % 10;
        if (d > 0) xmask |= (1 << (d - 1));
        xcopy /= 10;
    }
    int xp1mask = 0;
    xcopy = x + 1;
    while (xcopy > 0) {
        const int d = xcopy % 10;
        if (d > 0) xp1mask |= (1 << (d - 1));
        xcopy /= 10;
    }
    auto ir = memo[e].emplace(xmask + (xp1mask << 9), Matrix::identity());
    auto& result = ir.first->second;
    if (ir.second) {
        for (int i = 0; i <= 9; i++) {
            result = A(e - 1, 10*x + i) * result;
        }
    }
    return result;
}

void do_testcase(int K) {
    Matrix prod = Matrix::identity();
    int cur = 0;
    int e = 0;
    int p = 1;
    while (e <= 7 && p <= K) { e++; p *= 10; }
    while (cur < K) {
        while (cur + p > K) { e--; p /= 10; }
        while (cur + p <= K) {
            prod = A(e, cur / p) * prod;
            cur += p;
        }
    }
    const SR v[9] = {SR::zero(), {0, 1}, {1, 1}, SR::zero(), {2, 1},
                     SR::zero(), SR::zero(), SR::zero(), {3, 1}};
    SR result = SR::zero();
    for (int i = 0; i < 9; i++) result = result + prod[0][i]*v[i];
    if (result.s >= INF) {
        puts("IMPOSSIBLE");
    } else {
        printf("%d %d\n", result.s, result.w);
    }
}

int main() {
    int K;
    while (scanf("%d", &K) == 1) do_testcase(K);
}
