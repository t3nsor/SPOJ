// 2026-09-22
// This problem is solved using a modified version of KMP.  Let's suppose that
// we have a partial match of the pattern against the text, meaning that there
// is a prefix A of the pattern that is isomorphic to a suffix B of the current
// prefix of the text, where A, B are considered isomorphic iff for all i, j,
// sgn(A[i] - A[j]) = sgn(B[i] - B[j]).  To determine whether B can be extended
// using the next character of the text (to be isomorphic to the prefix of the
// pattern with 1 more character), we need to check whether the next text
// character has the same relationship to B as the next pattern character has to
// A.  This can be done as follows: precompute, for each pattern character, the
// index of the largest character value occurring before it and less than it,
// and same for equal and greater.  Now, we just need to check that the next
// text character is indeed greater than the character in B positionally
// corresponding to the character in A that is less than the next pattern
// character, and so on for the other two values.  When we fail to match, then
// we use the KMP failure function which is computed from the pattern in an
// analogous manner.
//
// The precomputation step for the `lt` and `gt` arrays takes O(KS) time, which
// is acceptable since S <= 25.  The official solution to CEOI 2011 "Matching"
// (which is essentially the same problem) explains how to do it in O(K) time;
// the algorithm from there needs to be slightly modified for this problem since
// duplicate values in the pattern are allowed.  Start out with a linked list
// whose nodes represent, in ascending order, the values [0, ..., S-1] that
// actually occur in the pattern, and maintain an lookup table from each value
// to the corresponding node, and the number of times each value occurs in the
// pattern.  Then, compute `lt[K - 1]` and `gt[K - 1]` first by looking at the
// node immediately preceding and immediately following the node corresponding
// to the value of character `K - 1` in the pattern.  Then "delete" this
// character by decrementing its frequency and, if the result is zero, deleting
// the corresponding node from the linked list.  Then, move on to computing
// `lt` and `gt` for `K - 2` and so on.
#include <stdio.h>
#include <vector>
using namespace std;
int main() {
    int N, K, S; scanf("%d %d %d", &N, &K, &S);
    vector<int> text(N);
    for (int i = 0; i < N; i++) scanf("%d", &text[i]);
    vector<int> pattern(K);
    vector<int> failure(K + 1);
    failure[0] = -1;
    vector<int> loc(S, -1);
    vector<int> lt(K, -1), eq(K, -1), gt(K, -1);
    for (int i = 0; i < K; i++) {
        scanf("%d", &pattern[i]); --pattern[i];
        for (int f = failure[i]; f >= 0; f = failure[f]) {
            // check whether the relationship of pattern[i] to
            // pattern[i-f...i-1] is the same as the relationship of pattern[f]
            // to pattern[0...f-1]
            if (lt[f] >= 0 && pattern[i] <= pattern[i - f + lt[f]]) continue;
            if (eq[f] >= 0 && pattern[i] != pattern[i - f + eq[f]]) continue;
            if (gt[f] >= 0 && pattern[i] >= pattern[i - f + gt[f]]) continue;
            failure[i + 1] = f + 1;
            break;
        }
        eq[i] = loc[pattern[i]];
        for (int j = pattern[i] - 1; j >= 0; j--) {
            if (loc[j] >= 0) { lt[i] = loc[j]; break; }
        }
        for (int j = pattern[i] + 1; j < S; j++) {
            if (loc[j] >= 0) { gt[i] = loc[j]; break; }
        }
        loc[pattern[i]] = i;
    }
    vector<int> matches;
    int p = 0;
    for (int i = 0; i < N; i++) {
        // check whether the relationship of pattern[p] to pattern[0...p-1] is
        // the same as the relationship of text[i] to text[i-p...i-1]
        for (; p >= 0; p = failure[p]) {
            if (lt[p] >= 0 && text[i] <= text[i - p + lt[p]]) continue;
            if (eq[p] >= 0 && text[i] != text[i - p + eq[p]]) continue;
            if (gt[p] >= 0 && text[i] >= text[i - p + gt[p]]) continue;
            p++;
            break;
        }
        if (p == K) {
            matches.push_back(i - K + 1);
            p = failure[p];
        }
    }
    printf("%d\n", (int)matches.size());
    for (const auto x : matches) printf("%d\n", x + 1);
}
