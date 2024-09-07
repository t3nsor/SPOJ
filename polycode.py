# 2024-09-07
import math
def bigsqrt(n):
    """Return the square root of n, rounded down."""
    # generate an integer estimate for sqrt(n)
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
    if x * x > n:
        while x * x > n:
            x -= 1
        return x
    else:
        while x * x <= n:
            x += 1
        return x - 1

def getrc(n):
    d = (bigsqrt(8 * n + 1) - 1) // 2
    low = (d * (d + 1)) // 2
    c = n - low
    r = d - c
    return (r, c)

while True:
    line = input()
    if line == '*': break
    z = int(line)
    N, z = getrc(z)
    x = []
    y = []
    for i in range(N - 1):
        xy, z = getrc(z)
        xy = getrc(xy)
        x.append(xy[0])
        y.append(xy[1])
    xy = getrc(z)
    x.append(xy[0])
    y.append(xy[1])
    a = abs(sum(x[i] * y[(i + 1) % N] - x[(i + 1) % N] * y[i]
                for i in range(N)))
    print(f"{a // 2}.{5 * (a % 2)}")
