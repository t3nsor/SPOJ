// 2014-05-02
#include <iostream>
#include <string>
#include <cstring>
using namespace std;
int dp[100001];
int sum[100001];
int last[100];
const int MOD = 1000000007;
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) {
        string s; cin >> s;
        dp[0] = 1; sum[0] = 1;
        memset(last, -1, sizeof last);
        for (int i = 0; i < s.length(); i++) {
            dp[i+1] = sum[i];
            if (~last[s[i]]) {
                dp[i+1] = (dp[i+1] - sum[last[s[i]]] + MOD)%MOD;
            }
            sum[i+1] = (sum[i] + dp[i+1])%MOD;
            last[s[i]] = i;
        }
        cout << sum[s.length()] << endl;
    }
    return 0;
}
