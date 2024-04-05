# 2024-04-05
while True:
    try:
        N = int(input())
    except:
        break
    L = 1
    while N != 1:
        L += 1
        if N % 2 == 0:
            N //= 2
        else:
            N = 3*N + 1
    print(L)
