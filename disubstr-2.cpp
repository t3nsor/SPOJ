// 2022-12-28
// This O(N^2) time, O(N) memory (independent of alphabet size) algorithm was
// communicated to me by the late Michael B. Cohen (1992-2017).
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int main() {
    string s;
    int T; cin >> T;
    while (T--) {
        cin >> s;
        const int N = s.size();
        vector<int> L(N, 0);
        // We will compute the array L, where for each i, L[i] is the length of
        // the longest *non-unique* substring of s whose last character is at
        // index i. A substring is non-unique if it also occurs earlier in the
        // string. It will then follow that all longer such substrings are
        // unique, meaning that there are (i + 1) - L[i] unique substrings with
        // last character at index i. Summing over all i gives the final
        // result.
        //
        // To compute L, we loop over all possible offsets k. At a given k, we
        // will compute, for each i, the length of the longest common suffix
        // of the prefixes ending at positions i and i-k. This can be used to
        // improve the current value of L[i].
        for (int k = 1; k < N; k++) {
            int run = 0;
            for (int i = k; i < N; i++) {
                if (s[i] == s[i - k]) ++run; else run = 0;
                L[i] = max(L[i], run);
            }
        }
        int result = 0;
        for (int i = 0; i < N; i++) {
            result += (i + 1) - L[i];
        }
        cout << result << '\n';
    }
}
