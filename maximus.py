# 2024-08-20

import functools
import sys

is_prime = [True] * 5001
for i in range(2, 5001):
    if is_prime[i]:
        j = 2*i
        while j <= 5000:
            is_prime[j] = False
            j += i

@functools.cache
def move(a, n, is_forward):
    if a > n: return 0
    if is_forward == is_prime[a]:
        return 1 + 2 * move(a + 1, n, not is_forward)
    else:
        return (2 + 2 * move(a + 1, n, is_forward) +
                move(a + 1, n, not is_forward))

sys.setrecursionlimit(1000000)

while True:
    N = int(input())
    if N == 0: break
    print(move(1, N, False))
