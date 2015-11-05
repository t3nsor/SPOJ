// 2015-11-04
#include <iostream>
#include <bitset>
#include <vector>
#include <cstdio>
using namespace std;
int main() {
    int T;
    cin >> T;
    for (int _cs = 1; _cs <= T; _cs++) {
        int N;
        cin >> N;
        int sum = 0;
        vector<int> x(N);
        for (int i = 0; i < N; i++) {
            cin >> x[i];
            sum += x[i];
        }
        printf("Case %d: ", _cs);
        if (sum%2 || N%2) {
            puts("No");
            continue;
        }
        vector<bitset<41>> dp(sum+1);
        dp[0][0] = 1;
        for (int i = 0; i < N; i++) {
            for (int j = dp.size() -1; j >= x[i]; j--) {
                dp[j] |= dp[j-x[i]] << 1;
            }
        }
        puts(dp[sum/2][N/2] ? "Yes" : "No");
    }
}
