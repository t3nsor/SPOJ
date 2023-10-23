// 2023-10-22
#include <algorithm>
#include <functional>
#include <stdio.h>
#include <vector>
using namespace std;
void do_testcase() {
    int Q, nfields, nstrips; scanf("%d %d %d", &Q, &nfields, &nstrips);
    int field_sum = 0;
    vector<int> dp(Q + 1, 0);
    dp[0] = 1;
    for (int i = 0; i < nfields; i++) {
        int x; scanf("%d", &x);
        field_sum += x;
        for (int j = Q; j >= x; j--) {
            dp[j] = dp[j] || dp[j - x];
        }
    }
    vector<int> strips(nstrips);
    for (int i = 0; i < nstrips; i++) {
        scanf("%d", &strips[i]);
    }
    if (dp[Q]) {
        printf("%d\n", Q);
        return;
    }
    // Let m be the maximum number such that dp[m] is true. If m < Q, there
    // can't be any field left whose size is less than or equal to Q - m.
    if (field_sum >= Q) {
        // There is a field left whose size is greater than Q - m. We just take
        // as many trees from that field as we can.
        printf("%d\n", Q - 1);
        return;
    }
    // There are no fields left.
    int deficit = 0;
    sort(strips.begin(), strips.end(), greater<int>());
    for (const int strip : strips) {
        ++deficit;
        const int take = min(strip, Q - field_sum);
        field_sum += take;
        if (field_sum == Q) break;
    }
    printf("%d\n", Q - deficit);
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
