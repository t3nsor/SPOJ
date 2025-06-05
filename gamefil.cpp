// 2025-06-05
// I didn't reuse the optimized solution from origlife.cpp because it needs to
// be changed pretty significantly to account for the fact that a cell can count
// multiple times.
#include <iostream>
#include <vector>
using namespace std;
constexpr int dr[8]={0,0,1,1,1,-1,-1,-1};
constexpr int dc[8]={1,-1,1,0,-1,1,0,-1};
void do_tc(int cs, int m, int n) {
    int board = 0;
    int live; cin >> live;
    while (live--) {
        int r, c; cin >> r >> c; board |= 1 << (r*n + c);
    }
    int result = 0;
    for (int i = 0; i < (1 << (m * n)); i++) {
        int succ = 0;
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < n; k++) {
                int cnt = 0;
                for (int p = 0; p < 8; p++) {
                    const int j2 = (j + dr[p] + m) % m;
                    const int k2 = (k + dc[p] + n) % n;
                    if (i & (1 << (j2*n + k2))) ++cnt;
                }
                if ((i & (1 << (j*n + k))) && (cnt == 2 || cnt == 3) ||
                    !(i & (1 << (j*n + k))) && cnt == 3) {
                    succ |= 1 << (j*n + k);
                }
            }
        }
        if (succ == board) ++result;
    }
    cout << "Case " << cs << ": ";
    if (result == 0) {
        cout << "Garden of Eden.\n";
    } else {
        cout << result << " possible ancestors.\n";
    }
}
int main() {
    for (int cs = 1;; ++cs) {
        int m, n; cin >> m >> n;
        if (m == 0) break;
        do_tc(cs, m, n);
    }
}
