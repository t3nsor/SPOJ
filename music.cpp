// 2026-05-25
// This problem becomes almost trivial once you view it in terms of the prefix
// sum array instead of the original array.  An operation consists of choosing
// two entries, where the earlier one has a greater value than the later one,
// and swapping them.
#include <algorithm>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
int main() {
    // finally a problem that doesn't have multiple test cases per run
    int N; scanf("%d", &N);
    vector<int> A(N), B(N + 1);
    vector<pair<int, int>> C;
    for (int i = 0; i < N; i++) {
        scanf("%d", &A[i]);
        B[i + 1] = B[i] + A[i];
    }
    const int total = B[N];
    for (int i = 0; i <= N; i++) C.emplace_back(B[i], i);
    sort(C.begin(), C.end());
    // check for impossible cases (easy)
    for (int i = 0; i <= N; i++) {
        if (C[i].first < 0 || (i > 0 && C[i].first <= C[i - 1].first)) {
            puts("-1");
            return 0;
        }
    }
    // compress coordinates
    for (int i = 1; i < N; i++) {
        B[i] = lower_bound(C.begin(), C.end(),
                           make_pair(B[i], 0)) - C.begin();
    }
    // apply selection sort
    int result = 0;
    for (int i = 1; i < N; i++) {
        if (B[i] == i) continue;
        ++result;
        // where value `i` is currently located
        const int loc = C[i].second;
        // swap `B[i]` into that location (don't bother updating `B[i]`)
        B[loc] = B[i];
        // record the new location of the value we swapped out of position `i`
        C[B[i]].second = loc;
    }
    printf("%d\n", result);
}
