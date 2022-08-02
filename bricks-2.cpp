// 2022-08-01
// Idea: for each c in 'a' ... 'z', we know that the ith occurrence of c in
// the first string needs to be moved to where the ith occurrence of c in the
// second string is. Replace each character in the first string with the
// position it needs to be moved to; then the number of swaps needed is the
// number of inversions in this permutation of 0 ... n-1. We can count this
// using mergesort. Recall that during the merge step, every time we use an
// element from the left half, we detect one inversion for every element from
// the right half that has been used so far. We keep a running total of which
// letters have been taken from the right, so we can check whether the current
// letter from the left half is not allowed to cross any of them.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
long long mergesort(vector<int>& v, int begin, int end, const string& b,
                    const vector<vector<int>>& allowed) {
    if (end - begin == 1) {
        return 0;
    }
    int mid = (begin + end) / 2;
    const long long r1 = mergesort(v, begin, mid, b, allowed);
    const long long r2 = mergesort(v, mid, end, b, allowed);
    if (r1 == -1 || r2 == -1) return -1;
    long long result = r1 + r2;
    const vector<int> l(v.begin() + begin, v.begin() + mid);
    const vector<int> r(v.begin() + mid, v.begin() + end);
    size_t i = 0, j = 0;
    vector<int> used(26, 0);
    while (i < l.size() || j < r.size()) {
        if (i < l.size() && (j == r.size() || l[i] < r[j])) {
            for (int k = 0; k < 26; k++) {
                // can be optimized using bitmask
                if (used[k] && !allowed[k][b[l[i]]]) {
                    return -1;
                }
            }
            v[begin + i + j] = l[i];
            result += j;
            i++;
        } else {
            used[b[r[j]]] = 1;
            v[begin + i + j] = r[j];
            j++;
        }
    }
    return result % (1LL << 32);
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) {
        string a, b; cin >> a >> b;
        int m; cin >> m;
        vector<vector<int>> allowed(26, vector<int>(26, 0));
        while (m--) {
            string p; cin >> p;
            p[0] -= 'a'; p[1] -= 'a';
            allowed[p[0]][p[1]] = allowed[p[1]][p[0]] = 1;
        }
        if (a.size() != b.size()) { cout << "-1\n"; continue; }
        const auto n = (int)a.size();
        vector<vector<int>> occ(26);
        for (int i = 0; i < n; i++) {
            a[i] -= 'a';
            b[i] -= 'a';
            occ[b[i]].push_back(i);
        }
        vector<int> next(26, 0);
        vector<int> perm;
        for (int i = 0; i < n; i++) {
            if (next[a[i]] == (int)occ[a[i]].size()) {
                cout << "-1\n"; goto end;  // continue 2
            }
            perm.push_back(occ[a[i]][next[a[i]]++]);
        }
        cout << mergesort(perm, 0, (int)perm.size(), b, allowed) << '\n';
end:; 
    }
}
