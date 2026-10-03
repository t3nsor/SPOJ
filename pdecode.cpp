// 2026-10-03
// the better solution is obviously to find the cycles but for some reason I
// decided I was too lazy to do it that way
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;
vector<int> mul(const vector<int>& a, const vector<int>& b) {
    vector<int> result(a.size());
    for (int i = 0; i < result.size(); i++) result[i] = a[b[i]];
    return result;
}
vector<int> power(const vector<int>& p, int e) {
    if (e == 1) return p;
    auto r = power(p, e/2);
    r = mul(r, r);
    if (e%2) {
        r = mul(r, p);
    }
    return r;
}
void do_testcase(int n, int m) {
    vector<int> perm(n);
    for (int i = 0; i < n; i++) { scanf("%d", &perm[i]); --perm[i]; }
    int c;
    do {
        c = getchar_unlocked();
    } while (c != '\r' && c != '\n');
    do {
        c = getchar_unlocked();
    } while (c == '\r' || c == '\n');
    string s;
    for (int i = 0; i < n; i++) {
        s.push_back(c);
        c = getchar_unlocked();
    }
    const auto pwr = power(perm, m);
    string result(n, 0);
    for (int i = 0; i < n; i++) result[pwr[i]] = s[i];
    printf("%s\n", result.c_str());
}
int main() {
    for (;;) {
        int n, m; scanf("%d %d", &n, &m);
        if (n == 0) break;
        do_testcase(n, m);
    }
}
