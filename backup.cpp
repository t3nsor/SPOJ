// 2025-05-30
// This problem is solved using a greedy approach: we start with no connections
// and repeatedly increase the number of connections by 1. In each step we
// either add a connection by connecting two adjacent offices that are not yet
// connected, *or* take a maximal sequence of consecutive pairs of connected
// offices (e.g. 3-4 5-6 7-8 where offices 2 and 9 are not connected to anyone)
// and expand it by connecting the leftmost office to its left neighbour, the
// rightmost office to its right neighbour, and "flipping" all internal links
// (e.g. resulting in 2-3 4-5 6-7 8-9). At each stage we take whatever option
// will increase the total cost of links so far by the smallest amount. This can
// be done with a heap, which gives an O(N + K log N) algorithm since the
// initial construction can be done in O(N) time. I was lazy and used a set,
// which gives O(N log N) with a shitty constant factor but it still passes.
//
// This took me several tries to get right because I didn't take into account
// the fact that when we expand a sequence of connected pairs, the resulting
// expanded sequence can be contiguous with an arbitrary number of connected
// pairs on either side (e.g. 1-2 3-4, 6-7 8-9, 11-12 13-14 can become 1-2 3-4
// 5-6 7-8 9-10 11-12 13-14). We need to be careful not to make wrong
// assumptions about the endpoints of the larger resulting maximal sequence, or
// about when expansion is possible.
#include <set>
#include <stdio.h>
#include <vector>
using namespace std;
struct Entry {
    int val;
    int left;
    int right;
    friend bool operator<(const Entry& e1, const Entry& e2) {
        return e1.val < e2.val;
    }
};
void do_testcase() {
    int n, k; scanf("%d %d", &n, &k);
    vector<int> loc(n);
    for (int i = 0; i < n; i++) scanf("%d", &loc[i]);
    multiset<Entry> S;
    vector<pair<bool, multiset<Entry>::iterator>> is_left(n);
    vector<pair<bool, multiset<Entry>::iterator>> is_right(n);
    for (int i = 1; i < n; i++) {
        const auto it = S.insert(Entry{loc[i] - loc[i - 1], i - 1, i});
        is_left[i - 1] = make_pair(true, it);
        is_right[i] = make_pair(true, it);
    }
    int result = 0;
    for (int i = 0; i < k; i++) {
        const auto entry = *S.begin();
        S.erase(S.begin());
        result += entry.val;
        is_left[entry.left].first = is_right[entry.right].first = false;
        int expand_right = -1;
        int expand_cost = -entry.val;
        if (is_left[entry.right].first) {
            const int l = entry.right;
            const auto it = is_left[entry.right].second;
            const int r = it->right;
            is_left[l].first = false;
            is_right[r].first = false;
            expand_right = r;
            expand_cost += it->val;
            S.erase(it);
        }
        int expand_left = -1;
        if (is_right[entry.left].first) {
            const int r = entry.left;
            const auto it = is_right[entry.left].second;
            const int l = it->left;
            is_left[l].first = false;
            is_right[r].first = false;
            expand_left = l;
            expand_cost += it->val;
            S.erase(it);
        }
        if (expand_left >= 0 && expand_right >= 0) {
            const auto it = S.insert(Entry{expand_cost,
                                           expand_left,
                                           expand_right});
            is_left[expand_left] = make_pair(true, it);
            is_right[expand_right] = make_pair(true, it);
        }
    }
    printf("%d\n", result);
}
int main() {
    int T; scanf("%d", &T); while (T--) do_testcase();
}
