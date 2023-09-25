// 2023-09-25
#include <algorithm>
#include <stdio.h>
#include <vector>
using namespace std;
int main() {
    int T; scanf("%d", &T);
    for (int i = 1; i <= T; i++) {
        int n; scanf("%d", &n);
        vector<int> coins(n);
        for (int j = 0; j < n; j++) {
            scanf("%d", &coins[j]);
        }
        sort(coins.begin(), coins.end());
        int result = 0;
        long long sum = 0;
        for (int i = 0; i < n - 1; i++) {
            if (sum + coins[i] < coins[i+1]) {
                sum += coins[i];
                ++result;
            }
        }
        if (n > 0) ++result;
        printf("Case #%d: %d\n", i, result);
    }
}
