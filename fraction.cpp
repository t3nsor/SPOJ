// 2024-03-09
// https://en.wikipedia.org/wiki/Farey_sequence#Next_term
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
void do_testcase() {
    int N, M; cin >> N >> M;
    vector<pair<int, int>> queries(M);
    vector<pair<int, int>> responses(M);
    for (int i = 0; i < M; i++) {
        cin >> queries[i].first;
        queries[i].second = i;
    }
    sort(queries.begin(), queries.end());
    int k = 1;
    int a = 0, b = 1;
    int c = 1, d = N;
    int j = 0;
    while (j < M) {
        while (j < M && k == queries[j].first) {
            responses[queries[j].second] = make_pair(a, b);
            j++;
        }
        const int x = (N + b) / d;
        const int p = x * c - a;
        const int q = x * d - b;
        a = c;
        b = d;
        c = p;
        d = q;
        ++k;
    }
    for (int i = 0; i < M; i++) {
        cout << responses[i].first << '/' << responses[i].second << '\n';
    }
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
