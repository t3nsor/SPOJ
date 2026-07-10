// 2026-07-10
// You can work out the idea from the sample cases.  For each cycle that needs
// to be executed, you either successively swap the smallest element `m` in the
// cycle, or you start by swapping `m` with the absolute smallest element in the
// entire array, use that element to execute the cycle, then swap `m` into place
// at the very end (which also puts the absolute smallest element back in its
// original place).  Sorry, I can't prove it (I'm a bit rusty with proofs).
#include <algorithm>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
int do_testcase(int n) {
    vector<int> v(n);
    vector<int> vis(n, 0);
    for (int i = 0; i < n; i++) scanf("%d", &v[i]);
    vector<int> s = v;
    sort(s.begin(), s.end());
    const int absm = s[0];
    int result = 0;
    for (int i = 0; i < n; i++) {
        if (vis[i]) continue;
        int m = 1001;
        int sum = 0;
        int j = i;
        int l = 0;
        do {
            l++;
            sum += v[j];
            m = min(m, v[j]);
            vis[j] = 1;
            j = lower_bound(s.begin(), s.end(), v[j]) - s.begin();
        } while (j != i);
        result += sum + min((l-2)*m, m+(l+1)*absm);
    }
    return result;
}
int main() {
    for (int i = 1;; i++) {
        int n; scanf("%d", &n);
        if (n == 0) break;
        printf("Case %d: %d\n\n", i, do_testcase(n));
    }
}
