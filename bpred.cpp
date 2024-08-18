// 2024-08-18
// Essentially, this is a matrix problem: let `v` be a column vector of size `N`
// where v[i] is the probability that instruction `i` is currently being
// executed, and let M be the matrix such that Mv gives the probabilities of
// each instruction being executed in the following time step. Then (I + M + M^2
// + ...)v0 gives the expected number of times each instruction is executed
// where v0 is initially 100% weighted on the start instruction. If this
// converges, then it's equal to (I - M)^{-1} v0, which we compute using
// Gaussian elimination. Note that there can be some extreme (ill-conditioned)
// test cases for which this code will not give the right answer, but they don't
// seem to occur in the judge data.
#include <iomanip>
#include <iostream>
#include <math.h>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

void do_testcase() {
    int N; cin >> N;
    long double P; cin >> P;
    string L; cin >> L;
    vector<string> label(N);
    vector<string> target(N);
    unordered_map<string, int> id;
    for (int i = 0; i < N; i++) {
        cin >> label[i] >> target[i];
        if (!id.emplace(label[i], i).second) throw;
    }
    vector<vector<long double>> A(N, vector<long double>(N + 1, 0));
    for (int i = 0; i < N; i++) {
        A[i][i] = 1;
    }
    for (int i = 0; i < N - 1; i++) {
        if (label[i] == "start") {
            A[i + 1][i] -= 1;
        } else {
            const int t = id[target[i]];
            A[i + 1][i] -= 1 - P;
            A[t][i] -= P;
        }
    }
    A[id["start"]][N] = 1;
    for (int i = 0; i < N; i++) {
        int j = i;
        while (j < N && A[i][j] == 0) {
            j++;
        }
        if (j == N) throw;  // not invertible
        swap(A[i], A[j]);
        long double x = A[i][i];
        for (int j = 0; j <= N; j++) {
            A[i][j] /= x;
        }
        for (int j = 0; j < N; j++) {
            if (j == i) continue;
            x = A[j][i];
            for (int k = 0; k <= N; k++) {
                A[j][k] -= x * A[i][k];
            }
        }
    }
    cout << "Expected number of times label " << L << " is executed is "
         << fixed << setprecision(5)
         << A[id[L]][N] << '\n';
}
int main() {
    int T; cin >> T;
    while (T--) do_testcase();
}
