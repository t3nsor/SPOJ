// 2025-08-28
#include <iostream>
using namespace std;
int main() {
    int N, k; cin >> N >> k;
    int coin[5];
    for (int i = 0; i < k; i++) {
        cin >> coin[i];
        if (i > 0) coin[i] += coin[i - 1];
    }
    bool dp[100001];
    dp[0] = true;
    for (int i = 1; i <= N; i++) {
        dp[i] = false;
        for (int j = 0; j < k; j++) {
            if (i >= coin[j]) dp[i] |= dp[i - coin[j]];
        }
    }
    puts(dp[N] ? "YES" : "NO");
}
