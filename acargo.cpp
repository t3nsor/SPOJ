// 2025-11-09
// The problem statement is a bit confusing.  Each piece of cargo can be thought
// of as being 1m wide, so when pieces of cargo occupy consecutive positions on
// the conveyor belt, there is no gap between them and they form a single
// "luggage" (weird choice of term tbh).  It's obvious that the total cost to
// form a single luggage is just the sum of distances from each piece of cargo
// to its final position.  There must exist an optimal final position in which
// one piece of cargo does not move and is the middle piece (or, in the case of
// even N, one of the two middle pieces) of the final luggage.  So, all we need
// to do is consider each piece of cargo as the final middle piece and calculate
// the cost needed to move all the other pieces of cargo into their
// corresponding positions, then take the min across all such possibilities.  To
// handle circularity, we treat the conveyor belt as linear but duplicate it
// once to the left and once to the right.
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
void do_testcase(int N, long long L) {
    vector<long long> loc(3*N);
    for (int i = 0; i < N; i++) {
        cin >> loc[i];
    }
    sort(loc.begin(), loc.begin() + N);
    for (int i = 0; i < N; i++) {
        loc[i + N] = loc[i] + L;
        loc[i + 2*N] = loc[i] + 2*L;
    }
    vector<long long> ploc(3*N + 1);
    for (int i = 0; i < 3*N; i++) {
        ploc[i + 1] = ploc[i] + loc[i];
    }
    long long best = 1LL << 50;
    const int nleft = (N - 1)/2;
    const int nright = N / 2;
    for (int i = N; i < 2*N; i++) {
        // use cargo i as the midpoint:
        // cargos i - nleft, ..., i - 1 move right to
        // positions loc[i] - nleft, ..., loc[i] - 1
        long long dist = nleft*(2*loc[i] - nleft - 1)/2
                         - (ploc[i] - ploc[i - nleft])
        // cargos i + 1, ..., i + nright move left to
        // positions loc[i] + 1, ..., loc[i] + nright
                         + (ploc[i + nright + 1] - ploc[i + 1])
                         - nright*(2*loc[i] + nright + 1)/2;
        best = min(best, dist);
    }
    cout << best << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    for (;;) {
        int N, L; cin >> N >> L;
        if (N == 0 && L == 0) break;
        do_testcase(N, L);
    }
}
