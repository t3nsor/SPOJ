// 2023-09-24
// This is an implementation of the official solution, given here:
// https://oi.edu.pl/static/attachment/20110731/oi4.pdf#page=36
// (No, there's no English version. I used Google Translate.)
// The idea is to build up palindromes starting from the ends and working toward
// the middle, adjoining a pair of equal letters at each step. Conceptually,
// dp[i][j] can be thought of as the number of different paths by which this can
// be done while ending at `i` on the left and `j` on the right. Note that `i`
// and `j` uniquely identify particular letters in the entire set of words,
// i.e., they can be up to 24,000. Surprisingly, this solution does run in time,
// although it's not clear to me how to avoid the use of hash tables, since
// 24000 * 24000 * 4 seems to exceed the memory limit.
#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
void do_testcase() {
    int n; cin >> n;
    vector<string> W(2*n);
    for (int i = 0; i < n; i++) {
        cin >> W[2*i];
    }
    for (int i = 0; i < n; i++) {
        cin >> W[2*i + 1];
    }
    if (n == 1) {
        // special case
        string s0 = W[0];
        string s1 = W[1];
        reverse(s0.begin(), s0.end());
        reverse(s1.begin(), s1.end());
        int result = 0;
        if (W[0] == s0) ++result;
        if (W[1] == s1) ++result;
        cout << result << '\n';
        return;
    }
    string T;
    vector<int> begin(2*n);
    vector<int> end(2*n);
    for (int i = 0; i < 2*n; i++) {
        begin[i] = T.size();
        T += W[i];
        end[i] = T.size() - 1;
    }
    vector<int> word(T.size());
    vector<array<int, 2>> left(T.size());
    vector<array<int, 2>> right(T.size());
    for (int i = 0; i < 2*n; i++) {
        for (int j = 0; j < W[i].size(); j++) {
            word[begin[i] + j] = i;
            if (j < W[i].size() - 1) {
                right[begin[i] + j][0] = begin[i] + j + 1;
                right[begin[i] + j][1] = -1;
            } else if (i < 2*n-2) {
                right[begin[i] + j][0] = begin[(i&~1)+2];
                right[begin[i] + j][1] = begin[(i&~1)+3];
            } else {
                right[begin[i] + j][0] = -1;
                right[begin[i] + j][1] = -1;
            }
            if (j > 0) {
                left[begin[i] + j][0] = begin[i] + j - 1;
                left[begin[i] + j][1] = -1;
            } else if (i >= 2) {
                left[begin[i] + j][0] = end[(i&~1)-2];
                left[begin[i] + j][1] = end[(i&~1)-1];
            } else {
                left[begin[i] + j][0] = -1;
                left[begin[i] + j][1] = -1;
            }
        }
    }
    unordered_map<int, int> layer;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (W[i].front() == W[2*n-2+j].back()) {
                const int index = (begin[i] << 16) + end[2*n-2+j];
                layer[index] = 1;
            }
        }
    }
    int result = 0;
    while (!layer.empty()) {
        unordered_map<int, int> layer2;
        for (const auto& entry : layer) {
            const int node = entry.first;
            const int x = node >> 16;
            const int y = node & 0xffff;
            if (x == y ||
                (x + 1 == y && word[x] == word[y]) ||
                (x == end[word[x]] && y == begin[word[y]] &&
                 (word[x]&~1) + 2 == (word[y]&~1))) {
                result += entry.second;
                continue;
            }
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    const int x2 = right[x][i];
                    const int y2 = left[y][j];
                    if (x2 >= 0 && y2 >= 0 && x2 <= y2 && T[x2] == T[y2]) {
                        layer2[(x2 << 16) + y2] += entry.second;
                    }
                }
            }
        }
        swap(layer, layer2);
    }
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
