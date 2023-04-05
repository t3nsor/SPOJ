# 2023-04-05
# Surprisingly, the set of boards that can be won is a regular language. We can
# compute it by exhaustive case analysis. It helps to first convince oneself of
# the fact that the board is not winnable if it contains an interior sequence of
# three or more zeroes, or more than one interior sequence of two zeroes.
import re

patterns = list(re.compile('0*' + p + '0*') for p in
  ['1',
   '110',
   '11(01)*(11)*01',
   '11(01)*00(11)*(10)*11',
   '11(01)*1101(11)*(10)*11'])

t = int(input())
while t > 0:
    t -= 1
    n = int(input())  # unnecessary
    s = input()
    if (any([p.fullmatch(s) for p in patterns]) or
        any([p.fullmatch(s[::-1]) for p in patterns])):
        print("yes")
    else:
        print("no")
