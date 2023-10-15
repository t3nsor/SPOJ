// 2023-10-14
// This problem can be solved in linear time using suffix arrays, but I don't
// feel like coding up the K-S algorithm so I just copied the one from the
// codebook (which is not linear time).
//
// The explanation of how to solve the problem for one string is given here:
// https://stackoverflow.com/a/77295027/481267
// Extending this to multiple strings is straightforward.
#include <algorithm>
#include <iostream>
#include <limits.h>
#include <stack>
#include <string>
#include <vector>
using namespace std;
struct SuffixArray {
  const int L;
  string s;
  vector<vector<int> > P;
  vector<pair<pair<int,int>,int> > M;

  SuffixArray(const string &s) : L(s.length()), s(s), P(1, vector<int>(L, 0)), M(L) {
    for (int i = 0; i < L; i++) P[0][i] = L==1 ? i : int(s[i]);
    for (int skip = 1, level = 1; skip < L; skip *= 2, level++) {
      P.push_back(vector<int>(L, 0));
      for (int i = 0; i < L; i++) 
             M[i] = make_pair(make_pair(P[level-1][i], i + skip < L ? P[level-1][i + skip] : -1000), i);
      sort(M.begin(), M.end());
      for (int i = 0; i < L; i++) 
             P[level][M[i].second] = (i > 0 && M[i].first == M[i-1].first) ? P[level][M[i-1].second] : i;
    }    
  }

  vector<int> GetSuffixArray() { return P.back(); }

  // returns the length of the longest common prefix of s[i...L-1] and s[j...L-1]
  int LongestCommonPrefix(int i, int j) {
    int len = 0;
    if (i == j) return L - i;
    for (int k = P.size() - 1; k >= 0 && i < L && j < L; k--) {
      if (P[k][i] == P[k][j]) {
        i += 1 << k;
        j += 1 << k;
        len += 1 << k;
      }
    }
    return len;
  }
};

struct IndexData {
    vector<int> lo;
    vector<int> hi;
};

void do_testcase() {
    int n; cin >> n;
    string s;
    vector<int> which_word;
    vector<int> index_within_word;
    for (int i = 0; i < n; i++) {
        string message; cin >> message; message.push_back(i);
        s += message;
        for (int j = 0; j < message.size(); j++) {
            which_word.push_back(i);
            index_within_word.push_back(j);
        }
    }
    SuffixArray SA(s);
    auto rsa = SA.GetSuffixArray();
    // compute actual suffix array
    vector<int> sa(s.size());
    for (int i = 0; i < rsa.size(); i++) {
        sa[rsa[i]] = i;
    }
    // compute LCP array
    vector<int> lcp(s.size() - 1);
    for (int i = 0; i + 1 < s.size(); i++) {
        lcp[i] = SA.LongestCommonPrefix(sa[i], sa[i + 1]);
    }
    // compute all nearest smaller values
    vector<IndexData> left_indices(lcp.size());
    stack<pair<int, int>> S;
    S.emplace(-1, -1);
    for (int i = 0; i < lcp.size(); i++) {
        IndexData& data = left_indices[i];
        data.lo.assign(n, INT_MAX);
        data.hi.assign(n, -1);
        data.lo[which_word[sa[i]]] = index_within_word[sa[i]];
        data.hi[which_word[sa[i]]] = index_within_word[sa[i]];
        data.lo[which_word[sa[i+1]]] = min(data.lo[which_word[sa[i+1]]],
                                           index_within_word[sa[i+1]]);
        data.hi[which_word[sa[i+1]]] = max(data.hi[which_word[sa[i+1]]],
                                           index_within_word[sa[i+1]]);
        while (S.top().second >= lcp[i]) {
            const auto p = S.top();
            S.pop();
            for (int j = 0; j < n; j++) {
                data.lo[j] = min(data.lo[j], left_indices[p.first].lo[j]);
                data.hi[j] = max(data.hi[j], left_indices[p.first].hi[j]);
            }
        }
        S.emplace(i, lcp[i]);
    }
    vector<IndexData> right_indices(lcp.size());
    while (!S.empty()) S.pop();
    S.emplace(lcp.size(), -1);
    for (int i = lcp.size() - 1; i >= 0; i--) {
        IndexData& data = right_indices[i];
        data.lo.assign(n, INT_MAX);
        data.hi.assign(n, -1);
        data.lo[which_word[sa[i]]] = index_within_word[sa[i]];
        data.hi[which_word[sa[i]]] = index_within_word[sa[i]];
        data.lo[which_word[sa[i+1]]] = min(data.lo[which_word[sa[i+1]]],
                                           index_within_word[sa[i+1]]);
        data.hi[which_word[sa[i+1]]] = max(data.hi[which_word[sa[i+1]]],
                                           index_within_word[sa[i+1]]);
        while (S.top().second >= lcp[i]) {
            const auto p = S.top();
            S.pop();
            for (int j = 0; j < n; j++) {
                data.lo[j] = min(data.lo[j], right_indices[p.first].lo[j]);
                data.hi[j] = max(data.hi[j], right_indices[p.first].hi[j]);
            }
        }
        S.emplace(i, lcp[i]);
    }
    int best = 0;
    for (int i = 0; i < lcp.size(); i++) {
        int min_diff = INT_MAX;
        for (int j = 0; j < n; j++) {
            const int hi = max(left_indices[i].hi[j], right_indices[i].hi[j]);
            const int lo = min(left_indices[i].lo[j], right_indices[i].lo[j]);
            min_diff = min(min_diff, hi - lo);
        }
        int len = min(min_diff, lcp[i]);
        best = max(best, len);
    }
    cout << best << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
