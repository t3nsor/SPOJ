# 2026-08-02
import sys
while True:
    n = int(input())
    if n == 0: sys.exit(0)
    p = [int(x) for x in input().split()]
    dp = [(n+1)*[0] for _ in range(n+1)]
    dp[1][0] = 1
    for i in range(2, n+1):
        for j in range(i):
            if p[i-1] > p[i-2]:
                dp[i][j] = sum(dp[i-1][k] for k in range(j))
            else:
                dp[i][j] = sum(dp[i-1][k] for k in range(j, i))
    print(sum(dp[n]))
