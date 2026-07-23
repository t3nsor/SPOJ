// 2026-07-23
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <utility>
#include <vector>
using namespace std;
constexpr int MAXDIST = 100;
string A, B;
int main() {
    ios::sync_with_stdio(false);
    int unused; cin >> unused >> unused;
    string A, B; cin >> A >> B;
    vector<int> row[3];  // row[0] is current row
    int ofs[3] = {0, 0, 0};
    for (int i = 1; i <= A.size(); i++) {
        ofs[0] = max(0, i - MAXDIST + 1);
        row[0].clear();
        auto entry = [&](int ii, int jj) {
                         if (ii == 0) return jj;
                         const auto& v = row[i - ii];
                         jj -= ofs[i - ii];
                         if (jj >= 0 && jj < v.size()) return v[jj];
                         else return MAXDIST;
                     };
        for (int j = ofs[0]; j <= min(i + MAXDIST - 1, (int)B.size()); j++) {
            int val = entry(i - 1, j) + 1;
            if (j > 0) {
                val = min(val, entry(i, j - 1) + 1);
                val = min(val, entry(i - 1, j - 1) + (A[i - 1] != B[j - 1]));
            }
            if (i >= 2 && j >= 2 &&
                A[i - 2] == B[j - 1] &&
                A[i - 1] == B[j - 2]) {
                val = min(val, entry(i - 2, j - 2) + 1);
            }
            row[0].push_back(val);
        }
        if (i == A.size()) {
            cout << entry(i, B.size()) << '\n';
            break;
        }
        row[2] = move(row[1]);
        ofs[2] = ofs[1];
        row[1] = move(row[0]);
        ofs[1] = ofs[0];
    }
}
