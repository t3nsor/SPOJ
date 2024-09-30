# 2024-09-29
# https://math.stackexchange.com/a/3888967 explains how to solve the optic
# equation. From this we eventually obtain that the number of solutions as
# ordered pairs (X, Y) is equal to (2e_1 + 1) * (2e_2 + 2) * ... where each e_i
# is the exponent of a prime factor of N.
isPrime = [True] * 10001
primes = []
for i in range(2, 10001):
    if not isPrime[i]: continue
    primes.append(i)
    j = 2*i
    while j <= 10000:
        isPrime[j] = False
        j += i

while True:
    N = int(input())
    if N == 0: break
    result = 1
    for p in primes:
        if p > N: break
        exponent = 0
        N2 = N
        while N2 > 0:
            N2 //= p
            exponent += N2
        result *= 2*exponent + 1
    print(result)
