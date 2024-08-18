// 2024-08-17
#include <iostream>
#include <vector>
using namespace std;
void do_testcase() {
    int N, S, Y; cin >> N >> S >> Y;
    long long C; cin >> C;
    vector<int> v(18);  // best value for given volume (1 ... 17)
    int lcm = 1;
    for (int i = 0; i < N; i++) {
        int W, V; cin >> W >> V;
        if (lcm % V) lcm *= V;
        if (v[V] < W) v[V] = W;
    }
    vector<long long> dp1(lcm + 1);
    for (int i = 1; i <= lcm; i++) {
        for (int j = 1; j <= 17; j++) {
            if (j <= i) dp1[i] = max(dp1[i], dp1[i - j] + v[j]);
        }
    }
    const auto V = [&](int ns) {
        const long long vol = ns * (long long)Y;
        return dp1[lcm] * (vol / lcm) + dp1[vol % lcm] - (ns - 1) * C;
    };
    vector<long long> dp2(S + 1);
    for (int i = 1; i <= S; i++) {
        for (int ns = 1; ns <= i; ns++) {
            dp2[i] = max(dp2[i], dp2[i - ns] + V(ns));
        }
    }
    cout << dp2[S] << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) do_testcase();
}
