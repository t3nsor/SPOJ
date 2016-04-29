// 2016-04-29
// stupid problem doesn't allow C++11 :(
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    int N; cin >> N;
    int m = 1e9, M = -1;
    vector<int> v(N);
    for (int i = 0; i < N; i++) {
        cin >> v[i];
        m = min(m, v[i]);
        M = max(M, v[i]);
    }
    int res;
    if (m == M) {
        if (m == 0) {
            res = 0;
        } else if (m == N-1) {
            res = N;
        } else {
            res = -1;
        }
    } else if (M - m > 1) {
        res = -1;
    } else {
        if (M >= 1 && M < N) {
            int nM = 0;
            for (int i = 0; i < N; i++) {
                if (v[i] == M) nM++;
            }
            if (M + nM == N) {
                res = M;
            } else {
                res = -1;
            }
        } else {
            res = -1;
        }
    }
    cout << res << "\n";
}
