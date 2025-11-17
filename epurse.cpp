// 2025-11-16
// The only tricky thing about this problem is to watch out for large values of
// W and B.  All values of W that are greater than or equal to N are equivalent
// (each throw can knock out only one ball) and all values of B that are greater
// than or equal to 2N are equivalent since any sequence of more than two throws
// affecting a particular residue class is equivalent to a sequence of two
// throws.
#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;
int main() {
    for (int cs = 1;; cs++) {
        int W, B, N; cin >> W >> B >> N;
        if (W == 0) break;
        if (B > 2*N) B = 2*N;
        if (W > N) W = N;
        vector<int> money(N);
        for (int i = 0; i < N; i++) cin >> money[i];
        vector<int> size1;
        vector<int> size2;
        for (int i = 0; i < W; i++) {
            vector<int> v;
            for (int j = i; j < N; j += W) {
                v.push_back(money[j]);
            }
            vector<int> psum(v.size() + 1);
            for (int j = 0; j < v.size(); j++) {
                psum[j + 1] = psum[j] + v[j];
            }
            int best1 = -1e9, best2 = -1e9;
            for (int j = 0; j <= v.size(); j++) {
                best1 = max(best1, psum[j]);
                best1 = max(best1, psum.back() - psum[j]);
            }
            for (int j = 0; j <= v.size(); j++) {
                for (int k = j; k <= v.size(); k++) {
                    best2 = max(best2, psum[j] + psum.back() - psum[k]);
                }
            }
            size1.push_back(best1);
            size2.push_back(best2);
        }
        vector<int> dp(B + 1, -1e9);
        dp[0] = 0;
        for (int i = 0; i < W; i++) {
            for (int j = B; j >= 1; j--) {
                dp[j] = max(dp[j], dp[j - 1] + size1[i]);
                if (j >= 2) dp[j] = max(dp[j], dp[j - 2] + size2[i]);
            }
        }
        int bestmoney = 0, bestmoves = 0;
        for (int i = 0; i <= B; i++) {
            if (dp[i] > bestmoney) {
                bestmoney = dp[i];
                bestmoves = i;
            }
        }
        printf("Case#%d: Jack wins $%d out of %d throws.\n",
               cs, bestmoney, bestmoves);
    }
}
