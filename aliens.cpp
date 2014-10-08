// 2013-11-11
// modified 2014-10-07
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;
struct point {
    double x;
    double y;
    bool operator<(const point& p) const {
        return x < p.x || x == p.x && y < p.y;
    }
    bool operator==(const point& p) const {
        return x == p.x && y == p.y;
    }
};
struct circle {
    double x;
    double y;
    double r;
    circle() {}
    circle(double x, double y, double r): x(x), y(y), r(r) {}
};
vector<point> P;
vector<point> R;
int i;
circle b_md() {
    if (R.size() == 0) {
        return circle(0, 0, -1);
    } else if (R.size() == 1) {
        return circle(R[0].x, R[0].y, 0);
    } else if (R.size() == 2) {
        return circle((R[0].x+R[1].x)/2.0,
                      (R[0].y+R[1].y)/2.0,
                      hypot(R[0].x-R[1].x, R[0].y-R[1].y)/2.0);
    } else {
        double D = (R[0].x - R[2].x)*(R[1].y - R[2].y) - (R[1].x - R[2].x)*(R[0].y - R[2].y);
        double p0 = (((R[0].x - R[2].x)*(R[0].x + R[2].x) + (R[0].y - R[2].y)*(R[0].y + R[2].y)) / 2 * (R[1].y - R[2].y) - ((R[1].x - R[2].x)*(R[1].x + R[2].x) + (R[1].y - R[2].y)*(R[1].y + R[2].y)) / 2 * (R[0].y - R[2].y))/D;
        double p1 = (((R[1].x - R[2].x)*(R[1].x + R[2].x) + (R[1].y - R[2].y)*(R[1].y + R[2].y)) / 2 * (R[0].x - R[2].x) - ((R[0].x - R[2].x)*(R[0].x + R[2].x) + (R[0].y - R[2].y)*(R[0].y + R[2].y)) / 2 * (R[1].x - R[2].x))/D;
        return circle(p0, p1, hypot(R[0].x - p0, R[0].y - p1));
    }
}

circle D;

void b_minidisk() {
    if (i == P.size() || R.size() == 3) {
        D = b_md();
    } else {
        i++;
        b_minidisk();
        i--;
        if (hypot(P[i].x-D.x, P[i].y-D.y) > D.r) {
            R.push_back(P[i]);
            i++;
            b_minidisk();
            i--;
            R.pop_back();
        }
    }
}

int main() {
    int c;
    scanf("%d", &c);
    while (c--) {
        int N;
        scanf("%d", &N);
        P.clear();
        for (int i = 0; i < N; i++) {
            double x, y;
            scanf("%lf %lf", &x, &y);
            point p; p.x = x; p.y = y;
            P.push_back(p);
        }
        // Remove identical points
        sort(P.begin(), P.end());
        vector<point>::iterator it = unique(P.begin(), P.end());
        P.resize(distance(P.begin(), it));
        random_shuffle(P.begin(), P.end());
        i = 0;
        R.clear();
        b_minidisk();
        printf("%.2f\n%.2f %.2f\n", D.r, D.x, D.y);
    }
    return 0;
}

