// 2023-04-07
// The basic idea is pretty simple. Consider X, the set of all possible spanning
// trees of the graph. Let x be an element of X. The cost of x will be of the
// form C_x(t) = m_x t + b_x, where m_x is the sum of the slopes of all the
// edges used by x, and b_x is the sum of the intercepts of all the edges used
// by x. So, C_x is an affine function of t. The actual cost at a given time t,
// C(t), is the minimum of C_x(t) across all x in X. This is the minimum of a
// set of affine functions on t, so the shape of C(t) is the lower envelope of
// all the C_x's, which means that it's a bitonic piecewise linear function. We
// can't compute an explicit description of C, because the number of segments
// can be up to |X|, which is exponentially large in the number of vertices. But
// the fact that C is bitonic is enough; we just have to do a ternary search.
//
// It's not clear what "accurate to three digits" means, so we just assume that
// the values of the optimal t and C(t) must be within 0.001 of the true values.
// C is a Lipschitz continuous function whose Lipschitz constant is at most the
// number of edges in a spanning tree (i.e. |V|-1) times the maximum absolute
// value of any edge's slope, so we know how precise t must be in order to get
// C(t) of the appropriate precision. The only tricky case is where there are
// multiple optimal values of t and we have to pick the leftmost one. The
// technique to handle this is explained below.
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;
double mst(const int V,
           const vector<int>& u, const vector<int>& v,
           const vector<int>& m, const vector<int>& b,
           double t) {
    const int E = u.size();
    vector<int> id(V);
    for (int i = 0; i < id.size(); i++) id[i] = i;
    vector<pair<double, int>> wt(E);
    for (int i = 0; i < E; i++) {
        wt[i] = {m[i] * t + b[i], i};
    }
    sort(wt.begin(), wt.end());
    vector<int> edges_used;
    int total_slope = 0;
    int total_intercept = 0;
    for (int i = 0; i < E && edges_used.size() < V - 1; i++) {
        int ui = u[wt[i].second];
        int vi = v[wt[i].second];
        while (id[ui] != ui) ui = id[ui] = id[id[ui]];
        while (id[vi] != vi) vi = id[vi] = id[id[vi]];
        if (ui == vi) continue;
        edges_used.push_back(wt[i].second);
        total_slope += m[edges_used.back()];
        total_intercept += b[edges_used.back()];
        id[ui] = vi;
    }
    // We do
    //   t * sum(m) + sum(b)
    // instead of
    //   sum(t*m + b)
    // to ensure that when sum(m) is zero, the result we get is exact. This is
    // important because if the optimal price occurs in an interval where
    // sum(m) = 0, then even a tiny imprecision in the MST weight can lead us
    // to pick an optimal time other than the leftmost point in the interval.
    return total_slope * t + total_intercept;
}
void do_testcase() {
    int V, E, t1, t2; cin >> V >> E >> t1 >> t2;
    vector<int> u, v, m, b;
    int maxm = 0;
    for (int i = 0; i < E; i++) {
        int ui, vi, mi, bi; cin >> ui >> vi >> mi >> bi;
        u.push_back(ui);
        v.push_back(vi);
        m.push_back(mi);
        b.push_back(bi);
        maxm = max(maxm, abs(mi));
    }
    double l = t1;
    double lval = mst(V, u, v, m, b, l);
    double r = t2;
    double rval = mst(V, u, v, m, b, r);
    while ((r - l) * (V - 1) * maxm > 0.001) {
        const double m1 = l + (r - l) / 3;
        const double m2 = r - (r - l) / 3;
        const double m1val = mst(V, u, v, m, b, m1);
        const double m2val = mst(V, u, v, m, b, m2);
        if (m1val < m2val) {
            l = m1;
            lval = m1val;
        } else {
            r = m2;
            rval = m2val;
        }
    }
    cout << fixed << setprecision(3) << l;
    cout << ' ';
    // I can never remember which manipulators are sticky
    cout << fixed << setprecision(3) << lval;
    cout << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
