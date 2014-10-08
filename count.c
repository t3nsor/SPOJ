/* 2014-04-29 */
#include <stdio.h>
#include <string.h>
// algorithm: number of partitions of n into exactly k parts
//          = number of partitions of n into parts of which the largest is k
//          = number of partitions of n into parts of size at most k
//          - number of partitions of n into parts of size at most k-1
//          = coefficient of x^n in 1/[(1-x)(1-x^2)...(1-x^k)]
//          - coefficient of x^n in 1/[(1-x)(1-x^2)...(1-x^{k-1})]
#define MOD 1988
static int dp[5001][5001]; // zeroed
int temp[5001];
int n, k;
int main() {
    dp[0][0] = 1;
    for (k = 1; k <= 5000; k++) {
        memset(temp, 0, k*sizeof(int));
        int nmodk = 0;
        for (n = 0; n <= 5000; n++) {
            temp[nmodk] = temp[nmodk] + dp[k-1][n];
            if (temp[nmodk] >= MOD) temp[nmodk] -= MOD;
            dp[k][n] = temp[nmodk];
            nmodk++; if (nmodk == k) nmodk = 0;
        }
    }
    for (;;) {
        scanf("%d %d", &n, &k);
        if (n == 0) return 0;
        printf("%d\n", (dp[k][n] - dp[k-1][n] + MOD)%MOD);
    }
}
