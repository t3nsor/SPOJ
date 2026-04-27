# 2026-04-27
# Consider the call tree if we were to compute the result naively for a given
# input x.  For example, if x is 11 then it would look like this:
#
# 11 ----- 10 ----- 9 ---- 8 ---- 7 --- 6 --- 5 --- 4 --- 3
# |        |        |      |      |     |     |     |
# 11-pi -- 10-pi -- 9-pi - 8-pi - 7-pi  6-pi  5-pi  4-pi
# |        |        |      |
# 11-2pi - 10-2pi   9-2pi  8-2pi
# |
# 11-3pi
#
# In the above diagram, lines from a vertex to its neighbors immediately to its
# right and below indicate recursive calls, e.g. P(8 - pi) = P(7 - pi) + 
# P(8 - 2pi).  Notice that there's no line between e.g. 7-pi and 6-pi, because
# 7-pi < 4 and therefore the recursion terminates at 7-pi.  The lowest vertex
# in each column is a base case for the recursion, where P is 1, and it counts a
# number of times equal to the number of ways to reach it from the upper-left
# corner by going only right or down, which is just a binomial coefficient.  The
# rest is left as an exercise for the reader.

import sys
from math import pi

def do_testcase(x):
    if x <= 3: return 1
    p = x - 3
    q = 0
    binom = 1
    result = binom
    while True:
        if p > 0:
            binom = binom*p//(p+q)
            p -= 1
        else:
            break
        if x - p - q*pi > 3 + pi:
            binom = binom*(p+q+1)//(q+1)
            q += 1
        result += binom
    return result

def out(x):
    s = str(x)
    i = 0
    while i < len(s):
        print(s[i:i+50])
        i += 50

if __name__ == "__main__":
    sys.set_int_max_str_digits(5000)
    while True:
        x = int(input())
        if x == -1: break
        out(do_testcase(x))
