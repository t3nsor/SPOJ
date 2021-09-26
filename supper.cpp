// 2021-09-26
// Idea: the standard LIS algorithm given an array a[0], ..., a[n-1] actually
// produces an output array l[0], ..., l[n-1] where l[i] is the length of the
// longest increasing subsequence that ends at l[i]. Similarly we can compute
// the length of the longest increasing subsequence that starts at l[i] for
// all i. Iff the sum of these two lengths, for given i, equals 1 plus the
// global LIS length, then l[i] is part of at least one LIS.
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
// result[i] equals the length of the longest increasing subsequence of v
// that ends at v[i]
vector<int> calc(const vector<int>& v) {
    vector<int> result(v.size());
    vector<int> least_last(v.size() + 1, 1000000);
    least_last[0] = -1000000;
    for (int i = 0; i < v.size(); i++) {
        const auto it =
          upper_bound(least_last.begin(), least_last.end(), v[i]);
        *it = v[i];
        result[i] = it - least_last.begin();
    }
    return result;
}
void do_testcase() {
    int n; cin >> n;
    vector<int> v(n);
    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }
    vector<int> lis1 = calc(v);
    // To get the length of the longest LIS *starting* at each element, we do
    // a lazy trick: reverse the array and change the sign of each element.
    // For example {2, 1, 4, 5, 3} -> {-3, -5, -4, -1, -2}. For example, the
    // increasing subsequence {1, 4, 5} starting at 1 in the original array
    // becomes {-5, -4, -1} ending at -1 in the transformed array. The
    // resulting array of LIS lengths then needs to itself be reversed in
    // order to have the lengths in the right positions corresponding to the
    // original array.
    reverse(v.begin(), v.end());
    for_each(v.begin(), v.end(), [](auto& x) { x *= -1; });
    vector<int> lis2 = calc(v);
    reverse(lis2.begin(), lis2.end());
    // Put the array back the way it was before, for output purposes
    reverse(v.begin(), v.end());
    for_each(v.begin(), v.end(), [](auto& x) { x *= -1; });
    vector<int> result;
    const int lis = *max_element(lis1.begin(), lis1.end());
    for (int i = 0; i < n; i++) {
        if (lis1[i] + lis2[i] == lis + 1) {
            result.push_back(v[i]);
        }
    }
    cout << result.size() << '\n';
    sort(result.begin(), result.end());
    for (int i = 0; i < result.size(); i++) {
        if (i) cout << ' ';
        cout << result[i];
    }
    cout << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    for (int i = 0; i < 10; i++) {
        do_testcase();
    }
}
