// 2024-01-07
// Let n be a positive integer. The number of ordered pairs of integers (a, b)
// with a^2 + b^2 = n is given by:
//
// https://en.wikipedia.org/wiki/Sum_of_squares_function#k_=_2
//
// If n = c^2 where c = 2^g p_1^{f_1} p_2^{f_2} ... q_1^{h_1} q_2^{h_2} ... then
// this expression equals 4(2f_1 + 1)(2f_2 + 1) ...
//
// If c^2 = a^2 + b^2, say that the ordered pair (a, b) is *canonical* if
// |a| >= |b| and a > 0. Let C(c) = (2f_1 + 1)(2f_2 + 1)... (that is, 1/4 times
// the expression above). Then C(c) is the number of canonical representations
// of c^2, and C is a multiplicative function. Let P(c) be the number of
// Pythagorean triangles with hypotenuse c. Then C(c) = 2P(c) + 1 and the
// Pythagorean triangles are obtained by taking the canonical pairs for which
// b is (strictly) positive.
//
// We must first find R, the smallest c such that P(c) = N, or C(c) = 2N + 1.
// Since factors of 2 or q_i increase c while not increasing C(c), all of R's
// prime factors must be congruent to 1 (mod 4). So we want to find f_1, f_2,
// ... that minimize c = 5^{f_1} 13^{f_2} ... subject to
// 2N + 1 = (2f_1 + 1)(2f_2 + 1) ... .
//
// The greatest possible number of nontrivial factors that can occur in 2N + 1
// given that N is less than some fixed bound must be attained when 2N + 1 is a
// power of 3. So the maximum is 7 (N = 1043; 2N + 1 = 2187 = 3^7). This
// implies that we only need the first 7 primes congruent to 1 (mod 4), namely
// 5, 13, 17, 29, 37, 41, 53 since we can never get a minimal value of c if we
// skip one of these primes in favour of a bigger one.
//
// Since the bound on N is small, it suffices to brute force generate all
// factorizations of 2N + 1 and take the minimum of all c values thus produced.
// Once the value of R has been computed we can generate all Pythagorean
// triangles with R as the hypotenuse by first generating all the canonical
// representations.
//
// Each prime congruent to 1 (mod 4) can be written as a^2 + b^2 (with a, b both
// positive, and a > b) in exactly one way (shown below in the `primes` array).
// Using the ring Z[i] we can derive the canonical representations of the
// squares of these primes. For example 5 = 2^2 + 1^2 = (2 + i)(2 - i) so
// 5^2 = (2 + i)^2 (2 - i)^2 = (3 + 4i)^2 (3 - 4i)^2 which gives the canonical
// representations (5, 0), (4, +/- 3).  And in fact 5^(2k) has 2k + 1 canonical
// representations; two of them are obtained from (3 + 4i)^k (3 - 4i)^k and the
// other 2k - 1 are just (5a, 5b) for each canonical representation (a, b) of
// 5^(2k - 2).
//
// To get all canonical representations of c^2 d^2, where c and d are coprime,
// we just write down all canonical representations of both factors:
// c^2 = (c + 0i)(c - 0i)
// c^2 = (a + bi)(a - bi) (*)
// c^2 = (a - bi)(a + bi)
// ...
// d^2 = (d + 0i)(d - 0i)
// d^2 = (e + fi)(e - fi)
// d^2 = (e - fi)(e + fi) (*)
// ...
//
// and take the Cartesian product, e.g, using the two starred representations we
// get c^2 d^2 = (p + qi)(p - qi) where p + qi = (a + bi)(e - fi). Note that
// (p, q) might not itself be canonical, but can be canonicalized by multiplying
// by a power of i.

#include <algorithm>
#include <limits>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;

using i64 = long long;
using i128 = __int128;
struct Zi {
    i64 a = 0;
    i64 b = 0;
};
Zi operator*(Zi c1, Zi c2) {
    return {i64(i128(c1.a) * c2.a - i128(c1.b) * c2.b),
            i64(i128(c1.a) * c2.b + i128(c1.b) * c2.a)};
}
Zi canonicalize(Zi zi) {
    if (abs(zi.a) < abs(zi.b)) {
        swap(zi.a, zi.b);
        if (zi.a < 0) {
            zi.a *= -1;
        } else {
            zi.b *= -1;
        }
    }
    return zi;
}

constexpr int nprimes = 7;
const int primes[nprimes][3] = {{ 5, 2, 1},
                                {13, 3, 2},
                                {17, 4, 1},
                                {29, 5, 2},
                                {37, 6, 1},
                                {41, 5, 4},
                                {53, 7, 2}};

vector<vector<Zi>> gen(int i) {
    const int p = primes[i][0];
    const Zi prime_rep = {primes[i][1], primes[i][2]};
    const Zi base = prime_rep * prime_rep;
    Zi power = {1, 0};
    vector<vector<Zi>> result;
    i128 c = 1;
    result.resize(1);
    result[0].push_back(Zi{1, 0});
    for (;;) {
        c = c * p;
        if (c > numeric_limits<i64>::max()) break;
        auto copy = result.back();
        result.push_back(move(copy));
        auto& newelem = result.back();
        for (Zi& zi : newelem) {
            zi.a *= p;
            zi.b *= p;
        }
        power = power * base;
        auto newrep = canonicalize(power);
        newelem.push_back(newrep);
        newrep.b = -newrep.b;
        newelem.push_back(newrep);
    }
    return result;
}
// canonical_pwr[p][k] gives the first factor for each canonical representation
// of p^(2k)
const vector<vector<Zi>> canonical_pwr[nprimes] = {
    gen(0),
    gen(1),
    gen(2),
    gen(3),
    gen(4),
    gen(5),
    gen(6)
};

void rec(int n, int nextf, int nextp,
         i64& curR, i64& bestR,
         vector<int>& curfact, vector<int>& bestfact) {
    if (n == 1) {
        if (curR < bestR || bestR == -1) {
            bestR = curR;
            bestfact = curfact;
        }
        return;
    }
    for (; nextf > 1; nextf--) {
        if (n % nextf) continue;
        const int exponent = (nextf - 1) / 2;
        i128 nextR = curR;
        bool fail = false;
        for (int i = 0; i < exponent; i++) {
            nextR *= primes[nextp][0];
            if (nextR > numeric_limits<i64>::max()) {
                fail = true;
                break;
            }
        }
        if (fail) continue;
        const i64 oldcurR = curR;
        curR = nextR;
        curfact.push_back(exponent);
        rec(n / nextf, nextf, nextp + 1, curR, bestR, curfact, bestfact);
        curR = oldcurR;
        curfact.pop_back();
    }
}

void do_testcase() {
    int N; scanf("%d", &N);
    vector<int> curfact, fact;
    i64 curR = 1, R = -1;
    rec(2*N + 1, 2*N + 1, 0, curR, R, curfact, fact);
    printf("%lld\n", R);
    vector<Zi> canonical = {{1, 0}};
    for (int i = 0; i < fact.size(); i++) {
        vector<Zi> new_canonical;
        for (const auto& zi1 : canonical) {
            for (const auto& zi2 : canonical_pwr[i][fact[i]]) {
                new_canonical.push_back(canonicalize(zi1 * zi2));
            }
        }
        swap(canonical, new_canonical);
    }
    vector<i64> result;
    for (const auto& zi : canonical) {
        if (zi.b > 0) result.push_back(zi.b);
    }
    sort(result.begin(), result.end());
    for (const auto leg : result) {
        printf("%lld\n", leg);
    }
}

int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
