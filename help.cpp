// 2024-02-15
// The strategy of having the frequency halve for every level you go down is,
// for some reason, NOT correct (it gets WA) although I haven't found a
// counterexample. In this solution, each node's frequency is greater than the
// sum of all its descendants' frequencies. I'm not sure how to prove that this
// works, but it gets accepted.
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
long long rec(int node, const vector<int>& l, const vector<int>& r,
              vector<long long>& freq) {
    long long lsum = 0;
    long long rsum = 0;
    if (l[node] >= 0) {
        lsum = rec(l[node], l, r, freq);
    }
    if (r[node] >= 0) {
        rsum = rec(r[node], l, r, freq);
    }
    freq[node] = lsum + rsum + 1;
    return freq[node] + lsum + rsum;
}

void do_testcase(int n) {
    vector<int> l(n), r(n);
    vector<long long> freq(n);
    vector<int> nonroot(n, 0);
    for (int i = 0; i < n; i++) {
        cin >> l[i] >> r[i];
        if (l[i] > 0) {
            --l[i];
            nonroot[l[i]] = 1;
        }
        if (r[i] > 0) {
            --r[i];
            nonroot[r[i]] = 1;
        }
    }
    int root = 0;
    while (nonroot[root]) ++root;
    rec(root, l, r, freq);
    cout << freq[0];
    for (int i = 1; i < n; i++) {
        cout << ' ' << freq[i];
    }
    cout << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    for (;;) {
        int n; cin >> n;
        if (n == 0) return 0;
        do_testcase(n);
    }
}
