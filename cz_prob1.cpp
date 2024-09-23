// 2024-09-23
#include <iostream>
#include <vector>
using namespace std;
vector<int> primes;
void do_testcase() {
    int n, k; cin >> n >> k; --n;
    n = primes[n];
    vector<int> dp(n + 1, 0);
    dp[0] = 1;
    for (int i = 1; i <= k; i++) {
        for (int j = i; j <= n; j++) {
            dp[j] += dp[j - i];
        }
    }
    cout << dp[n] << '\n';
}
int main() {
    bool isComposite[8000] = {0};
    for (int i = 2; i < 8000; i++) {
        if (isComposite[i]) continue;
        if (i == 2 || (i % 4 == 1)) primes.push_back(i);
        for (int j = 2*i; j < 8000; j += i) {
            isComposite[j] = true;
        }
    }
    int T; cin >> T;
    while (T--) do_testcase();
}
