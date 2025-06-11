# 2025-06-11
# See https://en.wikipedia.org/wiki/Chakravala_method
# Apparently this gives the minimal solution (I don't know the proof though)

# `extended_euclid` taken from U of T codebook
import math

def extended_euclid(a, b):
    """Return (g, x, y) where g = gcd(a, b) = ax + by"""
    x = 1
    y = 0
    xx = 0
    yy = 1
    while b != 0:
        q = a // b
        a, b = b, a % b
        x, xx = xx, x - q*xx
        y, yy = yy, y - q*yy
    return (abs(a), x, y)

t = int(input())
for _ in range(t):
    n = int(input())
    # choose initial (a, b, k) such that a^2 - nb^2 = k and k != 0, e.g., take
    # b = 1 and a^2 at least n + 1
    b = 1
    a = math.ceil(math.sqrt(n + 1))
    k = a*a - n
    while k != 1:
        # choose m such that bm = -a (mod |k|)
        mod = abs(k)
        g, _, y = extended_euclid(-mod, b)
        # by = g (mod |k|)
        assert a % g == 0
        m = (y * (-a // g)) % mod
        # minimize |m^2 - n| by adding a multiple of |k|/g to m, keeping m
        # positive
        period = mod // g
        if m*m <= n:
            rt = math.floor(math.sqrt(n))
            m1 = m + period * ((rt - m) // period)
            m2 = m1 + period
        else:
            rt = math.ceil(math.sqrt(n))
            m1 = m - period * ((m - rt) // period)
            m2 = m1 - period
        if (m2 <= 0) or (abs(m1*m1 - n) < abs(m2*m2 - n)):
            m = m1
        else:
            m = m2
        a, b = (a*m + n*b) // mod, (a + b*m) // mod
        k = (m*m - n) // k
    print(f'{a} {b}')
