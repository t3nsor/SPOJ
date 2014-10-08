// 2014-04-25
#include <cstdio>
#include <algorithm>
using namespace std;
int main() {
    int N;
    scanf("%d", &N);
    int x = 0, y = 0, z = 0;
    for (int i = 0; i < N; i++) {
        int a, b;
        scanf("%d/%d", &a, &b);
        if (a == 3) z++;
        else if (b == 4) x++;
        else y++;
    }
    int res = 0;
    res += z;
    x = max(0, x - z);
    res += y/2;
    y %= 2;
    res += x/4;
    x %= 4;
    if (y == 1) {
        if (x <= 2) {
            res += 1;
        } else {
            res += 2;
        }
    } else if (x >= 1) {
        res += 1;
    }
    printf("%d\n", res + 1);
}
