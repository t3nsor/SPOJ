# 2021-10-11

def fact(n):
    return n * fact(n - 1) if n > 1 else 1

def calc(keys):
    l = list(filter(lambda x: x < keys[0], keys))
    r = list(filter(lambda x: x > keys[0], keys))
    result = fact(len(l) + len(r)) // fact(len(l)) // fact(len(r))
    if len(l) > 0:
        result *= calc(l)
    if len(r) > 0:
        result *= calc(r)
    return result

d = int(input())
for i in range(d):
    n = int(input())
    keys = [int(x) for x in input().split()]
    print(calc(keys))
