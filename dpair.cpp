// 2014-04-30
#include <cstdio>
using namespace std;
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
char buf[20];
void put(long long x) {
    int cnt = 0;
    do {
        int y = x/10;
        buf[cnt++] = x - 10*y + '0';
        x = y;
    } while (x);
    while (cnt--) putchar_unlocked(buf[cnt]);
    putchar_unlocked('\n');
}
int a[800001];
char cnt[1000002];
long long d[800001];
int main() {
    int N = get();
    for (int i = 0; i < N; i++) {
        a[i] = get();
    }
    int l = 0, r = 0;
    a[N] = 1000001; cnt[a[N]] = 1;
    while (l < N) {
        while (!cnt[a[r]]) {
            cnt[a[r]]++; r++;
        }
        d[r-l+1]--; cnt[a[l++]]--;
    }
    d[1] = N;
    long long acc = 0;
    for (int i = 1; i <= N; i++) {
        acc += d[i]; d[i] = d[i-1] + acc;
    }
    int Q = get();
    while (Q--) {
        int a = get(), b = get();
        put(d[b] - d[a-1]);
    }
}
