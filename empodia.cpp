// 2024-02-08
// This solution is similar to
//   https://usaco.guide/problems/ioi-2004empodia/solution
// The time complexity is unclear.
#include <stack>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
void do_testcase() {
    int M; scanf("%d", &M);
    vector<int> a(M);
    for (int i = 0; i < M; i++) scanf("%d", &a[i]);

    vector<int> nsv_right(M, M);  // nearest smaller value on the right
    {
        stack<int> S;
        for (int i = 0; i < M; i++) {
            while (!S.empty() && a[i] < a[S.top()]) {
                nsv_right[S.top()] = i;
                S.pop();
            }
            S.push(i);
        }
    }

    vector<int> nlv_left(M, -1);  // nearest larger value on the left
    {
        stack<int> S;
        for (int i = M - 1; i >= 0; i--) {
            while (!S.empty() && a[i] > a[S.top()]) {
                nlv_left[S.top()] = i;
                S.pop();
            }
            S.push(i);
        }
    }

    // b[j] contains i such that a[i] - i + M = j
    vector<vector<int>> b(2*M);
    int num_empodia = 0;
    vector<int> empodio_right(M, -1);
    int latest_start = -1;
    // latest_start is the greatest index at which some empodio is currently
    // known to start
    for (int i = 0; i < M; i++) {
        const int j = a[i] - i + M;
        for (int k = b[j].size() - 1; k >= 0; k--) {
            const int prev = b[j][k];
            if (prev <= latest_start) {
                // We can stop looking because any other framed interval we find
                // will contain the empodio that starts at `prev`.
                break;
            }
            if (nlv_left[i] >= prev) {
                // there can't be a framed interval ending at i that begins on
                // or before nlv_left[i], so we don't need to look at any
                // earlier values in b[j]
                break;
            }
            if (nsv_right[prev] > i) {
                empodio_right[prev] = i;
                if (prev > latest_start) latest_start = prev;
                ++num_empodia;
            }
        }
        b[j].push_back(i);
    }

    printf("%d\n", num_empodia);
    for (int i = 0; i < M; i++) {
        if (empodio_right[i] >= 0) {
            printf("%d %d\n", i + 1, empodio_right[i] + 1);
        }
    }
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
