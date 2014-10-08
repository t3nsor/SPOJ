// 2014-04-25
#include <iostream>
#include <algorithm>
#include <cstdlib>
using namespace std;
int a[1000], b[1000];
int main() {
    int T; cin >> T;
    while (T--) {
        int M, N; cin >> M;
        for (int i = 0; i < M; i++) {
            cin >> a[i];
        }
        cin >> N;
        for (int i = 0; i < N; i++) {
            cin >> b[i];
        }
        sort(a, a+M); sort(b, b+N);
        int i = 0, j = 0; int best = 1e9;
        while (i < M && j < N) {
            best = min(best, abs(a[i] - b[j]));
            if (a[i] <= b[j]) {
                i++;
            } else if (a[i] > b[j]) {
                j++;
            }
        }
        cout << best << endl;
    }
}
