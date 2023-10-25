// 2024-10-24
// This is an O(n log n) sliding window approach. We move the end pointer to the
// right one position at a time until a duplicate enters the window, then
// immediately move the start pointer right past the earlier instance of that
// duplicate value. We keep a heap of values that are missing from the window,
// so that we can see at once what the smallest missing value is (this means
// that the window contains a permutation of all the smaller numbers, since none
// of them are missing). When we expand the window we normally would have to
// remove an element from the heap. In order to avoid doing this, we simply mark
// it as "skipped", so that if it happens to be at the top, we treat it as if it
// weren't there, and remove it.
#include <stdio.h>
#include <queue>
#include <vector>
using namespace std;
int main() {
    int n; scanf("%d", &n);
    vector<int> A(n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &A[i]); A[i]--;
    }
    vector<int> cnt(n);  // how many are in the window
    vector<int> r(n);
    for (int i = 0; i < n; i++) r[i] = i;
    priority_queue<int, vector<int>, greater<int>> missing(r.begin(), r.end());
    vector<int> skip(n);
    int start = 0, end = 0;
    int best = -1;
    while (end < n) {
        int dup = -1;
        while (end < n) {
            int val = A[end];
            ++end;
            if (++cnt[val] == 2) {
                dup = val;
                break;
            } else {
                skip[val]++;
                while (!missing.empty() && skip[missing.top()]) {
                    skip[missing.top()]--;
                    missing.pop();
                }
                if (missing.empty()) {
                    best = n;
                } else {
                    best = max(best, missing.top());
                }
            }
        }
        if (dup >= 0) {
            for (;;) {
                int val = A[start];
                ++start;
                if (--cnt[val] == 0) {
                    missing.push(val);
                }
                if (val == dup) break;
            }
        }
        while (!missing.empty() && skip[missing.top()]) {
            skip[missing.top()]--;
            missing.pop();
        }
        if (missing.empty()) {
            best = n;
        } else {
            best = max(best, missing.top());
        }
    }
    printf("%d\n", best);
}
