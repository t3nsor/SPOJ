// 2014-10-01
#include <iostream>
#include <cstring>
using namespace std;
int lsum[1<<17];
int rsum[1<<17];
int cnt[1<<15];
int pos[1<<15];
int buf[1<<17];
void radix_sort(int* begin, int* end) {
    for (int* i = begin; i < end; i++) {
        *i += 340000000;
        cnt[*i&((1<<15)-1)]++;
    }
    pos[0] = 0;
    for (int i = 1; i < (1<<15); i++) {
        pos[i] = pos[i-1] + cnt[i-1];
        cnt[i-1] = 0;
    }
    for (int* i = begin; i < end; i++) {
        buf[pos[*i&((1<<15)-1)]++] = *i;
        cnt[*i>>15]++;
    }
    pos[0] = 0;
    for (int i = 1; i < (1<<15); i++) {
        pos[i] = pos[i-1] + cnt[i-1];
        cnt[i-1] = 0;
    }
    for (int i = 0; i < end - begin; i++) {
        begin[pos[buf[i]>>15]++] = buf[i] - 340000000;
    }
}
int main() {
    int N, A, B; cin >> N >> A >> B;
    int lsize = N/2, rsize = (N+1)/2;
    int left[17], right[17];
    for (int i = 0; i < lsize; i++ ) {
        cin >> left[i];
    }
    for (int i = 0; i < rsize; i++) {
        cin >> right[i];
    }
    lsum[0] = 0;
    for (int i = 1; i < (1<<lsize); i++) {
        int tz = __builtin_ctz(i);
        lsum[i] = lsum[i-(1<<tz)] + left[tz];
    }
    rsum[0] = 0;
    for (int i = 1; i < (1<<rsize); i++) {
        int tz = __builtin_ctz(i);
        rsum[i] = rsum[i-(1<<tz)] + right[tz];
    }
    radix_sort(lsum, lsum + (1<<lsize));
    radix_sort(rsum, rsum + (1<<rsize));
    long long res = 0;
    int a = 1<<rsize, b = 1<<rsize;
    for (int i = 0; i < (1<<lsize); i++) {
        while (a > 0 && rsum[a-1] + lsum[i] >= A) {
            a--;
        }
        while (b > 0 && rsum[b-1] + lsum[i] > B) {
            b--;
        }
        res += b - a;
    }
    printf("%lld\n", res);
    return 0;
}
