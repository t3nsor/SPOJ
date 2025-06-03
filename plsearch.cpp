// 2025-06-03
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
void canonicalize(vector<pair<int, int>>& shape) {
    int xmin = 1e6;
    int ymin = 1e6;
    for (const auto xy : shape) {
        xmin = min(xmin, xy.first);
        ymin = min(ymin, xy.second);
    }
    for (auto& xy : shape) {
        xy.first -= xmin;
        xy.second -= ymin;
    }
}
void do_testcase(int n) {
    // read template
    vector<pair<int, int>> tmpl[8];
    int m; cin >> m;
    for (int i = 0; i < m; i++) {
        int x, y; cin >> x >> y;
        tmpl[0].emplace_back(x, y);
    }
    // generate all rotations of template
    for (int i = 1; i < 4; i++) {
        tmpl[i] = tmpl[i - 1];
        for (int j = 0; j < m; j++) {
            const int x = tmpl[i][j].second;
            const int y = -tmpl[i][j].first;
            tmpl[i][j] = make_pair(x, y);
        }
    }
    for (int i = 0; i < 4; i++) {
        canonicalize(tmpl[i]);
    }
    // generate all reverses of template
    for (int i = 4; i < 8; i++) {
        tmpl[i] = tmpl[i - 4];
        reverse(tmpl[i].begin(), tmpl[i].end());
    }

    for (int i = 0; i < n; i++) {
        vector<pair<int, int>> shape;
        cin >> m;
        for (int j = 0; j < m; j++) {
            int x, y; cin >> x >> y;
            shape.emplace_back(x, y);
        }
        canonicalize(shape);
        bool match = false;
        for (int j = 0; j < 8; j++) {
            if (shape == tmpl[j]) {
                match = true;
                break;
            }
        }
        if (match) cout << i + 1 << '\n';
    }

    cout << "+++++\n";
}
int main() {
    for (;;) {
        int n; cin >> n;
        if (n == 0) break;
        do_testcase(n);
    }
}
