# 2025-05-09
dp = [[1]+6*[0]]
for i in range(1, 501):
    a = dp[-1]
    b = 7*[0]
    for j in range(7):
        for k in range(10):
            if k != 7: b[(10*j+k)%7] += a[j]
    dp.append(b)

t = int(input())
for _ in range(t):
    n = int(input())
    print(10**n - 1 - sum(dp[n][1:]))
