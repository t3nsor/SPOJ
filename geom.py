# 2024-09-22
# The hardest part of this problem is remembering how to do anything in Python 2.
from fractions import Fraction

def linethru(P, Q):
    A = P[1] - Q[1]
    B = Q[0] - P[0]
    C = P[1] * Q[0] - P[0] * Q[1]
    return (A, B, C)

def perpline(P, l):
    A = l[1]
    B = -l[0]
    C = -(A*P[0] + B*P[1])
    return (A, B, C)

def line_eq(l1, l2):
    if l1[0] * l2[1] != l1[1] * l2[0]: return False
    if l1[0] != 0:
        return l1[0] * l2[2] == l1[2] * l2[0]
    else:
        return l1[1] * l2[2] == l1[2] * l2[1]

def insert(lines, l):
    for line in lines:
        if line_eq(line, l): return
    lines.append(l)

def intersect(l1, l2):
    det = l1[0] * l2[1] - l1[1] * l2[0]
    if det == 0: return None
    return ((l1[1] * l2[2] - l1[2] * l2[1]) / det,
            (l1[0] * l2[2] - l1[2] * l2[0]) / -det)

ox, oy = [int(w) for w in raw_input().split()]
s = int(raw_input())
A = (Fraction(2*ox + s, 2), Fraction(2*oy + s, 2))
B = (Fraction(2*ox + s, 2), Fraction(2*oy - s, 2))
C = (Fraction(2*ox - s, 2), Fraction(2*oy - s, 2))
D = (Fraction(2*ox - s, 2), Fraction(2*oy + s, 2))
px, py = [int(w) for w in raw_input().split()]
P = (Fraction(px, 1), Fraction(py, 1))
AP = linethru(A, P)
BP = linethru(B, P)
CP = linethru(C, P)
DP = linethru(D, P)
a = perpline(A, BP)
b = perpline(B, CP)
c = perpline(C, DP)
d = perpline(D, AP)

# remove identical lines
lines = [a]
insert(lines, b)
insert(lines, c)
insert(lines, d)

# find intersections
intersections = []
fail = False
for i in range(1, len(lines)):
    I = intersect(lines[0], lines[i])
    if I is None:
        fail = True
    intersections.append(I)

for i in range(1, len(intersections)):
    if intersections[i] != intersections[0]:
        fail = True
        break

if fail:
    print("NO")
else:
    print("YES")
    print("%.1f %.1f" % (intersections[0][0], intersections[0][1]))

