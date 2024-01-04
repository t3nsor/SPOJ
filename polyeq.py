# 2024-01-03

import re
from decimal import Decimal

def signof(x):
    assert(isinstance(x, int))

    if x > 0:
        return 1
    elif x == 0:
        return 0
    else:
        return -1

class Poly(list):
    @classmethod
    def _parse_monomial(cls, monomial):
        m = re.fullmatch('(?:([0-9]+)\*)?x(?:\^([0-9]))?', monomial)
        if m is None:
            coeff = int(monomial)
            exponent = 0
        else:
            coeff = 1 if m.group(1) is None else int(m.group(1))
            exponent = 1 if m.group(2) is None else int(m.group(2))
        return (coeff, exponent)

    @classmethod
    def parse(cls, s):
        result = Poly()
        l = re.split('([-+])', s)
        if l[0] == '':
            # For something like `-x^2` the result would be:
            # ['', '-', 'x^2']
            # This also handles the case of a leading plus sign.
            l = l[1:]
        else:
            # If there's no leading sign, the leading sign is '+'.
            l = ['+'] + l
        i = 0
        while i < len(l):
            sgn = 1 if l[i] == '+' else -1
            (coeff, exponent) = cls._parse_monomial(l[i + 1])
            while len(result) <= exponent:
                result.append(0)
            result[exponent] = sgn * coeff
            i += 2
        while result[-1] == 0:
            result.pop()
        return result

    def degree(self):
        return len(self) - 1

    def evaluate(self, x):
        power = 1
        result = 0
        for coefficient in self:
            result += coefficient * power
            power *= x
        return result

    def shift(self, a):
        # Returns P such that P.evaluate(x) == self.evaluate(x + a).
        result = Poly(len(self) * [0])
        for e in range(len(self)):
            c = self[e]
            # The monomial c * x^e becomes c * (x + a)^e
            binom = 1
            for i in range(e + 1):
                if i > 0:
                    binom = binom * (e - i + 1) // i
                result[i] += c * binom * a ** (e - i)
        return result

    def sign_changes(self):
        result = 0
        current_sign = 0
        for coeff in self:
            if coeff != 0:
                if current_sign != 0 and current_sign != signof(coeff):
                    result += 1
                current_sign = signof(coeff)
        return result

def find_roots(P, n, lower, upper):
    # Find `n` roots of `P` that fall within [lower, upper], with multiplicity.
    # For a root that is not an integer, either the floor or ceiling will be
    # returned.

    if upper - lower <= 1 or n == 0:
        return n * [lower]

    # Using Descartes's rule of signs, count the number of roots that are
    # strictly greater than `upper`.
    num_gt_upper_roots = P.shift(upper).sign_changes()

    mid = (lower + upper) // 2
    # Now count the number of roots that are strictly greater than `mid`.
    num_gt_mid_roots = P.shift(mid).sign_changes()

    big_root_list = []
    small_root_list = []
    num_big_roots = num_gt_mid_roots - num_gt_upper_roots
    num_small_roots = n - num_big_roots
    if num_big_roots > 0:
        big_root_list = find_roots(P, num_big_roots, mid + 1, upper)
    if num_small_roots > 0:
        small_root_list = find_roots(P, num_small_roots, lower, mid)

    return small_root_list + big_root_list

t = int(input())
for testcase_num in range(t):
    P = Poly.parse(input())

    # Transform `P` such that `x` is a root of the new `P` if `x/100` is a root
    # of the original `P`.
    for i in range(P.degree()):
        P[i] *= 100 ** (P.degree() - i)

    roots = find_roots(P, P.degree(), -10000, 10000)

    print(' '.join(['{}'.format((Decimal(r) / 100).quantize(Decimal(10) ** -2))
                    for r in roots]))
