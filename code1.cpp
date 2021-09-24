// 2021-09-24
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
void do_testcase() {
    int Xr, Xi, Br, Bi; cin >> Xr >> Xi >> Br >> Bi;
    const int sqnorm = Br * Br + Bi * Bi;
    vector<int> result;
    while (Xr != 0 || Xi != 0 && result.size() < 100) {
        // multiply by conjugate of B
        int zr = Xr * Br + Xi * Bi;
        int zi = Xi * Br - Xr * Bi;
        bool success = false;
        for (int i = 0; i * i < sqnorm; i++) {
            if (zr % sqnorm == 0 && zi % sqnorm == 0) {
                result.push_back(i);
                success = true;
                break;
            }
            zr -= Br;
            zi += Bi;
        }
        if (!success) break;
        Xr = zr / sqnorm;
        Xi = zi / sqnorm;
    }
    if (Xr == 0 && Xi == 0) {
        if (result.empty()) {
            cout << '0';
        } else {
            reverse(result.begin(), result.end());
            cout << result[0];
            for (int i = 1; i < result.size(); i++) {
                cout << ',' << result[i];
            }
        }
        cout << '\n';
    } else {
        cout << "The code cannot be decrypted.\n";
    }
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) {
        do_testcase();
    }
}
