# 2024-08-20 

import functools

@functools.cache
def calc(n, d):
    if d == 1: return 1
    return (calc(n, d - 1) + 1) ** n

while True:
    n, d = [int(w) for w in input().split()]
    if n == 0: break
    if d <= 1:
        print(f"{n} {d} 1")
    else:
        print(f"{n} {d} {calc(n, d) - calc(n, d - 1)}")
