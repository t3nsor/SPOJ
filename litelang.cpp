// 2023-10-04
// According to Hanson Wang, the algorithm works like this: maintain the current
// list of words, and repeatedly:
// 1. remove the lightest word, W, from the list
// 2. append each of the k letters to W and add the resulting k words to the
//    list
// 3. if there are more than n words in the list, repeatedy remove the heaviest
//    word until there are only n words left
// 4. if we already had n words prior to step 1, and the resulting list after
//    step 3 is equally or more heavy than the list before step 1, halt and use
//    the list before step 1. Otherwise, go back to step 1 and repeat.
// The naive greedy approach (repeatedly attempt to expand the size of the list
// by 1 by either adding a leaf or splitting a leaf into two leaves, whichever
// is cheaper) seems not to work as it gets WA on the judge, although I'm not
// immediately aware of a counterexample.
#include <iostream>
#include <numeric>
#include <set>
#include <vector>
using namespace std;
void do_testcase() {
    int n, S; cin >> n >> S;
    vector<int> wt(S);
    for (int i = 0; i < S; i++) {
        cin >> wt[i];
    }
    multiset<int> words = {0};
    while (words.size() < n) {
        auto it = words.begin();
        for (int i = 0; i < S; i++) {
            words.insert(*it + wt[i]);
        }
        words.erase(it);
        while (words.size() > n) {
            it = words.end();
            --it;
            words.erase(it);
        }
    }
    int current_sum = accumulate(words.begin(), words.end(), 0);
    for (;;) {
        auto it = words.begin();
        int removed = *it;
        int added = 0;
        for (int i = 0; i < S; i++) {
            added += removed + wt[i];
            words.insert(removed + wt[i]);
        }
        words.erase(it);
        while (words.size() > n) {
            it = words.end();
            --it;
            removed += *it;
            words.erase(it);
        }
        if (added >= removed) {
            cout << current_sum << '\n';
            return;
        }
        current_sum += added - removed;
    }
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
