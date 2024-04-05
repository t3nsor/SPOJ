# 2024-04-04
# Slow, but passes. Implementing `next_permutation` would give a faster solution
# but I'd rather spend time on more interesting things.

def generate_permutation(n, fact, rank):
    avail = []
    for i in range(1, n + 1):
        avail.append(i)
    perm = []
    for i in range(n, 0, -1):
        fact //= i
        idx = rank // fact
        rank -= fact * idx
        perm.append(avail[idx])
        avail.remove(avail[idx])
    return perm

t = int(input())
for testcase in range(t):
    n, rank, cnt = [int(w) for w in input().split()]
    fact = 1
    for i in range(1, n + 1):
        fact *= i
    for i in range(cnt):
        perm = generate_permutation(n, fact, rank)
        print(' '.join([str(x) for x in perm]))
        rank += 1
        if rank == fact:
            rank = 0
    print('')
