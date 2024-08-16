// 2024-08-16
#include <iostream>
#include <string>
using namespace std;
void do_testcase() {
    int N; cin >> N;
    long long I; cin >> I;
    string a[30];
    for (int i = 0; i < N; i++) {
        cin >> a[i];
    }
    long long cnt1[31][31] = {};  // i <= j
    long long cnt2[31][31] = {};  // i >= j
    for (int i = N - 1; i >= 0; i--) {
        for (int j = N - 1; j >= 0; j--) {
            if (i == N - 1 && j == N - 1) {
                cnt1[i][j] = cnt2[i][j] = 1;
                continue;
            }
            if (i <= j) {
                cnt1[i][j] = cnt1[i][j + 1];
                if (i < j) cnt1[i][j] += cnt1[i + 1][j];
            }
            if (i >= j) {
                cnt2[i][j] = cnt2[i + 1][j];
                if (i > j) cnt2[i][j] += cnt2[i][j + 1];
            }
        }
    }
    I %= (cnt1[0][0] + cnt2[0][0]);
    string result = {a[0][0]};
    long long cnt3[30][30] = {};
    long long cnt4[30][30] = {};
    cnt3[0][0] = cnt4[0][0] = 1;
    for (int i = 1; i <= 2*N - 3; i++) {
        long long cnt[26] = {};
        for (int j = 0; j < N; j++) {
            const int k = i - j;
            if (k < 0 || k >= N) continue;
            const int m = a[j][k] - 'A';
            long long s1 = 0;
            if (j <= k) {
                if (j > 0) s1 += cnt3[j - 1][k];
                if (k > j) s1 += cnt3[j][k - 1];
            }
            cnt[m] += s1 * cnt1[j][k];
            long long s2 = 0;
            if (j >= k) {
                if (k > 0) s2 += cnt4[j][k - 1];
                if (j > k) s2 += cnt4[j - 1][k];
            }
            cnt[m] += s2 * cnt2[j][k];
        }
        for (int j = 0; j < 26; j++) {
            if (I >= cnt[j]) {
                I -= cnt[j];
            } else {
                result.push_back('A' + j);
                break;
            }
        }
        for (int j = 0; j < N; j++) {
            const int k = i - j;
            if (k < 0 || k >= N || a[j][k] != result.back()) continue;
            if (j <= k) {
                if (j > 0) cnt3[j][k] += cnt3[j - 1][k];
                if (k > j) cnt3[j][k] += cnt3[j][k - 1];
            }
            if (j >= k) {
                if (k > 0) cnt4[j][k] += cnt4[j][k - 1];
                if (j > k) cnt4[j][k] += cnt4[j - 1][k];
            }
        }
    }
    if (N > 1) result.push_back(a[N - 1][N - 1]);
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) do_testcase();
}
