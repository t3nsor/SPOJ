// 2014-05-28
#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm>
using namespace std;
int main() {
    int t; scanf("%d", &t);
    while (t--) {
        int n; scanf("%d", &n);
        vector<pair<int, int> > intervals;
        for (int i = 0; i < n; i++) {
            int x, y; scanf("%d %d", &x, &y);
            intervals.push_back(make_pair(x, -y));
        }
        sort(intervals.begin(), intervals.end());
        vector<pair<int, int> > intervals2;
        for (int i = 0; i < n; i++) {
            intervals[i].second *= -1;
            while (intervals2.size() > 0 &&
                   intervals2.back().second >= intervals[i].second) {
                intervals2.pop_back();
            }
            intervals2.push_back(intervals[i]);
        }
        int res = 0, last = -1;
        for (int i = 0; i < intervals2.size(); i++) {
            if (intervals2[i].first >= last) {
                last = intervals2[i].second;
                res++;
            }
        }
        printf("%d\n", res);
    }
}
