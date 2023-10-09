# 2023-10-08
def N(k, x, y):
    if k == 0: return 0
    blocksize = 3**(k-1)
    blockx = x // blocksize
    blocky = y // blocksize
    x -= blocksize * blockx
    y -= blocksize * blocky
    if blockx == 0 and blocky == 0:
        return N(k-1, y, x)
    elif blockx == 1 and blocky == 0:
        return blocksize*blocksize + N(k-1, y, x)
    elif blockx == 2 and blocky == 0:
        return 2*blocksize*blocksize + N(k-1, x, y)
    elif blockx == 2 and blocky == 1:
        return 3*blocksize*blocksize + N(k-1, x, y)
    elif blockx == 2 and blocky == 2:
        return 4*blocksize*blocksize + N(k-1, x, y)
    elif blockx == 1 and blocky == 2:
        return 5*blocksize*blocksize + N(k-1, blocksize-x-1, blocksize-y-1)
    elif blockx == 1 and blocky == 1:
        return 6*blocksize*blocksize + N(k-1, blocksize-y-1, blocksize-x-1)
    elif blockx == 0 and blocky == 1:
        return 7*blocksize*blocksize + N(k-1, blocksize-y-1, blocksize-x-1)
    else:
        return 8*blocksize*blocksize + N(k-1, x, y)

t = int(input())
for testcase in range(t):
    k = int(input())
    x1, y1 = [int(x) - 1 for x in input().split()]
    x2, y2 = [int(x) - 1 for x in input().split()]
    print(N(k, x2, y2) - N(k, x1, y1))
