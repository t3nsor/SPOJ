# 2023-10-01
def get_rep(x):
    result = []
    while x != 0:
        if x % 2 == 0:
            result.append(0)
        else:
            result.append(1)
            x -= 1
        x //= -2;
    return result

def print_rep(rep):
    if len(rep) > 100:
        print('No')
        return
    i = len(rep) - 1
    used = []
    while i >= 0:
        if rep[i] > 0:
            used.append(str(i))
        i -= 1
    print(' '.join(used))

n = int(input())
for testcase in range(n):
    x = int(input())
    rep = get_rep(x)
    print_rep(rep)
    rep = get_rep(-x)
    print_rep(rep)
