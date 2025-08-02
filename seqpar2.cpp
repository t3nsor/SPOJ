// 2025-08-01
// Once the problem statement is understood, the problem can be solved using
// well-known techniques, almost systematically.
//
// The first step is to "exhaust" the first constraint as follows.  The sequence
// cannot be split between p and p + 1 if there exists p' <= p and q > p such
// that B_{p'} <= A_q and the sequence cannot be split between p' and p.  There
// is a unique finest partition P* that satisfies the first constraint: the one
// that takes every allowable split point (under the first constraint).  Any
// partition that is coarser than P* also satisfies the first constraint.  That
// is, the elements of P* are "atoms" where the first constraint is equivalent
// to the requirement to not split atoms.  Now, we know that we care about only
// the sum of B-components of each part, which can be expressed as the sum of
// B-components of the atoms in that part, if the B-component of an atom is
// defined as the sum of B-components within it.  Likewise, we care about only
// the maximum A-component in each part, which can be expressed as the maximum
// of A-components of the atoms in that part, if the A-component of an atom is
// defined as the maximum A-component within it. We thus replace the original
// sequence with the sequence of atoms thus defined, and reduce the original
// problem to a new problem in which the first constraint is no longer present.
//
// The next step is to transform the problem into a decision problem.  Instead
// of maximizing the objective function subject to Limit, instead choose a value
// of the objective function and attempt to minimize the second constraint.
// When a value m of the objective function is chosen, we must choose each part
// such that the sum of B-values in that part doesn't exceed m.  If a value m is
// feasible, so are all values larger than m, so we can use binary search to
// find the optimal (least) m.
//
// If the first `i` elements of the sequence have been validly partitioned, and
// the ith element is the last element in its part, then we don't care how the
// first `i` elements were partitioned; we only care about the sum of maximum
// A-components so far. That is to say, if we define f_m(i) as the minimum
// possible sum of maximum A-components across all valid partitions of the
// first `i` elements, where the ith element is the last element in its part;
// then
//
//   f_m(i) = \min_{j < i; \sum_{k \in [j, i)} B_k <= m}
//              (f_m(j) + \max_{k \in [j, i)} A_k)
//
// and we need to check whether f_m(N) <= Limit.  This is an O(N^2) DP, so we
// need to find a way to optimize it.
//
// First, observe that f_m(i + 1) >= f_m(j).  This is because we can always take
// a valid partition of the first i + 1 elements and obtain from it a valid
// partition of the first i elements with not greater total value by deleting
// the last element (and dropping its part, if it was a singleton).
//
// We need some kind of data structure that holds the values of `j` that are
// valid for the current `i`.  Such values of `j` fall within a sliding window.
// When `i` increments, a new value of `j` is added to the data structure
// (namely j = i - 1) but zero or more of the smallest values of `j` are
// deleted, namely those for which \sum_{k \in [j, i)} B_k > m.  So long as
// inserting at one end and deleting from the other end are "efficient" (say,
// O(log N) per insertion/deletion) we are good so far because we'll have N
// total insertions and deletions (for a given iteration of the top-level binary
// search).  We also need to be able to query the best `j` value that is
// currently in the data structure, namely the one for which f_m(j) + \max_{k
// \in [j, i)} A_k is least.  Call the second term M_i(j).  So what we have so
// far is: we have some interval of `j`s that are currently in the data
// structure.  Each `j` value can be thought of as a "point" whose coordinates
// are f_m(j) and M_i(j); the former is increasing, and the latter decreasing.
// At any given time we need to be able to quickly identify the point whose sum
// of coordinates is least.  When `i` increments, all M_i values of `j` values
// that are still in the data structure may change.  How do they change?  The
// ones that are less than A_i increase to A_i.  All others are unchanged.
// There can potentially be O(N) M_i values that change for *each* increment of
// `i`, so it is important that we avoid having to individually update all the
// M_i values.
//
// The idea is to maintain a set of lists.  Each list is associated with a
// particular value of M_i, and holds, in ascending order, the list of all `j`
// such that M_i(j) equals that value.  When we process A_i, all lists whose
// value is less than or equal to A_i are concatenated.  Note that because M_i
// is decreasing with `j`, if we have two lists L1 and L2 where L1 has a smaller
// M_i value than L2, it is known that all `j`s in L2 are less than all `j`s in
// L1.  So the lists are concatenated starting with the list with the largest
// value that is less than or equal to A_i.  The smallest f_m + M_i value among
// all currently valid `j` will be attained by the head of one of the lists
// (since the head has the least f_m among all the elements in the list, which
// all have the same M_i).  To efficiently keep the lists sorted amongst
// themselves by their M_i values, we keep a list of lists, but to efficiently
// identify the one with the best f_m + M_i, we keep all the f_m + M_i values in
// an ordered map (mapping a given f_m + M_i value to the number of times it's
// attained).  Finally, note that locating `j`s (in order to remove them) when
// `i` increments is easy: they're at the beginning of the list with the largest
// M_i (and if that entire list is removed, the beginning of the list with the
// next largest M_i, and so on).
//
// In the actual implementation, the use of linked lists is not necessary.  We
// only need to maintain an array of `f` values and a deque of pairs
// (h, m) where `h` is the index of the head of a list and `m` is the value of
// `M_i(j)` for all `j` in the list (plus the ordered map, of course).
//
// A Chinese blogger gives an alternative approach,
// https://blog.csdn.net/sdogsq/article/details/23270513

