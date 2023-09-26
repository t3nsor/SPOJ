# 2023-09-25
def calc(n, x, y, memo):
    l = 1 << n
    if x == 0 and y == 0:
        return 3**n
    if x <= -l or x >= l or y <= -l or y >= l:
        return 0
    # put (x, y) into a canonical form using symmetry
    # (we get TLE without this step; I guess it prevents exponential blowup in
    # the number of states)
    if x == 0:
        # move to the positive x-axis
        if y < 0: y = -y
        x, y = y, x
    else:
        if x < 0:
            x = -x
            y = -y
        if y > x:
            x, y = y,x
    if (n, x, y) in memo:
        return memo[(n, x, y)]
    # split both matrices into the 3 submatrices that have holes
    result = 0
    for o1 in [(0, 0), (0, l >> 1), (l >> 1, 0)]:
        for o2 in [(0, 0), (0, l >> 1), (l >> 1, 0)]:
            result += calc(n - 1, x - o1[0] + o2[0], y - o1[1] + o2[1], memo)
    memo[(n, x, y)] = result
    return result

# handle arbitrary numbers of blank lines just in case
def get():
    while True:
        try:
            return int(input())
        except:
            pass

t = get()
for i in range(t):
    n = get()
    # flip across a vertical edge, so that each matrix is symmetric with respect
    # to exchange of the x and y coordinates; this is not strictly necessary,
    # but makes the picture a bit easier to reason about
    x = -get()
    y = get()
    memo = {}
    print(calc(n, x, y, memo))
