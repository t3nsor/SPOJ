// 2024-10-29
// This is a brute force problem; we must optimize the meet-in-the-middle
// approach enough to get within the time limit. This solution uses templates to
// minimize the number of indirect loads. That's probably overkill though; I
// imagine a solution that only uses ordinary functions would be almost as fast.
// We use radix sort, which takes linear time. (The tricky part is how to get
// the negative numbers ordered before the positive ones; this took some tries
// to get right.)
#include <iostream>
#include <limits.h>
#include <string.h>
using namespace std;
unsigned pwr[151][31];
unsigned a[3375000], b[3375000];
unsigned k[6], p[6];
unsigned m;
template <unsigned* dest, int start, int len>
auto gen = []{};

template <unsigned* dest, int start>
auto gen<dest, start, 1> = [] {
    unsigned* cur = dest;
    for (unsigned x0 = 1; x0 <= m; x0++) {
        *cur++ = k[start] * pwr[x0][p[start]];
    }
};

template <unsigned* dest, int start>
auto gen<dest, start, 2> = [] {
    unsigned* cur = dest;
    for (unsigned x0 = 1; x0 <= m; x0++) {
        unsigned y = k[start] * pwr[x0][p[start]];
        for (unsigned x1 = 1; x1 <= m; x1++) {
            *cur++ = y + k[start + 1] * pwr[x1][p[start + 1]];
        }
    }
};

template <unsigned* dest, int start>
auto gen<dest, start, 3> = [] {
    unsigned* cur = dest;
    for (unsigned x0 = 1; x0 <= m; x0++) {
        unsigned y = k[start] * pwr[x0][p[start]];
        for (unsigned x1 = 1; x1 <= m; x1++) {
            unsigned z = y + k[start + 1] * pwr[x1][p[start + 1]];
            for (unsigned x2 = 1; x2 <= m; x2++) {
                *cur++ = z + k[start + 2] * pwr[x2][p[start + 2]];
            }
        }
    }
};

unsigned buf[3375000];
int ofs[65536];

template <unsigned* a>
void radixsort(int len) {
    memset(ofs, 0, sizeof(ofs));
    for (int i = 0; i < len; i++) {
        ++ofs[(a[i] + 1) & 0xffff];
    }
    ofs[0] = 0;
    for (int i = 1; i < 65536; i++) {
        ofs[i] += ofs[i - 1];
    }
    for (int i = 0; i < len; i++) {
        buf[ofs[a[i] & 0x0000ffff]++] = a[i];
    }
    memset(ofs, 0, sizeof(ofs));
    for (int i = 0; i < len; i++) {
        ++ofs[((buf[i] >> 16) + 1) & 0xffff];
    }
    ofs[32768] = 0;
    for (int i = 32769; i < 98304; i++) {
        ofs[i & 0xffff] += ofs[(i - 1) & 0xffff];
    }
    for (int i = 0; i < len; i++) {
        a[ofs[buf[i] >> 16]++] = buf[i];
    }
}

void do_testcase() {
    int n; cin >> n >> m;
    for (int i = 0; i < n; i++) {
        unsigned kk, pp; cin >> kk >> pp;
        if (kk == 0) {
            --i;
            --n;
        } else {
            k[i] = kk;
            p[i] = pp;
        }
    }
    if (m == 1) {
        unsigned ksum = 0;
        for (int i = 0; i < n; i++) ksum += k[i];
        cout << (int)(ksum == 0U) << '\n';
        return;
    }
    const int alen = pwr[m][n/2];
    const int blen = pwr[m][(n + 1)/2];
    if (n == 0) {
        gen<a, 0, 0>();
        gen<b, 0, 0>();
    } else if (n == 1) {
        gen<a, 0, 0>();
        gen<b, 0, 1>();
    } else if (n == 2) {
        gen<a, 0, 1>();
        gen<b, 1, 1>();
    } else if (n == 3) {
        gen<a, 0, 1>();
        gen<b, 1, 2>();
    } else if (n == 4) {
        gen<a, 0, 2>();
        gen<b, 2, 2>();
    } else if (n == 5) {
        gen<a, 0, 2>();
        gen<b, 2, 3>();
    } else {
        gen<a, 0, 3>();
        gen<b, 3, 3>();
    }
    radixsort<a>(alen);
    radixsort<b>(blen);
    int result = 0;
    int s = blen - 1, i = 0, j = 0;
    while (s >= 0) {
        int t = s - 1;
        while (t >= 0 && b[t] == b[s]) t--;
        while (i < alen && int(a[i] + b[s]) < 0) ++i;
        while (j < alen && int(a[j] + b[s]) <= 0) ++j;
        if (a[i] + b[s] == 0) {
            result += (j - i) * (s - t);
        }
        s = t;
    }
    cout << result << '\n';
}
int main() {
    for (int i = 1; i <= 150; i++) {
        pwr[i][0] = 1;
        for (int j = 1; j < 31; j++) {
            const long long nxt = pwr[i][j - 1]*(long long)i;
            if (nxt > INT_MAX) break;
            pwr[i][j] = nxt;
        }
    }
    int T; cin >> T;
    while (T--) do_testcase();
}
