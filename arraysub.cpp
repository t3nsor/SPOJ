// 2014-04-30
#include <deque>
#include <iostream>
#include <cstdio>
#include <utility>
using namespace std;
int a[1000000];
int main() {
    int n, k;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", a+i);
    }
    scanf("%d", &k);
    deque<pair<int, int> > D;
    for (int r = 0; r < n; r++) {
        while (!D.empty() && a[r] >= D.front().first) {
            D.pop_front();
        }
        D.push_front(make_pair(a[r], r));
        while (D.back().second <= r - k) {
            D.pop_back();
        }
        if (r >= k) putchar(' ');
        if (r >= k-1) printf("%d", D.back().first);
    }
    putchar('\n');
}
