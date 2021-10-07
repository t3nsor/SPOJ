// 2021-10-07
#include <algorithm>
#include <climits>
#include <iostream>
#include <utility>
#include <stack>
#include <vector>
using namespace std;
void ansv_left(vector<int>& v, vector<int>& res) {
    stack<pair<int, int> > stk; stk.push(make_pair(INT_MIN, v.size()));
    for (int i = v.size()-1; i >= 0; i--) {
        while (stk.top().first > v[i]) {
            res[stk.top().second] = i; stk.pop();
        }
        stk.push(make_pair(v[i], i));
    }
    while (stk.top().second < v.size()) {
        res[stk.top().second] = -1; stk.pop();
    }
}

void ansv_right(vector<int>& v, vector<int>& res) {
    stack<pair<int, int> > stk; stk.push(make_pair(INT_MIN, -1));
    for (int i = 0; i < v.size(); i++) {
        while (stk.top().first > v[i]) {
            res[stk.top().second] = i; stk.pop();
        }
        stk.push(make_pair(v[i], i));
    }
    while (stk.top().second > -1) {
        res[stk.top().second] = v.size(); stk.pop();
    }
}
void do_testcase() {
    int n, r; cin >> n >> r;
    vector<int> xcoords = {0, n};
    vector<int> ycoords = {0, n};
    vector<int> x1(r), x2(r), y1(r), y2(r);
    for (int i = 0; i < r; i++) {
        cin >> x1[i] >> x2[i] >> y1[i] >> y2[i];
        xcoords.push_back(x1[i]);
        xcoords.push_back(x2[i]);
        ycoords.push_back(y1[i]);
        ycoords.push_back(y2[i]);
    }
    sort(xcoords.begin(), xcoords.end());
    sort(ycoords.begin(), ycoords.end());
    xcoords.erase(unique(xcoords.begin(), xcoords.end()), xcoords.end());
    ycoords.erase(unique(ycoords.begin(), ycoords.end()), ycoords.end());
    const auto x_to_cx = [&](int x) {
        return lower_bound(xcoords.begin(), xcoords.end(), x) -
               xcoords.begin();
    };
    const auto y_to_cy = [&](int y) {
        return lower_bound(ycoords.begin(), ycoords.end(), y) -
               ycoords.begin();
    };
    vector<vector<int>> darr(xcoords.size(), vector<int>(ycoords.size(), 0));
    for (int i = 0; i < r; i++) {
        const int cx1 = x_to_cx(x1[i]);
        const int cx2 = x_to_cx(x2[i]);
        const int cy1 = y_to_cy(y1[i]);
        const int cy2 = y_to_cy(y2[i]);
        darr[cx1][cy1]++;
        darr[cx1][cy2]--;
        darr[cx2][cy1]--;
        darr[cx2][cy2]++;
    }
    vector<int> dcol(ycoords.size() - 1, 0);
    for (int i = 0; i < xcoords.size() - 1; i++) {
        int psum = 0;
        for (int j = 0; j < darr[i].size() - 1; j++) {
            dcol[j] += darr[i][j];
            psum += dcol[j];
            darr[i][j] = psum;
        }
    }
    vector<int> empty_width(ycoords.size() - 1, 0);
    int result = 0;
    for (int i = 0; i < xcoords.size() - 1; i++) {
        for (int j = 0; j < ycoords.size() - 1; j++) {
            if (darr[i][j]) {
                empty_width[j] = 0;
            } else {
                empty_width[j] += xcoords[i + 1] - xcoords[i];
            }
        }
        vector<int> al(ycoords.size() - 1);
        vector<int> ar(ycoords.size() - 1);
        ansv_left(empty_width, al);
        ansv_right(empty_width, ar);
        for (int j = 0; j < ycoords.size() - 1; j++) {
            int full_height = ycoords[j + 1] - ycoords[j];
            full_height += ycoords[j] - ycoords[al[j] + 1];
            full_height += ycoords[ar[j]] - ycoords[j + 1];
            result = max(result, full_height * empty_width[j]);
        }
    }
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int d; cin >> d;
    while (d--) {
        do_testcase();
    }
}
