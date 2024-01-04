# 2024-01-03
import decimal
from decimal import Decimal
import math
num_testcases = int(input())
for testcase in range(num_testcases):
    while True:
        try:
            n = int(input())
            break
        except:
            pass

    # scale the input by 10^30, so that we reduce the problem to that of finding
    # the cube root truncated to the nearest integer
    n *= 10**30

    # generate an integer estimate for n^(1/3)
    l = math.log10(n) / 3
    l2 = 0
    while l >= 10:
        l -= 10
        l2 += 10
    x = int(10**l) * 10**l2

    # improve the approximation using Newton's method; stop when
    # we get into a cycle
    s = set()
    while not (x in s):
        s.add(x)
        x = (2*x*x*x + n) // (3*x*x)

    # adjust until we reach the final result
    while x**3 > n:
        x -= 1

    while (x+1)**3 <= n:
        x += 1

    # Divide the result by 10^10 and format it to 10 decimal places. There is
    # probably a better way to write this :(
    decimal.getcontext().prec = 100
    d = (Decimal(x) / 10**10).quantize(Decimal(10) ** -10)
    s = '{}'.format(d)
    digitsum = 0
    for c in s:
        if c >= '0' and c <= '9':
            digitsum += int(c)
    print('{} {}'.format(digitsum % 10, s))
