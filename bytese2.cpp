// 2014-05-01
#include <cstdio>
#include <algorithm>
#include <utility>
using namespace std;
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        int N; scanf("%d", &N);
        pair<int, int> a[200];
        for (int i = 0; i < N; i++) {
            int x, y; scanf("%d %d", &x, &y);
            a[2*i] = make_pair(x, 0); a[2*i+1] = make_pair(y, 1);
        }
        sort(a, a+2*N);
        int res = 0, cur = 0;
        for (int i = 0; i < 2*N; i++) {
            if (a[i].second) {
                cur--;
            } else {
                cur++;
            }
            res = max(res, cur);
        }
        printf("%d\n", res);
    }
    return 0;
}
