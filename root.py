# 2023-12-24
import sys
def gcd(a, b):
    while (b != 0):
        t = b
        b = a % b
        a = t
    return a

def trivial(N, D):
    d = D;
    while d % 2 == 0:
        d = d // 2
    while d % 5 == 0:
        d = d // 5
    if d > 1:
        return False
    x = N
    while x:
        sys.stdout.write(chr(48 + 10*x//D))
        x = 10*x%D
    sys.stdout.write('\n')
    return True

def modinverse(x, m):
    A = list([0]) * 10000
    B = list([0]) * 10000
    C = list([0]) * 10000
    I = 2;
    A[0] = 1
    B[0] = 0
    C[0] = m
    A[1] = 0
    B[1] = 1
    C[1] = x
    while C[I - 1] > 1:
        k = C[I - 2] // C[I - 1]
        A[I] = A[I - 2] - k*A[I - 1]
        B[I] = B[I - 2] - k*B[I - 1]
        C[I] = C[I - 2] % C[I - 1]
        I += 1
    return (B[I - 1] + m) % m;

t = int(input())
for i in range(t):
    N, D = [int(w) for w in input().split()]
    if D == 0:
        print("Invalid Input!!!")
        sys.stdout.flush()
        continue
    if N % D == 0:
        print(str(N // D) + ".0")
        sys.stdout.flush()
        continue
    sys.stdout.write(str(N // D) + ".")
    N %= D
    f = gcd(N, D)
    N //= f
    D //= f
    if trivial(N, D):
        sys.stdout.flush()
        continue
    factor = 1
    d = D
    while d % 2 == 0:
        d //= 2
        factor *= 2
    while d % 5 == 0:
        d //= 5
        factor *= 5
    x = N
    nonrep = ""
    nonrepsize = 0
    while x % factor > 0:
        nonrep += chr(48 + 10*x//D)
        nonrepsize += 1
        x = 10 * x % D
    expect = x
    rep = ""
    repsize = 0
    while True:
        rep += chr(48 + 10*x//D)
        repsize += 1
        x = 10 * x % D
        if x == expect: break
    prefix = ""
    prefixsize = 0
    while nonrepsize > 0 and rep[repsize - 1] == nonrep[nonrepsize - 1]:
        nonrepsize -= 1
        repsize -= 1
        prefix = rep[repsize] + prefix
        prefixsize += 1
    sys.stdout.write(nonrep[0 : nonrepsize])
    sys.stdout.write('(')
    sys.stdout.write(prefix)
    sys.stdout.write(rep[0 : repsize])
    sys.stdout.write(")\n")
    sys.stdout.flush()
