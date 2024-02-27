// 2024-02-26
#include <iostream>
#include <iterator>
#include <set>
#include <vector>
using namespace std;
bool isPrime[10001];
bool rec(vector<int>& seq, set<int>& avail, int d) {
    if (avail.empty()) return true;
    auto it = avail.cbegin();
    while (it != avail.cend()) {
        const int val = *it;
        int sum = val;
        bool fail = false;
        for (int i = 2; i <= d && seq.size() >= i - 1; i++) {
            sum += seq[seq.size() - i + 1];
            if (isPrime[sum]) {
                fail = true;
                break;
            }
        }
        if (fail) {
            ++it;
            continue;
        }
        seq.push_back(val);
        avail.erase(it);
        if (rec(seq, avail, d)) return true;
        seq.pop_back();
        it = next(avail.insert(val).first);
    }
    return false;
}
int main() {
    for (int i = 2; i <= 10000; i++) {
        isPrime[i] = true;
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                isPrime[i] = false;
                break;
            }
        }
    }
    for (;;) {
        int n, m, d; cin >> n >> m >> d;
        if (n == 0) return 0;
        set<int> avail;
        for (int i = n; i <= m; i++) avail.insert(i);
        vector<int> seq;
        if (rec(seq, avail, d)) {
            for (int i = n; i <= m; i++) {
                if (i > n) cout << ',';
                cout << seq[i - n];
            }
        } else {
            cout << "No anti-prime sequence exists.";
        }
        cout << '\n';
    }
}
