# 2024-04-04
# Use the Schur-Cohn algorithm, as suggested in the comments.
# https://en.wikipedia.org/wiki/Lehmer%E2%80%93Schur_algorithm#Schur-Cohn_algorithm
def parse_frac(w):
    sign = 1
    if w[0] == '-':
        sign = -1
        w = w[1:]
    parts = w.split('.')
    if len(parts) == 1 or len(parts[1]) == 0:
        return (sign * int(parts[0]), 1)
    if len(parts[0]) == 0:
        parts[0] = '0'
    den = 10**len(parts[1])
    return (sign * (den * int(parts[0]) + int(parts[1])), den)

def T(poly):
    if len(poly) == 0:
        return []
    a = [coeff * poly[0] for coeff in poly]
    b = [coeff * poly[-1] for coeff in reversed(poly)]
    c = [a[i] - b[i] for i in range(len(a))]
    while len(c) > 0 and c[-1] == 0:
        c.pop()
    return c

def deg(poly):
    return len(poly) - 1

num_testcases = int(input())
for testcase in range(num_testcases):
    input()
    # treat the coefficients as exact rationals, then scale up the polynomial so
    # that all coefficients are integers
    fracs = [parse_frac(w) for w in input().split()]
    max_den = max(frac[1] for frac in fracs)
    poly = list(frac[0] * max_den // frac[1] for frac in fracs)
    while len(poly) > 0 and poly[-1] == 0:
        poly.pop()
    if len(poly) <= 1:
        print('0')
        continue
    K = 0
    Tps = [poly]
    while len(Tps[-1]) > 0:
        Tps.append(T(Tps[-1]))
    if len(Tps) == 2:
        # in this case 1/z is a root whenever z is
        print('0')
        continue
    K = len(Tps) - 1
    Tps.pop()
    delta = [p[0] for p in Tps]
    if Tps[1][0] < 0 and all(p[0] > 0 for p in Tps[2:]) and deg(Tps[-1]) == 0:
        print('1')
    else:
        print('0')
