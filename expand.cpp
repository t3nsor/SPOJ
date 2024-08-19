// 2024-08-18
#include <algorithm>
#include <initializer_list>
#include <stdio.h>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;
bool can_expand(const vector<int>& vbegin, const vector<int>& vend,
                int zbegin, int zend) {
    const int cnt1 = lower_bound(vbegin.begin(), vbegin.end(), zbegin)
                     - vbegin.begin();
    const int cnt2 = lower_bound(vend.begin(), vend.end(), zbegin)
                     - vend.begin();
    if (cnt1 != cnt2) return false;
    const int cnt3 = upper_bound(vbegin.begin(), vbegin.end(), zend)
                     - vbegin.begin();
    return cnt3 == cnt1 + 1;
}
void do_testcase() {
    unordered_map<int, vector<int>> H1, H2, V1, V2;
    int n; scanf("%d", &n);
    vector<int> x1(n), y1(n), x2(n), y2(n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d %d", &x1[i], &y1[i], &x2[i], &y2[i]);
        if (x1[i] > x2[i]) swap(x1[i], x2[i]);
        if (y1[i] > y2[i]) swap(y1[i], y2[i]);
        H1[y1[i]].push_back(x1[i]);
        H2[y1[i]].push_back(x2[i]);
        H1[y2[i]].push_back(x1[i]);
        H2[y2[i]].push_back(x2[i]);
        V1[x1[i]].push_back(y1[i]);
        V2[x1[i]].push_back(y2[i]);
        V1[x2[i]].push_back(y1[i]);
        V2[x2[i]].push_back(y2[i]);
    }
    for (auto* m : {&H1, &H2, &V1, &V2}) {
        for (auto& v : *m) {
            sort(v.second.begin(), v.second.end());
        }
    }
    int result = 0;
    for (int i = 0; i < n; i++) {
        if (can_expand(H1[y1[i]], H2[y1[i]], x1[i], x2[i]) &&
            can_expand(H1[y2[i]], H2[y2[i]], x1[i], x2[i]) &&
            can_expand(V1[x1[i]], V2[x1[i]], y1[i], y2[i]) &&
            can_expand(V1[x2[i]], V2[x2[i]], y1[i], y2[i])) ++result;
    }
    printf("%d\n", result);
}
int main() {
    int T; scanf("%d", &T);
    while (T--) do_testcase();
}
