// 2024-02-12
// I think this problem is fairly difficult, so it's surprising that there are
// so many accepted users. I gave up and looked at the official solution, which
// starts here:
//   https://oi.edu.pl/static/attachment/20130309/1995_1.pdf#page=129
// If (like me) you don't know Polish, you can use Google Translate :)
//
// We imagine moving the balls from the initial set of boxes into a final set
// of initially empty boxes (numbered 1 to n like the initial boxes). First, for
// each box that is initially nonempty, we take 1 ball from that box and place
// it in the corresponding final box. Then, repeatedly
// * arbitrarily pick a ball from one of the initial boxes
// * either place it to the left of the leftmost final box currently occupied by
//   balls that came from the same initial box, or place it to the right of the
//   rightmost such final box.
// To decide whether to place it at the left or right, we calculate the cost of
// both (including the marginal cost of shifting over other balls) and then pick
// the cheaper one. In that sense the algorithm is greedy.
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <utility>
#include <vector>
using namespace std;
int dist(int n, int x, int y) {
    const int d = abs(x - y);
    return min(d, n - d);
}
int cost(const vector<int>& a) {
    int result = 0;
    const int n = a.size();
    for (int i = 0; i < n; i++) {
        if (a[i] >= 0) {
            result += dist(n, i, a[i]);
        }
    }
    return result;
}
void do_testcase() {
    int n; cin >> n;
    const auto dec = [n] (int x) {
        return (x + n - 1) % n;
    };
    const auto inc = [n] (int x) {
        return (x + 1) % n;
    };
    vector<int> initial(n);
    vector<int> final(n, -1);
    for (int i = 0; i < n; i++) {
        cin >> initial[i];
        if (initial[i]) {
            --initial[i];
            final[i] = i;
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < initial[i]; j++) {
            // find the leftmost box occupied by a ball with label `i`
            int left = i;
            while (final[left] != i) {
                left = inc(left);
            }
            while (final[dec(left)] == i) {
                left = dec(left);
            }
            // find the rightmost
            int right = i;
            while (final[right] != i) {
                right = dec(right);
            }
            while (final[inc(right)] == i) {
                right = inc(right);
            }
            // push the ball to the left
            auto pl = final;
            int displaced = i;
            do {
                left = dec(left);
                swap(pl[left], displaced);
            } while (displaced >= 0);
            auto pr = final;
            displaced = i;
            do {
                right = inc(right);
                swap(pr[right], displaced);
            } while (displaced >= 0);
            const auto cl = cost(pl);
            const auto cr = cost(pr);
            if (cl <= cr) {
                final = move(pl);
            } else {
                final = move(pr);
            }
        }
    }
    cout << cost(final) << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
