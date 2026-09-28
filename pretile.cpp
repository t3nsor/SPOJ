// 2026-09-28
// nearly trivial application of Z-function
// why do some people have 0.00s? maybe they used even faster I/O, or maybe it's
// mostly a less precise measurement from the before times
#include <algorithm>
#include <stdio.h>
using namespace std;
using LL = long long;
char buf[100010];
int Z[100010];
void do_testcase() {
    fgets(buf, sizeof(buf), stdin);
    long long result = 1;
    for (int i = 1, l = 0, r = 0; buf[i] > 32; i++) {
        if (i < r) Z[i] = min(r - i, Z[i - l]); else Z[i] = 0;
        while (buf[Z[i]] == buf[i + Z[i]]) ++Z[i];
        if (i + Z[i] > r) { l = i; r = i + Z[i]; }
        result += Z[i] + i + 1;
    }
    printf("%lld\n", result);
}
int main() {
    int T; scanf("%d", &T);
    fgets(buf, sizeof(buf), stdin);  // eat trailing whitespace
    while (T--) do_testcase();
}
