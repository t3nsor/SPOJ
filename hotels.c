/* 2014-04-30 */
#include <stdio.h>
int get() {
    int res = 0;
    int c;
    do {
        c = getchar_unlocked();
    } while (c <= 32);
    do {
        res = 10*res + c - '0';
        c = getchar_unlocked();
    } while (c > 32);
    return res;
}
int a[300000];
int main() {
    int N = get(), M = get();
    int best = 0, l = 0, sum = 0, i = 0;
    int n = (N + 7) / 8;
    switch (N % 8) {
        case 0: do {
            sum += a[i] = get();
            while (sum > M || sum < 0) {
                sum -= a[l++];
            }
            if (sum > best) best = sum;
            i++;
        case 7:
            sum += a[i] = get();
            while (sum > M || sum < 0) {
                sum -= a[l++];
            }
            if (sum > best) best = sum;
            i++;
        case 6:
            sum += a[i] = get();
            while (sum > M || sum < 0) {
                sum -= a[l++];
            }
            if (sum > best) best = sum;
            i++;
        case 5:
            sum += a[i] = get();
            while (sum > M || sum < 0) {
                sum -= a[l++];
            }
            if (sum > best) best = sum;
            i++;
        case 4:
            sum += a[i] = get();
            while (sum > M || sum < 0) {
                sum -= a[l++];
            }
            if (sum > best) best = sum;
            i++;
        case 3:
            sum += a[i] = get();
            while (sum > M || sum < 0) {
                sum -= a[l++];
            }
            if (sum > best) best = sum;
            i++;
        case 2:
            sum += a[i] = get();
            while (sum > M || sum < 0) {
                sum -= a[l++];
            }
            if (sum > best) best = sum;
            i++;
        case 1:
            sum += a[i] = get();
            while (sum > M || sum < 0) {
                sum -= a[l++];
            }
            if (sum > best) best = sum;
            i++;
        } while (--n > 0);
    }
    printf("%d\n", best);
    return 0;
}
