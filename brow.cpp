// 2024-07-25
// This is an obvious two pointers problem: for a given potential brewery
// location, find the optimal splitting point such that deliveries to cities on
// one side of that point go around the island in one direction and deliveries
// to cities on the other side go around in the opposite direction, then move
// the brewery location by 1 city and adjust the splitting point, and so on.
// The only tricky detail is how to handle the circularity, which we do here by
// concatenating the array to a copy of itself.
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
void do_testcase() {
    int N; cin >> N;
    vector<long long> demand(2*N), dist(2*N);
    for (int i = 0; i < N; i++) {
        cin >> demand[i] >> dist[i];
        demand[i + N] = demand[i];
        dist[i + N] = dist[i];
    }
    vector<long long> pdist(2*N + 1);
    vector<long long> pdemand(2*N + 1);
    pdist[0] = 0;
    pdemand[0] = 0;
    for (int i = 0; i < 2*N; i++) {
        pdist[i + 1] = pdist[i] + dist[i];
        pdemand[i + 1] = pdemand[i] + demand[i];
    }
    long long cur = 0;
    for (int j = 1; j < N; j++) {
        // find cost for j to go right to i + N
        cur += demand[j] * (pdist[N] - pdist[j]);
    }
    long long best = 1LL << 40;
    int j = 1;
    for (int i = 0; i < N; i++) {
        if (i > 0) {
            // i, i + 1, ..., j - 1 have their distance decrease by dist[i - 1]
            // j, j + 1, ..., i + N - 1 have their distance increase likewise
            cur -= (pdemand[j] - pdemand[i]) * dist[i - 1];
            cur += (pdemand[i + N] - pdemand[j]) * dist[i - 1];
        }
        // advance j until the distance starts increasing
        while (j < i + N) {
            // switch j from going right to going left
            long long newcur = cur;
            newcur -= (pdist[i + N] - pdist[j]) * demand[j];
            newcur += (pdist[j] - pdist[i]) * demand[j];
            if (newcur <= cur) {
                ++j;
                cur = newcur;
            } else break;
        }
        best = min(best, cur);
    }
    cout << best << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) do_testcase();
}
