// 2024-02-13
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
bool rec(int rails_left, vector<int>& xs, const vector<int>& a) {
    if (rails_left == 0) {
        static vector<char> need(5001);
        for (const int g : a) {
            need[g] = 1;
        }
        for (int i = 0; i < xs.size(); i++) {
            for (int j = 0; j < i; j++) {
                const int g = abs(xs[i] - xs[j]);
                if (g >= 0 && g <= 5000) need[g] = 0;
            }
        }
        for (const int g : a) {
            if (need[g]) return false;
        }
        return true;
    }
    for (int i = 0; i < xs.size(); i++) {
        const int x = xs[i];
        for (int j = 0; j < a.size(); j++) {
            xs.push_back(x + a[j]);
            if (rec(rails_left - 1, xs, a)) {
                return true;
            } else {
                xs.pop_back();
            }
            if (x < a[j]) continue;
            xs.push_back(x - a[j]);
            if (rec(rails_left - 1, xs, a)) {
                return true;
            } else {
                xs.pop_back();
            }
        }
    }
    return false;
}
void do_testcase() {
    int n; cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int nrails = 1; nrails <= 4; nrails++) {
        vector<int> xs = {0};
        if (rec(nrails, xs, a)) {
            sort(xs.begin(), xs.end());
            cout << nrails + 1 << ":";
            for (const int x : xs) {
                cout << ' ' << x;
            }
            cout << '\n';
            return;
        }
    }
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    for (int i = 1; i <= t; i++) {
        cout << "Scenario #" << i << '\n';
        do_testcase();
        cout << '\n';
    }
}
