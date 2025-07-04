// 2025-07-04
#include <algorithm>
#include <iomanip>
#include <iostream>
using namespace std;
void do_testcase(int cs, int M, int N) {
    int matrix[3][10];
    int iq[3];
    for (int i = 0; i < M; i++) cin >> iq[i];
    for (int i = 0; i < N; i++) {
        int k; cin >> k;
        int cutoff[10], time[10];
        for (int j = 0; j < k; j++) cin >> cutoff[j] >> time[j];
        for (int j = 0; j < M; j++) {
            matrix[j][i] = 3e7;
            for (int p = k - 1; p >= 0; p--) {
                if (iq[j] >= cutoff[p]) {
                    matrix[j][i] = time[p];
                    break;
                }
            }
        }
    }
    int pwr = 1;
    int best = 2e9, bestmask;
    for (int i = 0; i < N; i++) pwr *= M;
    for (int mask = 0; mask < pwr; mask++) {
        int assign[3][10]; int nassign[3] = {0};
        int mask2 = mask;
        for (int i = 0; i < N; i++) {
            int person = mask2 % M;
            mask2 /= M;
            assign[person][nassign[person]++] = matrix[person][i];
        }
        int time = 0;
        for (int i = 0; i < M; i++) {
            sort(assign[i], assign[i] + nassign[i]);
            int cur = 0;
            for (int j = 0; j < nassign[i]; j++) {
                cur += assign[i][j];
                time += cur;
            }
        }
        if (time < best) {
            best = time;
            bestmask = mask;
        }
    }
    cout << "Case " << cs << '\n';
    cout << "Average solution time = "
         << fixed << setprecision(2) << best / (double)N << '\n';
    int assign[3][10]; int nassign[3] = {0};
    for (int i = 0; i < N; i++) {
        int person = bestmask % M;
        bestmask /= M;
        assign[person][nassign[person]++] = i;
    }
    int start[10], finish[10], member[10];
    for (int i = 0; i < M; i++) {
        sort(assign[i], assign[i] + nassign[i],
             [&](int p1, int p2) { return matrix[i][p1] < matrix[i][p2]; });
        int cur = 0;
        for (int j = 0; j < nassign[i]; j++) {
            member[assign[i][j]] = i;
            start[assign[i][j]] = cur;
            cur += matrix[i][assign[i][j]];
            finish[assign[i][j]] = cur;
        }
    }
    for (int i = 0; i < N; i++) {
        cout << "Problem " << i + 1
             << " is solved by member " << member[i] + 1
             << " from " << start[i] << " to " << finish[i] << '\n';
    }
    cout << '\n';
}
int main() {
    for (int cs = 1;; cs++) {
        int M, N; cin >> M >> N; if (M == 0) return 0;
        do_testcase(cs, M, N);
    }
}
