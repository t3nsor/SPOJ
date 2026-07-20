# 2026-07-20
# Same concept as CONTPACK, but more confusing problem description.
# As mentioned in the comments, each (X, Y) pair in the input describes a single
# stone, not a type of stone that you can use arbitrarily many of.
def do_testcase():
    stones = []
    for _ in range(int(input())):
        X, Y = [int(word) for word in input().split()]
        while X >= len(stones): stones.append([])
        stones[X].append(Y)
    towers = [int(input()) for _ in range(int(input()))]
    remaining = sum(towers)
    exponent = 0
    result = 0
    while remaining > 0:
        if exponent >= len(stones): return -1
        fail = True
        need = []
        for i in range(len(towers)):
            if (1 << exponent) <= towers[i]: fail = False
            if (towers[i] & (1 << exponent)) != 0: need.append(i)
        if fail: return -1
        if len(stones[exponent]) < len(need): return -1
        stones[exponent].sort()
        for i in range(len(need)):
            j = need[i]
            result += stones[exponent][-1]
            towers[j] -= (1 << exponent)
            remaining -= (1 << exponent)
            stones[exponent].pop()
        if len(stones[exponent]) >= 2 and len(stones) <= exponent + 1:
            stones.append([])
        while len(stones[exponent]) >= 2:
            stones[exponent + 1].append(stones[exponent][-1] +
                                        stones[exponent][-2])
            stones[exponent].pop()
            stones[exponent].pop()
        exponent += 1
    return result

if __name__ == "__main__":
    for _ in range(int(input())):
        ans = do_testcase()
        if ans < 0:
            print('Plan Failed!')
        else:
            print(ans)
