// 2023-11-20
// This solution is based on explicitly representing and updating the full dp
// function, which, just like in the slow O(n^3) approach, is as follows:
// * dp_0(h[0]) = 0;
// * dp_i(x) = min_{|x' - x| <= d} dp_{i - 1}(x') + |x - h[i]|
// This is a piecewise linear function with O(n) pieces, where the slope of each
// piece is an integer, and (crucially) the slope increases monotonically with
// increasing x. The idea is to find a way to perform the transition (i.e.,
// transform the function from dp_{i - 1} to dp_i) in O(log n) time. In order to
// do so, we need to find the right representation for the function. One piece
// of information we will need to maintain is the interval [l, r], possibly
// empty, where the slope is negative on the left of `l`, and positive on the
// right of `r`. By the monotonicity property, the function also achieves its
// global minimum on this interval. During the first part of the transition,
// namely computing min_{|x' - x| <= d} dp_{i-1}(x') for all `x`, this interval
// is expanded by `d` in each direction, since for l - d <= x <= l, the optimal
// `x'` will be `l`, and for r <= x <= r + d, the optimal `x'` will be `r`. For
// the second part of the transition, where we add |x - h[i]|, we consider that
// this adds -1 to the slope of each piece to the left of h[i], and +1 to the
// slope of each piece to the right of h[i]. This means that our representation
// of the function shouldn't be a map from x-coordinates to slopes directly,
// but rather, at each x-coordinate, we should store the *difference* in slopes
// between the next piece and the previous piece. That means we only need to
// update O(1) of these differences in order to add |x - h[i]| to the existing
// function. Note as well that during the first part of the transition, all the
// x coordinates get shifted by `-d` if to the left of `l` and `+d` if to the
// right of `r`. This implies that we also must use a relative representation
// for the x-coordinates. Since we need to separately consider the part to the
// left of [l, r] and the part to the right, we have two maps, `L` and `R`,
// which store pairs (k, D) where
// * for `L`, the key `k` is the x-coordinate relative to the extreme left
//   point for which the function is defined, namely h[0] - i*d
// * similarly, for `R`, `k` is the x-coordinate relative to h[0] + i*d
// and `D` is the change in slope that occurs at the x-coordinate.
// The implementation is a bit complicated...
// During the main loop, since the maps only keep track of the differences in
// slopes, we also maintain the variables `ls` and `rs`, which are the slopes
// immediately to the left of `l` and to the right of `r`, respectively. If the
// slope doesn't change until the extreme left/right point, respectively, the
// corresponding map is empty. That is, the size of the map is actually 1 less
// than the number of segments on the corresponding side. When we eventually
// finish the main loop, we have to reconstruct the actual values using the
// difference data.
// For another approach using range trees, see ccrossx-1.cpp.
#include <iostream>
#include <map>
#include <vector>
using namespace std;
using ll = long long;
void do_testcase() {
    int n; ll d; cin >> n >> d;
    vector<int> h(n);
    for (int i = 0; i < n; i++) cin >> h[i];
    if (abs(h[n-1] - h[0]) > (n-1)*d) {
        cout << "impossible\n";
        return;
    }
    ll l = h[0];
    ll r = h[0];
    map<ll, ll> L, R;
    ll ls, rs;
    for (int i = 1; i < n; i++) {
        l -= d;
        r += d;
        // insert new point for h[i], if necessary
        if (h[i] > h[0] - i*d && h[i] < l) {
            ll x = h[i] - (h[0] - i*d);
            auto it = L.lower_bound(x);
            if (it == L.end() || it->first != x) {
                L[x] = 0;
            }
        } else if (h[i] < h[0] + i*d && h[i] > r) {
            ll x = h[i] - (h[0] + i*d);
            auto it = R.lower_bound(x);
            if (it == R.end() || it->first != x) {
                R[x] = 0;
            }
        } else if (h[i] > l && h[i] < r) {
            if (l == h[0] - i*d) {
                ls = 0;
            } else {
                L[l - (h[0] - i*d)] = -ls;
                ls = 0;
            }
            if (r == h[0] + i*d) {
                rs = 0;
            } else {
                R[r - (h[0] + i*d)] = rs;
                rs = 0;
            }
            l = r = h[i];
        }
        // add -1 to the slope of everything to the left
        if (h[i] > h[0] - i*d) {
            if (h[i] < l) {
                ll x = h[i] - (h[0] - i*d);
                L[x]++;
            } else if (h[i] == l) {
                ls--;
            } else {
                if (l == h[0] - i*d) {
                    ls = -1;
                } else {
                    L[l - (h[0] - i*d)] = -ls;
                    ls = -1;
                }
                l = r;
                if (h[i] > r) {
                    rs--;
                    if (h[i] < h[0] + i*d) {
                        R[h[i] - (h[0] + i*d)]++;
                    }
                }
            }
        }
        // add +1 to the slope of everything to the right
        if (h[i] < h[0] + i*d) {
            if (h[i] > r) {
                ll x = h[i] - (h[0] + i*d);
                R[x]++;
            } else if (h[i] == r) {
                rs++;
            } else {
                if (r == h[0] + i*d) {
                    rs = 1;
                } else {
                    R[r - (h[0] + i*d)] = rs;
                    rs = 1;
                }
                r = l;
                if (h[i] < l) {
                    ls++;
                    if (h[i] > h[0] - i*d) {
                        L[h[i] - (h[0] - i*d)]++;
                    }
                }
            }
        }
        // fix up the equal range
        while (!L.empty() && ls == 0) {
            ls -= L.rbegin()->second;
            l = h[0] - i*d + L.rbegin()->first;
            L.erase(prev(L.end()));
        }
        if (L.empty() && ls == 0) {
            l = h[0] - i*d;
        }
        while (!R.empty() && rs == 0) {
            rs += R.begin()->second;
            r = h[0] + i*d + R.begin()->first;
            R.erase(R.begin());
        }
        if (R.empty() && rs == 0) {
            r = h[0] + i*d;
        }
    }
    // reconstruct the actual function
    ll y = 0;
    for (int i = 0; i < n; i++) {
        y += abs(h[0] - i*d - h[i]);
    }
    vector<pair<ll, ll>> costfn;
    costfn.emplace_back(h[0] - (n-1)*d, y);
    ll slope = 0;
    if (h[0] - (n-1)*d < l) {
        slope = ls;
        for (const auto p : L) {
            slope -= p.second;
        }
    }
    ll x = h[0] - (n-1)*d;
    for (const auto p : L) {
        auto newx = h[0] - (n-1)*d + p.first;
        costfn.emplace_back(newx, y += (newx - x) * slope);
        slope += p.second;
        x = newx;
    }
    if (x < l) {
        costfn.emplace_back(l, y += (l - x) * slope);
        x = l;
    }
    if (x < r) {
        costfn.emplace_back(r, y);
        x = r;
    }
    if (x < h[0] + (n-1)*d) {
        slope = rs;
        for (const auto p : R) {
            auto newx = h[0] + (n-1)*d + p.first;
            costfn.emplace_back(newx, y += (newx - x) * slope);
            slope += p.second;
            x = newx;
        }
        costfn.emplace_back(h[0] + (n-1)*d,
                            y + (h[0] + (n-1)*d - x) * slope);
    }
    // calculate the answer
    for (int i = 0; i < costfn.size(); i++) {
        if (h[n-1] == costfn[i].first) {
            cout << costfn[i].second << '\n';
            return;
        }
        if (h[n-1] <= costfn[i + 1].first) {
            slope = (costfn[i + 1].second - costfn[i].second) /
                    (costfn[i + 1].first - costfn[i].first);
            cout << costfn[i].second + slope * (h[n-1] - costfn[i].first)
                 << '\n';
            return;
        }
    }
    throw;
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
