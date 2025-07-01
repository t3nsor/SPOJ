// 2025-07-01
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;
void do_testcase(int cs) {
    int n; cin >> n;
    vector<string> v;
    while (n--) { string s; cin >> s; v.push_back(s); }
    sort(v.begin(), v.end(),
         [](const string& s1, const string& s2) {
             return s1.size() > s2.size();
         });
    vector<string> v2;
    for (const auto& s : v) {
        bool contained = false;
        for (const auto& s2 : v2) {
            if (s2.find(s) != s2.npos) { contained = true; break; }
        }
        if (!contained) v2.push_back(s);
    }
    swap(v, v2);
    sort(v.begin(), v.end());
    int overlap[15][15];
    // overlap[i][j] is the length of the longest suffix of v[i] that is a
    // prefix of v[j]; we can compute this table naively because the strings are
    // not that long
    int sufrank[15][15];
    // sufrank[i][j] is the rank of v[j].substr(overlap[i][j]) among all such
    // strings for the given `i`
    const int N = v.size();
    for (int i = 0; i < N; i++) {
        vector<pair<string, int>> suffix;
        for (int j = 0; j < N; j++) {
            int& l = overlap[i][j];
            for (l = min(v[i].size(), v[j].size()); l >= 0; l--) {
                if (v[i].substr(v[i].size() - l, l) ==
                    v[j].substr(0, l)) break;
            }
            suffix.emplace_back(v[j].substr(l), j);
        }
        sort(suffix.begin(), suffix.end());
        for (int j = 0; j < N; j++) {
            sufrank[i][suffix[j].second] = j;
        }
    }
    // dp[i][j].first is the length of the shortest string that contains all
    // strings in the mask `i`, where `j` is the *first* string in it (we have
    // to build the string from back to front in order to get the
    // lexicographically shortest one). dp[i][j].second is the optimal first
    // string in the remainder of the string.
    vector<vector<pair<int, int>>> dp(1 << N,
                                      vector<pair<int, int>>(N, {2000, -1}));
    for (int i = 0; i < N; i++) {
        dp[1 << i][i].first = v[i].size();
    }
    for (int i = 0; i < (1 << N); i++) {
        for (int j = 0; j < N; j++) {
            if (!(i & (1 << j))) continue;
            for (int k = 0; k < N; k++) {
                if (j == k || !(i & (1 << k))) continue;
                const int l = (int)v[j].size() - overlap[j][k]
                               + dp[i - (1 << j)][k].first;
                // If there are multiple transitions that get the least length
                // then we pick the one with the lowest sufrank (meaning that
                // the part of the string after the v[j] prefix is
                // lexicographically least).
                if (l < dp[i][j].first ||
                    (l == dp[i][j].first &&
                     sufrank[j][k] < sufrank[j][dp[i][j].second]))
                    dp[i][j] = make_pair(l, k);
            }
        }
    }
    printf("Scenario #%d:\n", cs);
    int mask = (1 << N) - 1;
    // We sorted all the strings beforehand, so we can break ties at the
    // beginning by the index of the string.
    int first = 0;
    for (int i = 1; i < N; i++) {
        if (dp[mask][i].first < dp[mask][first].first) first = i;
    }
    string result = v[first];
    while ((mask - (1 << first)) > 0) {
        const int next = dp[mask][first].second;
        result.erase(result.size() - overlap[first][next]);
        result += v[next];
        mask -= (1 << first);
        first = next;
    }
    cout << result << "\n\n";
}
int main() {
    int T; cin >> T;
    for (int cs = 1; cs <= T; cs++) do_testcase(cs);
}
