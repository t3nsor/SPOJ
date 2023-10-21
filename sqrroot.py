# 2023-10-20
import math
num_testcases = int(input())
for testcase in range(num_testcases):
    n = int(input())
    # generate an integer estimate for sqrt(x)
    l = math.log10(n) / 2
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
        x = (x * x + n) // (2 * x)
    # adjust until we reach the final result
    while x * x > n:
        x -= 1
    while x * x < n:
        x += 1
    print(x)
