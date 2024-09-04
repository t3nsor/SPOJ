// 2024-09-03
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;
int main() {
    int n; scanf("%d", &n);
    if (n == 1) {
        puts("0\n");
        return 0;
    }
    vector<int> indexof(n);
    for (int i = 0; i < n; i++) {
        int x; scanf("%d", &x); --x;
        indexof[x] = i;
    }
    unsigned cur = 0;
    int step = 0;
    vector<int> dstep(n);
    for (int i = 0; i < n; i++) {
        const int vec = (indexof[i] - i + n) % n;
        cur += min(vec, n - vec);
        if (n % 2 == 0) {
            if (vec > n/2) {
                step += 1;
                dstep[vec - n/2] -= 2;
                dstep[vec] += 2;
            } else if (vec == n/2) {
                step -= 1;
                dstep[n/2] += 2;
            } else if (vec > 0) {
                step -= 1;
                dstep[vec] += 2;
                dstep[vec + n/2] -= 2;
            } else {
                step += 1;
                dstep[n/2] -= 2;
            }
        } else {
            if (vec > (n + 1)/2) {
                step += 1;
                dstep[vec - (n + 1)/2] -= 1;
                dstep[vec - (n + 1)/2 + 1] -= 1;
                dstep[vec] += 2;
            } else if (vec == (n + 1) / 2) {
                dstep[1] -= 1;
                dstep[vec] += 2;
            } else if (vec > 0) {
                step -= 1;
                dstep[vec] += 2;
                dstep[vec + (n - 1)/2] -= 1;
                dstep[vec + (n + 1)/2] -= 1;
            } else {
                step += 1;
                dstep[(n - 1)/2] -= 1;
                dstep[(n + 1)/2] -= 1;
            }
        }
    }
    unsigned best = cur;
    int j = 0;
    for (int i = 1; i < n; i++) {
        cur += step;
        best = min(best, cur);
        step += dstep[i];
    }
    printf("%u\n", best);
}
