// 2026-03-11
// This is the two-dimensional version of range update, range sum.  The
// one-dimensional version is described here:
// https://cp-algorithms.com/data_structures/fenwick.html#3-range-update-and-range-query
// The generalization to multiple dimensions is described here:
// https://arxiv.org/pdf/1311.6093
#include <iostream>
using namespace std;
using LL = long long;
using BIT = LL[1001][1001];
BIT BITs[4];
int N;

// Query one single BIT.
LL query(const BIT& bit, int r, int c) {
    LL result = 0;
    for (int r2 = r; r2 > 0; r2 -= (r2 & -r2)) {
        for (int c2 = c; c2 > 0; c2 -= (c2 & -c2)) {
            result += bit[r2][c2];
        }
    }
    return result;
}

// Return the sum of all values in the rectangle whose lower-right corner is at
// (r, c).
LL query(int r, int c) {
    return r*c*query(BITs[0], r, c) +
           r*query(BITs[1], r, c) +
           c*query(BITs[2], r, c) +
           query(BITs[3], r, c);
}

// Update one single BIT.
void update(BIT& bit, int r, int c, int k) {
    for (int r2 = r; r2 <= N; r2 += (r2 & -r2)) {
        for (int c2 = c; c2 <= N; c2 += (c2 & -c2)) {
            bit[r2][c2] += k;
        }
    }
}

// For each cell at row r' and column c' where r' >= r and c' >= c, update that
// cell by adding k(r' - r + 1)(c' - c + 1).
void update(int r, int c, LL k) {
    // Note that the `k` values that we will pass to the `update` above are
    // obtained by comparing the expression above with the formula in `query`.
    // We expand that expression and read off the coefficients of the r'c', r',
    // c', and constant terms.
    update(BITs[0], r, c, k);
    update(BITs[1], r, c, (1 - c)*k);
    update(BITs[2], r, c, (1 - r)*k);
    update(BITs[3], r, c, (1 - r)*(1 - c)*k);
}

int main() {
    ios::sync_with_stdio(false);
    int Q; cin >> N >> Q;
    while (Q--) {
        int type; cin >> type;
        int r1, c1, r2, c2; cin >> r1 >> c1 >> r2 >> c2;
        if (type == 1) {
            cout << query(r2, c2) - query(r2, c1 - 1)
                    - query(r1 - 1, c2) + query(r1 - 1, c1 - 1) << '\n';
        } else {
            int k; cin >> k;
            update(r1, c1, k);
            update(r1, c2 + 1, -k);
            update(r2 + 1, c1, -k);
            update(r2 + 1, c2 + 1, k);
        }
    }
}
