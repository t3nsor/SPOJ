# 2026-04-12
# translated to Python 3 from cfrac.cpp and cfrac2.cpp

import math

def cfrac(p, q):
    v = []
    while True:
        g = math.gcd(p, q)
        p //= g;
        q //= g;
        if q == 1: break
        v.append(p // q)
        p, q = q, p % q
    v.append(p - 1)
    width = 3*len(v) + 1
    for x in v:
        width += len(str(x))
    result = [['.' for _ in range(width)] for _ in range(2*len(v) + 1)]
    col = 0
    for i in range(len(v)):
        line = 2*i + 1
        s = str(v[i])
        for c in s:
            result[line][col] = c
            col += 1
        result[line][col] = '.'
        result[line][col + 1] = '+'
        result[line][col + 2] = '.'
        col += 3
        for col2 in range(col, width): result[line][col2] = '-'
        onepos = (col + (width - 1)) // 2
        result[line - 1][onepos] = '1'
    result[-1][-1] = '1'
    for l in result: print(''.join(l))

def cfrac2(inp):
    a = []
    for i in range(len(inp)):
        if i % 2 != 0:
            j = 0
            while inp[i][j] == '.': j += 1
            k = j + 1
            while inp[i][k] != '.': k += 1
            a.append(int(inp[i][j:k]))
    p = 1
    q = 1
    for i in range(len(a) - 1, -1, -1):
        p, q = q, p
        p += a[i] * q
    print(f'{p} {q}')

kind = None
cnum = 0
while True:
    cnum += 1
    if kind == None: kind = input()
    if kind == 'C': break
    if kind == 'A':
        print(f'Case {cnum}:')
        cfrac(*(int(word) for word in input().split()))
        kind = None
    else:
        inp = []
        while True:
            line = input()
            if line == 'A' or line == 'B' or line == 'C':
                kind = line
                break
            inp.append(line)
        print(f'Case {cnum}:')
        cfrac2(inp)