#include <algorithm>
#include <deque>
#include <map>
#include <stdio.h>
#include <vector>
using namespace std;
void dec(map<long long, int>& M, long long key) {
    auto it = M.find(key);
    if (0 == --it->second) {
        M.erase(it);
    }
}
bool can(const vector<int>& A, const vector<int>& psum, int L, int m) {
    const int N = A.size();
    vector<long long> f(N + 1);
    f[0] = 0;
    // Initially, we have only one list, it starts with 0, and its M value is
    // arbitrarily set at -1 (so it will have to increase regardless of how
    // small A[0] is).
    deque<pair<int, int>> D = {{0, -1}};
    // The placeholder -1 is not included in M.
    map<long long, int> M;

    for (int i = 1; i <= N; i++) {
        // Update M_i values.
        int p = D.size();
        while (p > 0 && D[p - 1].second <= A[i - 1]) --p;
        if (p < D.size() && D[p].second < A[i - 1]) {
            int j = D[p].first;
            long long y = f[j] + D[p].second;
            if (D[p].second != -1) dec(M, y);
            D[p].second = A[i - 1];
            y = f[j] + D[p].second;
            M[y]++;
        }
        while (D.size() > p + 1) {
            int j = D.back().first;
            long long y = f[j] + D.back().second;
            if (D.back().second != -1) dec(M, y);
            D.pop_back();
        }

        // Delete from `D` any indices that cannot be in the same part as
        // i - 1.  Note that `m` is always at least as large as all B values,
        // therefore i - 1 can always be assigned to a part (maybe its size has
        // to be 1, though).
        while (psum[i] - psum[D[0].first] > m) {
            int j = D[0].first;
            long long y = f[j] + D[0].second;
            dec(M, y);
            if (D.size() >= 2 && D[0].first + 1 == D[1].first) {
                D.pop_front();
            } else {
                j = ++D[0].first;
                y = f[j] + D[0].second;
                M[y]++;
            }
        }

        f[i] = M.begin()->first;

        // Finally, start a new list with the current index.  It always starts
        // out with an M_i value of -1, since the current A value is not itself
        // included in the max.
        D.emplace_back(i, -1);
    }

    return f[N] <= L;
}
int main() {
    int N, L; scanf("%d %d", &N, &L);
    vector<int> A(N), B(N);
    for (int i = 0; i < N; i++) scanf("%d %d", &A[i], &B[i]);
    vector<int> sufmax(N);
    sufmax[N - 1] = A[N - 1];
    for (int i = N - 2; i >= 1; i--) sufmax[i] = max(sufmax[i + 1], A[i]);
    vector<int> A2, B2;
    int atomA = A[0], atomB = B[0], minB = B[0];
    for (int i = 1; i < N; i++) {
        if (sufmax[i] < minB) {
            A2.push_back(atomA);
            B2.push_back(atomB);
            atomA = A[i];
            atomB = minB = B[i];
        } else {
            atomA = max(atomA, A[i]);
            atomB += B[i];
            minB = min(minB, B[i]);
        }
    }
    A2.push_back(atomA);
    B2.push_back(atomB);
    vector<int> psum(B2.size() + 1);
    for (int i = 0; i < B2.size(); i++) {
        psum[i + 1] = psum[i] + B2[i];
    }
    // The initial left value for the binary search will be the maximum B value.
    // The initial right value will be the total B value.
    int l = -1, r = 0;
    for (int i = 0; i < B2.size(); i++) {
        l = max(l, B2[i]);
        r += B2[i];
    }
    while (l < r) {
        const int m = l + (r - l) / 2;
        if (can(A2, psum, L, m)) {
            r = m;
        } else {
            l = m + 1;
        }
    }
    printf("%d\n", l);
}
