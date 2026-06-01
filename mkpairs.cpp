// 2026-05-26
// You may have immediately intuited that this is a "convex hull trick" problem.
// All we have to do is work out the details.
//
// Let dp[i][j] be the maximum profit that can be achieved using only the first
// `i` accordionists and `j` banjoists, where accordionist `i-1` and banjoist
// `j-1` are paired.  Since all `A` and `B` values are nonnegative, it is never
// necessary to consider a transition that simultaneously skips one or more
// accordionists and one or more banjoists.  That is:
//
//   dp[i][j] = A_{i-1} B_{j-1} +
//              max(
//                max_{0 <= k < j} (dp[i-1][k] - (B_k + ... + B_{j-2})^2),
//                max_{0 <= k < i} (dp[k][j-1] - (A_k + ... + A_{i-2})^2)
//              )
//
// where the base case is given by dp[0][0] = 0 and dp[0][k] = dp[k][0] = -inf
// for all `k` > 0.
//
// To apply the convex hull trick, write
//
//   dp[i-1][k] - (B_k + ... + B_{j-2})^2 = (2E_k)x + F_{i-1,k} - x^2
//
// where x         = B_0 + ... + B_{j-2},
//       E_k       = B_0 + ... + B_{k-1},
//       F_{i-1,k} = dp[i-1][k] - E_k^2
//
// So for a fixed value of `i`, we have a family of linear functions of the form
// y = 2E_k x + F_{i-1,k}, and to compute dp[i][j] we query this family for the
// largest y for the current x (and then remember to subtract x^2 at the end).
// Similarly for each `j` value we need to maintain a family of linear functions
// of the form (2C_k)x + D_{k,j-1} (here x = A_0 + ... + A_{i-2}).
// 
// To simplify updating the data structure, we will need to ensure that the
// lines are always inserted in increasing order of `k`, so that their slopes
// will be monotonically increasing and new lines are always inserted at the end
// (which can result in some lines being popped off from the end).  We also want
// to ensure that the queries for a given `i` (the x values) are monotonically
// increasing so that we end up with an O(N^2) algorithm (where lines are popped
// from the front over time) instead of O(N^2 log N) (binary search).  This is
// actually the trickiest part of the algorithm as we need to ensure that lines
// are added at just the right time.  The approach we take is to compute all
// dp[i][j] with i + j = s + 1 immediately before inserting all dp[i][j] with
// i + j = s into the data structures.

#include <deque>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>
using namespace std;

using LL = long long;

struct DS {
    deque<pair<LL, LL>> lines;

    // `m` must be greater than or equal to all previous `m`s added to this `DS`
    void add(LL m, LL b) {
        if (lines.size() >= 1 && m == lines.back().first) {
            if (b <= lines.back().second) return;
            lines.pop_back();
        }
        while (lines.size() >= 2) {
            const auto last = lines[lines.size() - 1];
            const auto secondlast = lines[lines.size() - 2];
            if ((last.second - b)*(last.first - secondlast.first) <=
                (secondlast.second - last.second)*(m - last.first)) {
                lines.pop_back();
            } else break;
        }
        lines.emplace_back(m, b);
    }

    // `x` must be greater than or equal to all previous queries on this `DS`
    LL query(LL x) {
        // if there are no lines, we cannot use this branch of the max since
        // there are no cows to pair with (happens when computing dp[1][k] or
        // dp[k][1] for k > 1)
        if (lines.empty()) return numeric_limits<LL>::lowest() / 2;
        while (lines.size() >= 2) {
            if (lines[1].first * x + lines[1].second >=
                lines[0].first * x + lines[0].second) {
                lines.pop_front();
            } else break;
        }
        return lines[0].first * x + lines[0].second;
    }
};

int main() {
    int N; cin >> N; ++N;
    vector<int> A(N), B(N);
    vector<LL> pA(N + 1), pB(N + 1);
    vector<DS> ds1(N + 1), ds2(N + 1);
    for (int i = 0; i < N; i++) {
        if (i == N - 1) A[i] = 0; else cin >> A[i];
        pA[i + 1] = pA[i] + A[i];
    }
    for (int i = 0; i < N; i++) {
        if (i == N - 1) B[i] = 0; else cin >> B[i];
        pB[i + 1] = pB[i] + B[i];
    } 

    // insert dp[0][0]
    ds1[0].add(0, 0);
    ds2[0].add(0, 0);

    vector<vector<LL>> dp(N + 1, vector<LL>(N + 1));
    for (int sum = 2; sum <= 2*N; sum++) {
        for (int i = max(1, sum - N), j = sum - i;
             i <= N && j >= 1;
             i++, j--) {
            const LL x1 = pB[j-1];
            const LL x2 = pA[i-1];
            dp[i][j] = A[i-1]*B[j-1] + max(ds1[i-1].query(x1) - x1*x1,
                                           ds2[j-1].query(x2) - x2*x2);
        }
        for (int i = max(1, sum - 1 - N), j = sum - 1 - i;
             i <= N && j >= 1;
             i++, j--) {
            ds1[i].add(2*pB[j], dp[i][j] - pB[j]*pB[j]);
            ds2[j].add(2*pA[i], dp[i][j] - pA[i]*pA[i]);
        }
    }
    cout << dp[N][N] << '\n';
}
