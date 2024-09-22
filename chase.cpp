// 2024-09-21
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;
int gcd(int x, int y) {
    if (x == 0) return y; else return gcd(y % x, x);
}
void do_testcase() {
    int N; cin >> N;
    vector<int> x(N), y(N);
    for (int i = 0; i < N; i++) cin >> x[i] >> y[i];
    int best = 0;
    for (int i = 0; i < N; i++) {
        unordered_map<long long, int> M;
        for (int j = 0; j < N; j++) {
            const int dx = x[j] - x[i];
            const int dy = y[j] - y[i];
            if (dy < 0 || (dy == 0 && dx <= 0)) continue;
            const int g = gcd(dx, dy);
            long long key = (((long long)(dx / g)) << 32) + (dy / g);
            M[key]++;
        }
        for (const auto& p : M) {
            best = max(best, p.second);
        }
    }
    cout << best << '\n';
}
int main() {
    int T; cin >> T;
    while (T--) do_testcase();
}
