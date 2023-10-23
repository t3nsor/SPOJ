#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    int N; cin >> N;
    int sum = 0;
    int cnt[100];
    int cal[100];
    for (int i = 0; i < N; i++) {
        cin >> cnt[i] >> cal[i];
        sum += cnt[i] * cal[i];
    }
    int MAX = sum / 2;
    vector<char> dp(MAX + 1);
    dp[0] = 1;
    int best = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < cal[i]; j++) {
            // j is the remainder
            int last = -1000000;
            for (int k = j; k <= MAX; k += cal[i]) {
                if (dp[k]) {
                    last = k;
                } else if (k - last <= cnt[i] * cal[i]) {
                    dp[k] = 1;
                }
                if (dp[k]) best = max(best, k);
            }
        }
    }
    cout << 2 * (MAX - best) + sum % 2 << '\n';
}
