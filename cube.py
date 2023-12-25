# 2023-12-25
t = int(input())
for _ in range(t):
    n, i = [int(w) for w in input().split()]
    print(n**i - (n-2)**i)
