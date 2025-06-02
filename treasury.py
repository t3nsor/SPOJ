# 2025-06-01
# This is just the obvious DP solution, but it gets TLE because this problem
# setter likes to torture people with strict time limits. The solution that gets
# AC is treasury.hs.

def combine(p1, p2):
    if p1[0] > p2[0]:
        return p1
    elif p2[0] > p1[0]:
        return p2
    else:
        return (p1[0], p1[1] + p2[1])

def rec(children, u):
    child_results = [rec(children, v) for v in children[u]]
    p00 = 0
    p01 = 1
    mindiff = 2000
    minset = []
    for i in range(len(child_results)):
        p00 += child_results[i][1][0]
        p01 *= child_results[i][1][1]
        diff = child_results[i][1][0] - child_results[i][0][0]
        if diff < mindiff:
            mindiff = diff
            minset = [i]
        elif diff == mindiff:
            minset.append(i)
    in_minset = len(child_results) * [False]
    for v in minset: in_minset[v] = True
    if len(minset) == 0:
        p10 = 0
        p11 = 0
    else:
        p10 = 1 - mindiff
        p11 = 0
        prefix_prods = [1]
        for m in minset:
            prefix_prods.append(prefix_prods[-1] * child_results[m][1][1])
        suffix_prods = [1]
        for m in reversed(minset):
            suffix_prods.append(suffix_prods[-1] * child_results[m][1][1])
        suffix_prods.reverse()
        factor = 1
        for i in range(len(child_results)):
            p10 += child_results[i][1][0]
            if not in_minset[i]:
                factor *= child_results[i][1][1]
        for i in range(len(minset)):
            p11 += (prefix_prods[i] * child_results[minset[i]][0][1] *
                    suffix_prods[i + 1])
        p11 *= factor
    result = ((p00, p01), combine((p00, p01), (p10, p11)))
    return result

while True:
    try:
        V = int(input())
    except:
        break
    children = [[] for _ in range(V)]
    for _ in range(V):
        line = [int(w) - 1 for w in input().split()]
        children[line[0]] = line[2:]
    p0, p1 = rec(children, 0)
    print(p1[0])
    print(p1[1])
