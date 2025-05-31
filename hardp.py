# 2025-05-31
# So first, obviously, we look up 90,20,66 in OEIS to figure out what the
# problem is asking.
# You can then use Faulhaber's formula. Or, to make it even easier, you can use
# a trick I read in a book a long time ago but can't find on the internet: let
# S_k(n) be the polynomial that gives the sum of the first n kth powers. Then,
# we can obtain S_k(n) by multiplying S_{k-1}(n) by k, integrating with respect
# to n (choosing the result that has no constant term) and then adding a linear
# term that makes S_k(1) equal to 1.
from fractions import Fraction
import math

S = [[Fraction(0), Fraction(1)]]

for k in range(1, 51):
    Sk = [Fraction(0)]
    Sk.extend(k*a for a in S[-1])
    a1 = Fraction(1)
    for i in range(2, len(Sk)):
        Sk[i] = Sk[i] / i
        a1 -= Sk[i]
    Sk[1] = a1
    S.append(Sk)

denom = []

for i in range(51):
    denom.append(math.lcm(*(a.denominator for a in S[i])))
    ints = [(a*denom[-1]).numerator for a in S[i]]
    S[i] = ints

while True:
    try:
        N = int(input())
    except:
        break

    s = f'{N}={denom[N]}*(0'
    for i in range(2, N + 2, 2):
        s += f'{S[N][i]:+d}'
    for i in range(1, N + 2, 2):
        s += f'{S[N][i]:+d}'
    print(s + ')')
