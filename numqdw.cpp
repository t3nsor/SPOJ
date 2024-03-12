// 2024-03-12
// Define f(i, j) to be the number of valid suffixes of length i, where j
// represents the prefix:
// * if no characters in w occur in the prefix, j = c
// * otherwise, out of all characters that occur in the prefix, the one whose
//   first occurrence in w is the earliest is the value of j.
// Then
// * f(0, x) = 1 for all x
// * f(i, x) = \sum_y f(i - 1, min(x, y))
//             subject to: y is a valid character, and either x = c or there is
//             no y that occurs after any x
// where min(x, y) is defined as follows:
// * if x is c, then min(x, y) is y
// * otherwise, min(x, y) is either x or y depending on which one's first
//   occurrence in w is earlier
//
// Thus the vector f(i, ?) is obtained as a linear transformation of
// f(i - 1, ?) and we can obtain the final vector f(0, ?) through matrix
// exponentiation. Finally, the result we seek is f(0, c).
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
using namespace std;
int crt[2][3][7][101];
vector<vector<int>> modmul(const vector<vector<int>>& A,
                           const vector<vector<int>>& B) {
    const int N = A.size();
    vector<vector<int>> result(N, vector<int>(N, 0));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                result[i][j] = (result[i][j] + A[i][k] * B[k][j]) % 4242;
            }
        }
    }
    return result;
}
vector<vector<int>> modexp(const vector<vector<int>>& A, int exponent) {
    const int N = A.size();
    if (exponent == 0) {
        vector<vector<int>> identity(N, vector<int>(N, 0));
        for (int i = 0; i < N; i++) identity[i][i] = 1;
        return identity;
    }
    auto result = modexp(A, exponent / 2);
    result = modmul(result, result);
    if (exponent % 2) {
        result = modmul(result, A);
    }
    return result;
}
void do_testcase() {
    int n, c; cin >> n >> c;
    string s; cin >> s;
    vector<int> first(c, s.size());
    vector<vector<int>> occursafter(c, vector<int>(c, 0));
    for (int i = 0; i < s.size(); i++) {
        for (int j = 0; j < c; j++) {
            if (first[j] < i) {
                occursafter[j][s[i] - 'A'] = 1;
            }
        }
        first[s[i] - 'A'] = min(first[s[i] - 'A'], i);
    }
    vector<vector<int>> matrix(c + 1, vector<int>(c + 1, 0));
    for (int x = 0; x <= c; x++) {
        for (int y = 0; y < c; y++) {
            if (x < c && occursafter[x][y]) continue;
            if (x == c) {
                matrix[x][y]++;
            } else {
                if (first[x] < first[y]) matrix[x][x]++;
                else matrix[x][y]++;
            }
        }
    }
    matrix = modexp(matrix, n);
    int result = 0;
    for (int i = 0; i <= c; i++) {
        result = (result + matrix[c][i]) % 4242;
    }
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
