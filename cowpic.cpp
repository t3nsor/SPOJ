// 2017-04-14
#include <stdio.h>
#include <algorithm>
int cow[111111];
int pos[111111];
int buf[111111];
long long mergesort(int* begin, int* end) {
    int n = end - begin;
    if (n == 1) return 0;
    int l_size = n/2;
    int r_size = n - l_size;
    int* r_begin = begin + l_size;
    long long result =
        mergesort(begin, r_begin) + mergesort(r_begin, end);
    int next_l = 0;
    int next_r = 0;
    int next = 0;
    while (next < n) {
        if (next_r < r_size &&
            (next_l == l_size || r_begin[next_r] < begin[next_l])) {
            result += l_size - next_l;
            buf[next++] = r_begin[next_r++];
        } else {
            buf[next++] = begin[next_l++];
        }
    }
    std::copy(buf, buf + n, begin);
    return result;
}
int main() {
    int N; scanf("%d", &N);
    for (int i = 1; i <= N; i++) {
        scanf("%d", cow + i);
        pos[cow[i]] = i;
    }
    long long inv = mergesort(cow + 1, cow + N + 1);
    long long best = inv;
    for (int i = N; i >= 2; i--) {
        inv += 2*pos[i] - N - 1;
        best = std::min(inv, best);
    }
    printf("%lld\n", best);
}
