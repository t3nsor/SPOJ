# 2023-10-30
t = int(input())
for testcase in range(t):
    x = int(input())
    result = ''
    while x > 0:
        d = x % 7
        x //= 7
        if d == 0:
            result += '0'
        elif d == 1:
            result += '1'
        elif d == 2:
            result += '2'
        elif d == 3:
            result += '5'
        elif d == 4:
            result += '9'
        elif d == 5:
            result += '8'
        else:
            result += '6'
    print(result)
