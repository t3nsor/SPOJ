c = int(input())
while c > 0:
    c -= 1
    n = int(input())
    power2 = 1
    while power2 <= n:
        power2 <<= 1
    result = 0
    while n > 0:
        while n < (power2 << 1) - 1: power2 >>= 1
        result += 1
        n -= power2
    print(result)
