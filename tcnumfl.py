# 2024-04-04
# The small number of accepted users is intriguing. Maybe the time limit used
# to be a lot stricter under Pyramid.

import sys

N = int(input())
for testcase in range(N):
    words = input().split()
    b1 = int(words[1])
    b2 = int(words[2])
    prec = int(words[3])
    intfrac = words[0].split(',')
    if len(intfrac) == 1:
        int_ = intfrac[0]
        frac_ = ''
    else:
        int_ = intfrac[0]
        frac_ = intfrac[1]
    if len(int_) == 0:
        int_ = 0
    else:
        int_ = int(int_, b1)
    if len(frac_) == 0:
        num = 0
        den = 1
    else:
        num = int(frac_, b1)
        den = b1**len(frac_)
    l = []
    while int_ > 0:
        quot = int_ // b2
        l.append(int_ - b2 * quot)
        int_ = quot
    if len(l) == 0:
        l.append(0)
    l = ''.join('0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ'[i] for i in reversed(l))
    sys.stdout.write(l)
    sys.stdout.write(',')
    l = []
    for i in range(prec):
        num *= b2
        quot = num // den
        l.append(quot)
        num -= quot * den
    l = ''.join('0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ'[i] for i in l)
    sys.stdout.write(l)
    sys.stdout.write('\n')
