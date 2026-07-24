// 2026-07-23
#include <algorithm>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
using LL = long long;
int N;
LL H[100000];
LL br[100000];
LL bl[100000];
void calc_br(LL* out) {
    // sequence of building indices with strictly increasing height
    vector<pair<int, LL>> stk = {{-1, 0}};
    LL area = 0;
    for (int i = 0; i < N; i++) {
        while (stk[stk.size() - 1].second >= H[i]) {
            area -= stk[stk.size() - 1].second *
                    (stk[stk.size() - 1].first - stk[stk.size() - 2].first);
            stk.pop_back();
        }
        area += H[i] * (i - stk[stk.size() - 1].first);
        out[i] = area;
        stk.emplace_back(i, H[i]);
    }
}
int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; i++) scanf("%lld", H + i);
    calc_br(br);
    reverse(H, H + N);
    calc_br(bl);
    reverse(bl, bl + N);
    LL result = bl[0];
    printf("%lld", result);
    for (int i = 1; i < N; i++) {
        result += bl[i] - br[i - 1];
        printf(" %lld", result);
    }
    putchar('\n');
}
