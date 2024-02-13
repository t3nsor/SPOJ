// 2024-02-12
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
void update(int& bestv, int& bestc, int v, int c) {
    if (v > bestv) {
        bestv = v;
        bestc = c;
    } else if (v == bestv) {
        bestc += c;
    }
}
void do_testcase() {
    int R, C; cin >> R >> C;
    vector<vector<int>> a(R, vector<int>(C));
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            int x; cin >> x;
            a[i][j] = 2*x - 1;
        }
    }
    // calculate prefix sums in each row
    vector<vector<int>> rsum(R, vector<int>(C + 1));
    for (int i = 0; i < R; i++) {
        rsum[i][0] = 0;
        for (int j = 0; j < C; j++) {
            rsum[i][j + 1] = rsum[i][j] + a[i][j];
        }
    }
    int bestv = -1e6;
    int bestc;
    for (int j1 = 0; j1 + 1 <= C; j1++) {
        for (int j2 = j1 + 1; j2 <= C; j2++) {
            int incompletev = -1e6;
            int incompletec;
            for (int i = 0; i < R; i++) {
                const int row = rsum[i][j2] - rsum[i][j1];
                update(bestv, bestc, row, 1);
                update(bestv, bestc, row + incompletev, incompletec);
                const int new_incompletev =
                  incompletev + a[i][j1] + (j2 - 1 > j1 ? a[i][j2 - 1] : 0);
                if (row > new_incompletev) {
                    incompletev = row;
                    incompletec = 1;
                } else if (new_incompletev > row) {
                    incompletev = new_incompletev;
                } else {
                    incompletev = row;
                    incompletec++;
                }
            }
        }
    }
    cout << bestv << ' ' << bestc << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
