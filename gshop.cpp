// 2014-01-25
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    int T;
    cin >> T;
    while (T--) {
        int n, k;
        cin >> n >> k;
        vector<int> pos, neg;
        for (int i = 0; i < n; i++) {
            int x;
            cin >> x;
            if (x >= 0) pos.push_back(x); else neg.push_back(x);
        }
        sort(pos.begin(), pos.end());
        sort(neg.begin(), neg.end());
        int sum = 0;
        for (int i = 0; i < pos.size(); i++) sum += pos[i];
        if (k <= neg.size()) {
            for (int i = 0; i < k; i++) sum -= neg[i];
            for (int i = k; i < neg.size(); i++) sum += neg[i];
        } else {
            for (int i = 0; i < neg.size(); i++) sum -= neg[i];
            k -= neg.size();
            if (k%2 == 1) {
                reverse(neg.begin(), neg.end());
                // pick one element to flip
                if (pos.size() == 0 || neg.size() != 0 && -neg[0] < pos[0]) {
                    sum += 2*neg[0];
                } else {
                    sum -= 2*pos[0];
                }
            }
        }
        cout << sum << endl;
    }
}
