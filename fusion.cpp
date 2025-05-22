// 2025-05-22
#include <algorithm>
#include <iostream>
using namespace std;
int main() {
    int T; cin >> T;
    while (T--) {
        int K, N; cin >> K >> N;
        int result = N;
        for (int i = 0; i < K; i++) {
            int x, y, z; cin >> x >> y >> z;
            result = min(result, max({x, N - x, y, N - y, z, N - z}));
        }
        cout << result << '\n';
    }
}
