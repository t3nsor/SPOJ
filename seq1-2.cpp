// 2024-10-15
// This solution is due to Jacob Plachta and is much simpler and faster than my
// original solution, seq1-1.cpp. The idea is again to work with the prefix sums
// of S. We maintain an upper bound for each prefix sum. Initially, the upper
// bound for P_i = S_0 + ... + S_{i-1} is just `i`. The value of an upper bound
// can be used to refine other upper bounds according to the constraints given
// in the input and the inherent constraint 0 <= P_{i+1} - P_i <= 1. When it is
// necessary to lower an upper bound, we add it to the queue so we can check
// whether the new value leads to further reductions in other upper bounds and
// so on. The algorithm terminates in at most O(N^2) steps.
#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
void do_testcase() {
    int N, A0, B0, L0, A1, B1, L1;
    cin >> N >> A0 >> B0 >> L0 >> A1 >> B1 >> L1;
    int tmp = A0;
    A0 = L0 - B0;
    B0 = L0 - tmp;
    vector<int> bound(N + 1);
    queue<int> Q;
    for (int i = 0; i <= N; i++) {
        bound[i] = i;
        Q.push(i);
    }
    while (!Q.empty()) {
        int i = Q.front();
        Q.pop();
        if (i > 0 && bound[i] < bound[i - 1]) {
            bound[i - 1] = bound[i];
            Q.push(i - 1);
        }
        else if (i < N && bound[i + 1] - bound[i] > 1) {
            bound[i + 1] = bound[i] + 1;
            Q.push(i + 1);
        }
        if (i >= L0 && bound[i] - bound[i - L0] < A0 && bound[i - L0] > 0) {
            bound[i - L0] = max(bound[i] - A0, 0);
            Q.push(i - L0);
        }
        if (i + L0 <= N && bound[i + L0] - bound[i] > B0) {
            bound[i + L0] = bound[i] + B0;
            Q.push(i + L0);
        }
        if (i >= L1 && bound[i] - bound[i - L1] < A1 && bound[i - L1] > 0) {
            bound[i - L1] = max(bound[i] - A1, 0);
            Q.push(i - L1);
        }
        if (i + L1 <= N && bound[i + L1] - bound[i] > B1) {
            bound[i + L1] = bound[i] + B1;
            Q.push(i + L1);
        }
    }
    bool fail = false;
    for (int i = 0; i < N; i++) {
        if (bound[i + 1] - bound[i] != 0 &&
            bound[i + 1] - bound[i] != 1) {
            fail = true;
            break;
        }
        if (i + L0 <= N &&
            (bound[i + L0] - bound[i] < A0 ||
             bound[i + L0] - bound[i] > B0)) {
            fail = true;
            break;
        }
        if (i + L1 <= N &&
            (bound[i + L1] - bound[i] < A1 ||
             bound[i + L1] - bound[i] > B1)) {
            fail = true;
            break;
        }
    }
    if (fail) {
        cout << "-1\n";
        return;
    }
    for (int i = 0; i < N; i++) {
        cout << bound[i + 1] - bound[i];
    }
    cout << '\n';
}
int main() {
    for (int i = 0; i < 5; i++) do_testcase();
}
