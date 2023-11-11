// 2023-11-11
// The suffix array code is copied from "Simple Linear Work Suffix Array
// Construction", so please contact the authors of that paper if you need a
// license for it.
#include <algorithm>
#include <deque>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <utility>
#include <vector>

#define MAX_STRING_LENGTH 1111111
#define GetI() (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2)

int leq2(int a1, int a2, int b1, int b2) {
    return a1 < b1 || a1 == b1 && a2 <= b2;
}

int leq3(int a1, int a2, int a3, int b1, int b2, int b3) {
    return a1 < b1 || a1 == b1 && leq2(a2,a3, b2,b3);
}

static void radix_pass(int* a, int* b, int* r, int n, int K) {
    static int c[MAX_STRING_LENGTH];
    int i, sum, t;
    for (i = 0;  i <= K;  i++) c[i] = 0;
    for (i = 0;  i < n;  i++) c[r[a[i]]]++;
    for (i = 0, sum = 0;  i <= K;  i++) {
        t = c[i];
        c[i] = sum;
        sum += t;
    }
    for (i = 0;  i < n;  i++) b[c[r[a[i]]]++] = a[i];
}
 
void suffix_array(int* s, int* SA, int n, int K) {
    s[n]=s[n+1]=s[n+2]=SA[n]=SA[n+1]=SA[n+2]=0;
    int n0 = (n+2)/3, n1 = (n+1)/3, n2 = n/3, n02 = n0+n2;
    std::vector<int> s12(n02 + 3);
    std::vector<int> SA12(n02 + 3);
    static int s0[(MAX_STRING_LENGTH+2)/3];
    static int SA0[(MAX_STRING_LENGTH+2)/3];
    int i, j, name, c0, c1, c2, p, t, k;
    s12[n02] = s12[n02+1] = s12[n02+2] = 0;
    SA12[n02] = SA12[n02+1] = SA12[n02+2] = 0;
    for (i=0, j=0; i < n+(n0-n1); i++) if (i%3 != 0) s12[j++] = i;
    radix_pass(s12.data(), SA12.data(), s+2, n02, K);
    radix_pass(SA12.data(), s12.data(), s+1, n02, K);
    radix_pass(s12.data(), SA12.data(), s, n02, K);
    name = 0, c0 = -1, c1 = -1, c2 = -1;
    for (i = 0; i < n02; i++) {
        if (s[SA12[i]] != c0 || s[SA12[i]+1] != c1 || s[SA12[i]+2] != c2) {
            name++;
            c0 = s[SA12[i]];
            c1 = s[SA12[i]+1];
            c2 = s[SA12[i]+2];
        }
        if (SA12[i] % 3 == 1) s12[SA12[i]/3] = name;
        else s12[SA12[i]/3 + n0] = name;
    }
    if (name < n02) {
        suffix_array(s12.data(), SA12.data(), n02, name);
        for (i = 0; i < n02; i++) s12[SA12[i]] = i + 1;
    }
    else for (i = 0; i < n02; i++) SA12[s12[i] - 1] = i;
    for (i=0, j=0; i < n02; i++) if (SA12[i] < n0) s0[j++] = 3*SA12[i];
    radix_pass(s0, SA0, s, n0, K);
    for (p=0, t=n0-n1, k=0; k < n; k++) {
        i = GetI();
        j = SA0[p];
        if (SA12[t] < n0 ?
                leq2(s[i], s12[SA12[t] + n0], s[j], s12[j/3]) :
                leq3(s[i], s[i+1], s12[SA12[t]-n0+1], s[j], s[j+1], s12[j/3+n0])) {
            SA[k] = i; t++;
            if (t == n02) for (k++; p < n0; p++, k++) SA[k] = SA0[p];
        }
        else {
            SA[k] = j; p++;
            if (p == n0) for (k++; t < n02; t++, k++) SA[k] = GetI();
        }
    }
}
char s[MAX_STRING_LENGTH];
int buf[MAX_STRING_LENGTH];
int sa[MAX_STRING_LENGTH];
int lcp[MAX_STRING_LENGTH];
int rank[MAX_STRING_LENGTH];
int where[MAX_STRING_LENGTH];
int main() {
    int N = 0;
    int nstr = 0;
    for (;;) {
        if (!fgets(s, MAX_STRING_LENGTH, stdin)) break;
        if (s[0] == '\n') {
            // empty string
            puts("0");
            return 0;
        }
        if (nstr > 0) {
            where[N] = -1;
            buf[N++] = nstr;
        }
        for (const char* p = s; *p != '\n'; ++p) {
            where[N] = nstr;
            buf[N++] = *p;
        }
        ++nstr;
    }
    if (nstr == 1) {
        printf("%d\n", N);
        return 0;
    }
    suffix_array(buf, sa, N, 128);
    for (int i = 0; i < N; i++) {
        rank[sa[i]] = i;
    }
    int k = 0;
    for (int i = 0; i < N; i++) {
        if (k > 0) --k;
        if (rank[i] == N - 1) {
            lcp[rank[i]] = k = 0;
            continue;
        }
        int j = sa[rank[i] + 1];
        while (i + k < N && j + k < N && buf[i + k] == buf[j + k]) ++k;
        lcp[rank[i]] = k;
    }
    int res = 0;
    // we have to do sliding window min here, unlike the 2 string version where
    // we just look for 2 adjacent suffixes from different strings
    int cnt[10] = {0};
    int nhave = 0;
    std::deque<std::pair<int, int>> Q;
    int j = 0;
    for (int i = 0; i < N; i++) {
        if (i > 0) {
            if (where[sa[i - 1]] >= 0) {
                if (0 == --cnt[where[sa[i - 1]]]) {
                    --nhave;
                }
            }
            if (Q.size() > 0 && Q.back().first == i - 1) {
                Q.pop_back();
            }
        }
        while (nhave < nstr && j < N) {
            if (where[sa[j]] >= 0) {
                if (0 == cnt[where[sa[j]]]++) {
                    ++nhave;
                }
            }
            if (j > i) {
                int val = lcp[j - 1];
                while (Q.size() > 0 && Q.front().second >= val) {
                    Q.pop_front();
                }
                Q.emplace_front(j - 1, val);
            }
            ++j;
        }
        if (nhave == nstr) {
            res = std::max(res, Q.back().second);
        }
    }
    printf("%d\n",res);
    return 0;
}
