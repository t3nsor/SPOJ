// 2022-08-30
// Uses the Kirchhoff matrix-tree theorem

#include <cmath>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

// `GaussJordan` taken from
// https://github.com/t3nsor/codebook/blob/master/gaussian.cpp
const double EPS = 1e-10;

typedef vector<int> VI;
typedef long double T;
typedef vector<T> VT;
typedef vector<VT> VVT;

T GaussJordan(VVT &a, VVT &b) {
  const int n = a.size();
  const int m = b[0].size();
  VI irow(n), icol(n), ipiv(n);
  T det = 1;

  for (int i = 0; i < n; i++) {
    int pj = -1, pk = -1;
    for (int j = 0; j < n; j++) if (!ipiv[j])
      for (int k = 0; k < n; k++) if (!ipiv[k])
        if (pj == -1 || fabs(a[j][k]) > fabs(a[pj][pk])) { pj = j; pk = k; }
    if (fabs(a[pj][pk]) < EPS) { return 0; }
    ipiv[pk]++;
    swap(a[pj], a[pk]);
    swap(b[pj], b[pk]);
    if (pj != pk) det *= -1;
    irow[i] = pj;
    icol[i] = pk;

    T c = 1.0 / a[pk][pk];
    det *= a[pk][pk];
    a[pk][pk] = 1.0;
    for (int p = 0; p < n; p++) a[pk][p] *= c;
    for (int p = 0; p < m; p++) b[pk][p] *= c;
    for (int p = 0; p < n; p++) if (p != pk) {
      c = a[p][pk];
      a[p][pk] = 0;
      for (int q = 0; q < n; q++) a[p][q] -= a[pk][q] * c;
      for (int q = 0; q < m; q++) b[p][q] -= b[pk][q] * c;      
    }
  }

  for (int p = n-1; p >= 0; p--) if (irow[p] != icol[p]) {
    for (int k = 0; k < n; k++) swap(a[k][irow[p]], a[k][icol[p]]);
  }

  return det;
}

int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        int n, m; cin >> n >> m;
        vector<vector<long double>> laplacian(n, vector<long double>(n, 0));
        while (m--) {
            int u, v; cin >> u >> v;
            u--; v--;
            laplacian[u][u] += 1;
            laplacian[v][v] += 1;
            laplacian[u][v] = laplacian[v][u] = -1;
        }
        laplacian.pop_back();
        for (auto& row : laplacian) {
            row.pop_back();
        }
        if (laplacian.empty()) {
            // the determinant of an empty matrix is 1, so the formula is
            // correct in this case, but our `GaussJordan` doesn't handle it
            cout << "1\n";
            continue;
        }
        vector<vector<long double>> b(laplacian.size());
        cout << abs(llround(GaussJordan(laplacian, b))) << '\n';
    }
}
