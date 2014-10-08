// 2014-05-02
#include <cstdio>
#include <queue>
#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
using namespace std;
bool do_it(vector<int>& X, vector<int>& D, int t, int dir) {
    int N = X.size();
    int cur = (dir == 0 ? 0 : N-1);
    vector<bool> taken(N, false);
    priority_queue<pair<int, int> > Q[2];
    int next[2]; next[0] = N - 1; next[1] = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 2; j++) {
            Q[j].push(make_pair(D[i] + (1-2*j)*X[i], i));
        }
    }
    while (next[dir] >= 0 && next[dir] < N) {
        t = min(t, D[next[dir]] + abs(X[cur] - X[next[dir]]) - 1);
        while (!Q[dir].empty()) {
            pair<int, int> p = Q[dir].top();
            if (taken[p.second]) {
                Q[dir].pop(); continue;
            }
            if (p.first > t + (1 - 2*dir)*X[cur]) {
                taken[p.second] = true;
                Q[dir].pop();
            } else {
                break;
            }
        }
        if (!taken[next[dir]]) return false;
        t -= abs(X[cur] - X[next[dir]]);
        if (t < 0) return false;
        cur = next[dir];
        dir = 1 - dir;
        while (next[0] >= 0 && taken[next[0]]) next[0]--;
        while (next[1] < N && taken[next[1]]) next[1]++;
    }
    return true;
}
int main() {
    int T; scanf("%d", &T);
    while (T--) {
        int N; scanf("%d", &N);
        vector<int> X, D;
        for (int i = 0; i < N; i++) {
            int x, d; scanf("%d %d", &x, &d);
            X.push_back(x); D.push_back(d);
        }
        // binary search
        int l, r;
        l = 0; r = D[0];
        while (l < r) {
            int m = (l+r)/2;
            if (do_it(X, D, m, 0)) {
                r = m;
            } else {
                l = m+1;
            }
        }
        int l1 = l; bool ok = l < D[0];
        l = 0; r = D[N-1];
        while (l < r) {
            int m = (l+r)/2;
            if (do_it(X, D, m, 1)) {
                r = m;
            } else {
                l = m+1;
            }
        }
        if (ok || l < D[N-1]) {
            l1 = (l1 < D[0] ? l1 : 1e9);
            l = (l < D[N-1] ? l : 1e9);
            printf("%d\n", min(l1, l));
        } else {
            puts("No solution");
        }
    }
}
