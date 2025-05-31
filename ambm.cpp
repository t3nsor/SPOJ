// 2025-05-30
// We don't really need `__int128`. It just makes it easier.
#include <deque>
#include <iostream>
#include <vector>
using namespace std;
void do_testcase() {
    long long N; cin >> N;
    int K; cin >> K;
    vector<long long> a(K);
    for (int i = 0; i < K; i++) cin >> a[i];
    vector<__int128> b(K);
    b[0] = a[0];
    for (int i = 1; i < K; i++) {
        b[i] = 2*b[i - 1] + a[i];
    }
    deque<int> result;
    for (int i = K - 1; i >= 0; i--) {
        if (b[i] <= N) {
            result.push_front(i);
            N -= b[i];
        }
    }
    if (N > 0) cout << "-1\n";
    else {
        cout << result[0] + 1;
        for (int i = 1; i < result.size(); i++) {
            cout << ' ' << result[i] + 1;
        }
        cout << '\n';
    }
}
int main() {
    int T; cin >> T;
    while (T--) do_testcase();
}
