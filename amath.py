# 2025-05-31
# Write n = 2^p 5^q r. It's easy to prove that if 1 <= p < k or 1 <= q < k,
# n^{1+T} - n can never be divisible by 10^k. Also, we have the trivial case
# where p, q >= k and we can simply take T = 1. This leaves two interesting
# cases: (1) p = q = 0 or (2) either p or q is 0 and the other is >= k.
#
# If p = q = 0, n^{a+T}-n^a = n^a(n^T - 1) can be divisible by 10^k only if
# n^T - 1 is divisible by 10^k, that is, n^T = 1 (mod 10^k). Because n is
# relatively prime to 10^k, we have n^{phi(10^k)} = 1 (mod 10^k) and the minimal
# T divides phi(10^k) and all other possible T. Since phi(10^k) has a known
# factorization, we can simply binary search to find a, b where T = 2^a 5^b.
#
# In case 2 if p = 0 and q >= k, then n^a already supplies all necessary factors
# of 5 and we need n^T = 1 (mod 2^k), which is solved in a similar fashion. If
# p >= k and q = 0, then we need n^T = 1 (mod 5^k).

def modexp(b, e, m):
    if e == 0: return 1
    r = modexp(b, e//2, m)
    r = (r * r) % m
    if e % 2 == 1: r = (r * b) % m
    return r

def calc(n, m, p, q):
    """
    Finds the smallest positive T such that n^T = 1 (mod m).
    p, q must be such that 2^p 5^q = phi(m).
    """
    l = 0
    r = p
    while r > l:
        mid = (l + r) // 2
        if modexp(n, 2**mid * 5**q, m) == 1:
            r = mid
        else:
            l = mid + 1
    res = 2**l
    l = 0
    r = q
    while r > l:
        mid = (l + r) // 2
        if modexp(n, res * 5**mid, m) == 1:
            r = mid
        else:
            l = mid + 1
    return res * 5**l

while True:
    try:
        n, k = [int(w) for w in input().split()]
    except:
        break
    p = 0
    q = 0
    r = n
    while r % 2 == 0:
        p += 1
        r /= 2
    while r % 5 == 0:
        q += 1
        r /= 5
    if (p >= 1 and p < k) or (q >= 1 and q < k):
        print(-1)
        continue
    if p == 0 and q == 0:
        m = 10**k
        p = k + 1
        q = k - 1
    elif p == 0:
        m = 2**k
        p = k - 1
        q = 0
    else:
        m = 5**k
        p = 2
        q = k - 1
    print(calc(n, m, p, q))
