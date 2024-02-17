// 2024-02-17
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
void do_testcase() {
    int n, X, Y; cin >> n >> X >> Y;
    vector<int> x1(n), x2(n), y1(n), y2(n);
    vector<int> allx = {0, X}, ally = {0, Y};
    for (int i = 0; i < n; i++) {
        cin >> x1[i] >> y1[i] >> x2[i] >> y2[i];
        allx.push_back(x1[i]);
        ally.push_back(y1[i]);
        allx.push_back(x2[i]);
        ally.push_back(y2[i]);
    }
    sort(allx.begin(), allx.end());
    allx.erase(unique(allx.begin(), allx.end()), allx.end());
    sort(ally.begin(), ally.end());
    ally.erase(unique(ally.begin(), ally.end()), ally.end());
    vector<vector<int>> cov(allx.size() + 1, vector<int>(ally.size() + 1, 0));
    for (int i = 0; i < n; i++) {
        const int x3 = lower_bound(allx.begin(), allx.end(), x1[i]) -
                       allx.begin();
        const int x4 = lower_bound(allx.begin(), allx.end(), x2[i]) -
                       allx.begin();
        const int y3 = lower_bound(ally.begin(), ally.end(), y1[i]) -
                       ally.begin();
        const int y4 = lower_bound(ally.begin(), ally.end(), y2[i]) -
                       ally.begin();
        cov[x3][y3]++;
        cov[x3][y4]--;
        cov[x4][y3]--;
        cov[x4][y4]++;
    }
    for (int pass = 0; pass < 2; pass++) {
        for (int i = 0; i < cov.size(); i++) {
            for (int j = 1; j < cov[i].size(); j++) {
                cov[i][j] += cov[i][j - 1];
            }
        }
        for (int i = 1; i < cov.size(); i++) {
            for (int j = 0; j < cov[i].size(); j++) {
                cov[i][j] += cov[i - 1][j];
            }
        }
    }
    // shift the rectangle sums so that the upper bound is exclusive
    for (int i = cov.size() - 1; i >= 0; i--) {
        for (int j = cov[i].size() - 1; j >= 0; j--) {
            cov[i][j] = (i > 0 && j > 0) ? cov[i - 1][j - 1] : 0;
        }
    }
    int x, y; cin >> x >> y;
    for (int j = 0; j < ally.size(); j++) {
        if (ally[j] + y > Y) continue;
        const int j2 = lower_bound(ally.begin(), ally.end(), ally[j] + y) -
                       ally.begin();
        for (int i = 0; i < allx.size(); i++) {
            if (allx[i] + x > X) continue;
            const int i2 = lower_bound(allx.begin(), allx.end(), allx[i] + x) -
                           allx.begin();
            if (cov[i2][j2] - cov[i2][j] - cov[i][j2] + cov[i][j] == 0) {
                cout << allx[i] << ' ' << ally[j] << '\n';
                return;
            }
        }
    }
    cout << "Fail!\n";
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
