# 2024-03-10
import sys

def G(A, B, b):
    if len(b) == 0 or A == B:
        return A
    if b[0] == '0':
        return G(A, (A + B) // 2, b[1:])
    else:
        return G((A + B) // 2 + 1, B, b[1:])

# main program starts here
K = int(input())
for testcase in range(K):
    M, N = [int(w) for w in input().split()]
    # dp[i][j] is the number of valid suffixes starting at position `i` where
    # the first entry is `j`
    dp = [[0] * M for i in range(N)]
    for i in range(N - 1, M): dp[N - 1][i] = 1
    for i in reversed(range(N - 1)):
        for j in range(i):
            dp[i][j] = 0
        dp[i][M - 1] = dp[i + 1][M - 1]
        for j in reversed(range(i, M - 1)):
            dp[i][j] = dp[i][j + 1] + dp[i + 1][j]
    T = sum(dp[0])
    b = input()[2:]
    index = G(1, T, b) - 1
    result = []
    j = 0
    for i in range(N):
        while j < M and dp[i][j] <= index:
            index -= dp[i][j]
            j += 1
        result.append(j + 1)
    for i in range(N):
        if i > 0:
            if i % 20 == 0:
                sys.stdout.write('\n')
            else:
                sys.stdout.write(' ')
        s = str(result[i])
        while len(s) < 3: s = ' ' + s
        sys.stdout.write(s)
    sys.stdout.write('\n')
