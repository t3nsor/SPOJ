// 2024-09-07
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
void rec(const string& s, const vector<int>& lis, int i, string& cur) {
    if (lis[i] == 1) {
        cout << cur << '\n';
        return;
    }
    for (int j = s.size() - 1; j > i; j--) {
        if (s[j] >= s[i] && lis[j] == lis[i] - 1) {
            cur.push_back(s[j]);
            rec(s, lis, j, cur);
            cur.pop_back();
        }
    }
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        string s; cin >> s;
        vector<int> lis(s.size(), 1);
        int best = 1;
        for (int i = s.size() - 2; i >= 0; i--) {
            for (int j = i + 1; j < s.size(); j++) {
                if (s[j] >= s[i]) lis[i] = max(lis[i], lis[j] + 1);
            }
            best = max(best, lis[i]);
        }
        string cur;
        for (int i = s.size() - 1; i >= 0; i--) {
            if (lis[i] == best) {
                cur = s[i];
                rec(s, lis, i, cur);
            }
        }
        cout << '\n';
    }
}
