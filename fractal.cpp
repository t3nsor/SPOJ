// 2026-05-03
// n = 12 is on the upper end of brute-forceable.  I precompute the fractal
// itself for up to n = 12, and for a given value of alpha, the trigonometric
// functions of it (so that we're not recomputing them 8 million times, which
// would be expensive).  Then, we just need to "flood fill" for each test case.
// There are additional constant optimizations possible, which are left as an
// exercise for the reader.  I would imagine that there's a linear time
// solution that's based on using the fractal structure, but it seems like it
// would have to be extremely complicated.  None of the official solutions
// attempt such an approach (https://neerc.ifmo.ru/archive/2008/northern/jury-solutions.rar).
#include <math.h>
#include <queue>
#include <stdio.h>
#include <vector>
using namespace std;

constexpr int dr[4] = {1, 0, -1, 0};
constexpr int dc[4] = {0, 1, 0, -1};

const double pi = acos(-1.0);

struct Curve {
    int N;
    vector<char> grid;
    Curve(int N) : N(N), grid(1<<(2*N)) {}
    char* operator[](int row) { return grid.data() + (row<<N); }
    const char* operator[](int row) const { return grid.data() + (row<<N); }
};

vector<Curve> curves;

struct Node {
    int pd;
    int r;
    int c;
    double h;
};

double do_testcase(int n, double a) {
    const Curve& H = curves[n];
    const double sn = sin(a);
    const double cs = cos(a);
    const double tn = tan(a);
    const double ct = 1/tn;
    double result = 0;
    queue<Node> Q;
    for (int i = 0; i < (1 << n); i++) {
        if (H[0][i]) Q.push(Node{2, 0, i, cos(a)});
    }
    while (!Q.empty()) {
        Node node = Q.front();
        Q.pop();
        if (node.h <= min(sn, cs)) {
            result += node.h*node.h/(2*sn*cs);
        } else if (node.h <= max(sn, cs)) {
            double h2 = node.h - min(sn, cs);
            result += 0.5*min(tn, ct) + h2/max(sn, cs);
        } else {
            double h2 = sn + cs - node.h;
            result += 1 - h2*h2/(2*sn*cs);
        }
        for (int i = 0; i < 4; i++) {
            if (i == node.pd) continue;
            const int r2 = node.r + dr[i];
            const int c2 = node.c + dc[i];
            if (r2 < 0 || r2 >= (1 << n) || c2 < 0 || c2 >= (1 << n) ||
                !H[r2][c2]) {
                continue;
            }
            int newpd = (i + 2) % 4;
            if (i == 0) {
                double h2 = node.h + cs;
                Q.push(Node{newpd, r2, c2, min(h2, sn + cs)});
            } else if (i == 2) {
                if (node.h <= cs) continue;
                Q.push(Node{newpd, r2, c2, node.h - cs});
            } else if (i == 1) {
                if (node.h <= sn) continue;
                Q.push(Node{newpd, r2, c2, node.h - sn});
            } else {
                double h2 = node.h + sn;
                Q.push(Node{newpd, r2, c2, min(h2, sn + cs)});
            }
        }
    }
    return result;
}

int main() {
    curves.emplace_back(0);  // element 0 not used
    Curve one(1);
    one[0][0] = 1;
    curves.push_back(one);
    for (int i = 2; i <= 12; i++) {
        Curve c(i);
        const Curve& last = curves.back();
        int w = (1 << i) - 1;
        int hw = (1 << (i - 1)) - 1;
        for (int j = 0; j < w; j++) c[hw][j] = 1;
        for (int j = 0; j < hw; j++) c[j][hw] = 1;
        for (int j = 0; j < hw; j++) {
            for (int k = 0; k < hw; k++) {
                c[j][k] = !last[k][hw - 1 - j];
                c[j][k + hw + 1] = !last[hw - 1 - k][j];
                c[j + hw + 1][k] = last[j][k];
                c[j + hw + 1][k + hw + 1] = last[j][k];
            }
        }
        curves.push_back(move(c));
    }
    int n, a;
    while (scanf("%d %d", &n, &a) == 2) {
        printf("%.9f\n", do_testcase(n, a*pi/180.0));
    }
}
