// 2024-02-27
// The most straightforward DP passes in time: for every slab size, try all
// possible vertical cuts and all possible horizontal cuts. Determining the time
// complexity is left as an exercise for the reader.
#include <stdio.h>
#include <vector>
using namespace std;
void do_testcase() {
    int W, H; scanf("%d %d", &W, &H);
    vector<vector<int>> waste(W + 1, vector<int>(H + 1, 1e6));
    waste[0][0] = 0;
    int N; scanf("%d", &N);
    while (N--) {
        int w, h; scanf("%d %d", &w, &h);
        waste[w][h] = 0;
    }
    for (int i = 0; i <= W; i++) {
        for (int j = 0; j <= H; j++) {
            waste[i][j] = min(waste[i][j], i * j);
            for (int k = 1; k < i; k++) {
                waste[i][j] = min(waste[i][j], waste[k][j] + waste[i - k][j]);
            }
            for (int k = 1; k < j; k++) {
                waste[i][j] = min(waste[i][j], waste[i][k] + waste[i][j - k]);
            }
        }
    }
    printf("%d\n", waste[W][H]);
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
