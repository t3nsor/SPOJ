// 2026-07-20
// I didn't know this technique, so I had to give up and look up the solution.
// https://codeforces.com/blog/entry/58621#comment-422384
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
using LL = long long;
vector<LL> a, b, values;
bool can(int R) {
    vector<int> from;
    vector<int> to;
    vector<int> wt;
    const int V = values.size() + 1;
    for (int i = 0; i < V - 1; i++) {
        from.push_back(V - 1);
        to.push_back(i);
        wt.push_back(0);
    }
    for (int i = 0; i < V - 2; i++) {
        from.push_back(i + 1);
        to.push_back(i);
        wt.push_back(0);
    }
    for (int i = 0; i < a.size(); i++) {
        from.push_back(a[i]);
        to.push_back(b[i]);
        wt.push_back(R);
        from.push_back(b[i]);
        to.push_back(a[i]);
        wt.push_back(-1);
    }
    // run Bellman-Ford algorithm
    vector<int> dist(V, 1e9);
    dist[V - 1] = 0;
    for (int i = 0; i < V - 1; i++) {
        for (int j = 0; j < from.size(); j++) {
            dist[to[j]] = min(dist[to[j]], dist[from[j]] + wt[j]);
        }
    }
    // check for negative cycles
    for (int j = 0; j < from.size(); j++) {
        if (dist[to[j]] > dist[from[j]] + wt[j]) return false;
    }
    return true;
}
int main() {
    int N; cin >> N;
    a.resize(N); b.resize(N);
    for (int i = 0; i < N; i++) {
        LL h;
        cin >> a[i] >> b[i] >> h;
        a[i] = 2*a[i];
        b[i] = 2*b[i] - 1;
        values.push_back(a[i]);
        values.push_back(b[i]);
    }
    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());
    for (int i = 0; i < N; i++) {
        a[i] = lower_bound(values.begin(), values.end(), a[i]) - values.begin();
        b[i] = lower_bound(values.begin(), values.end(), b[i]) - values.begin();
    }
    int l = 1, r = N;
    while (r > l) {
        const int m = (l + r) / 2;
        if (can(m)) {
            r = m;
        } else {
            l = m + 1;
        }
    }
    cout << l << '\n';
}
