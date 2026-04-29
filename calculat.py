# 2026-04-29
# The hard part of this problem is computing the leading 15 digits of large
# factorials.  This is done using Stirling's approximation, but because the
# leading term is N log N, we need to be able to compute log N to enough
# decimal places to have about 15 decimal places accurate *after* we multiply
# the result by N (plus a couple extra to be safe).  (And then the actual
# implementation is easy if you know how to use `decimal`.)
from math import exp, floor, log
import decimal
decimal.getcontext().prec = 26
decimal.getcontext().rounding = decimal.ROUND_DOWN
pi = decimal.Decimal('3.141592653589793238462643')
ln10 = decimal.Decimal(10).ln()

small = [1]
for i in range(1, 405):
    small.append(i * small[-1])

T = int(input())
for _ in range(T):
    input()
    N, K, L = [int(n) for n in input().split()]
    if N < len(small):
        s = str(small[N])
        print(f"{s[:K]} {s[-L:]}")
    else:
        n = decimal.Decimal(N)
        l = (n*n.ln() - n + (2*pi*n).ln()/2 + 1/(12*n) - 1/(360*n**3) +
             1/(1260*n**5) - 1/(1680*n**7) +
             1/(1188*n**9) - 691/(360360*n**11)) / ln10
        a = (ln10*(l - round(l, 0))).exp()
        print(f"{(10**(K-1)*a).to_integral():0{K}} {L*'0'}")
