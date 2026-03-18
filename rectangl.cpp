// 2026-03-16
// This problem looks really hard, because N <= 250000 suggests that we have to
// find an O(N log N) solution, and it's not clear whether that's possible.
// However, it turns out that O(N^1.5) can pass, and once you know this, you
// can guess how the algorithm must work.
//
// Let X be the set of all distinct x-coordinates that are found in the input.
// Divide X into the subsets X_< and X_>, where X_< consists of those
// x-coordinates that occur at most sqrt(N) times in S, and X_> consists of all
// remaining elements of X.
//
// First, we find the number of rectangles where the x-coordinates of both
// vertical sides lie in X_<.  For each x-coordinate `x` in X_<, consider each
// pair (y_1, y_2) of y-coordinates for which (x, y_1) and (x, y_2) are in S.
// Add each such pair to a hash table and count the number of times it was
// found in the hash table immediately before this addition.  That is the count
// of rectangles with two vertices at (x, y_1) and (x, y_2) and the other two
// having previously been considered.  So when we've gone through all of X_<,
// we are done with the first subproblem.
//
// Now, we must find the number of rectangles where at least one vertical side
// has an x-coordinate that lies in X_>.  Observe that the cardinality of X_> is
// at most sqrt(N); therefore for each such `x` in X_>, we have a "time budget"
// of O(N), which makes it straighforward: for each x' != x we just count the
// number of y-coordinates in common between x' and x.  (To avoid
// double-counting, we should consider all x' in X_<, but only those x' in X_>
// with x' < x.)
//
// Because the second part of the algorithm doesn't require hash tables, it has
// a lower constant factor than the first part.  This suggests that the
// threshold should actually be set to sqrt(N)/d (where d is a constant greater
// than 1).  Setting d = 1 below gave a runtime of 4.50s, while d = 8 gave
// 0.86s.
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <unordered_map>
#include <vector>
using namespace std;
int main() {
    int N; scanf("%d", &N);
    const int rt = sqrt(N) / 8;
    vector<int> X(N), Y(N);
    vector<int> allx, ally;
    for (int i = 0; i < N; i++) {
        scanf("%d %d", &X[i], &Y[i]);
        allx.push_back(X[i]); ally.push_back(Y[i]);
    }
    // compress coordinates
    sort(allx.begin(), allx.end());
    allx.erase(unique(allx.begin(), allx.end()), allx.end());
    sort(ally.begin(), ally.end());
    ally.erase(unique(ally.begin(), ally.end()), ally.end());
    vector<vector<int>> yforx(allx.size());
    for (int i = 0; i < N; i++) {
        const int x = lower_bound(allx.begin(), allx.end(), X[i])
                      - allx.begin();
        yforx[x].push_back(lower_bound(ally.begin(), ally.end(), Y[i])
                           - ally.begin());
    }
    for (auto& ys : yforx) sort(ys.begin(), ys.end());
    long long result = 0;
    // step 1
    unordered_map<long long, int> M;
    for (int i = 0; i < allx.size(); i++) {
        if (yforx[i].size() > rt) continue;
        for (int j = 1; j < yforx[i].size(); j++) {
            for (int k = 0; k < j; k++) {
                const auto val = (((long long)yforx[i][j]) << 32) + yforx[i][k];
                result += M[val]++;
            }
        }
    }
    // step 2
    vector<char> active(ally.size(), 0);
    for (int i = 0; i < allx.size(); i++) {
        if (yforx[i].size() <= rt) continue;
        for (const int y : yforx[i]) {
            active[y] = 1;
        }
        for (int j = 0; j < allx.size(); j++) {
            if (yforx[j].size() > rt && j >= i) continue;
            int cnt = 0;
            for (const int y : yforx[j]) {
                if (active[y]) ++cnt;
            }
            result += ((long long)cnt) * (cnt - 1) / 2;
        }
        for (const int y : yforx[i]) {
            active[y] = 0;
        }
    }
    printf("%lld\n", result);
}
