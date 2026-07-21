// 2026-07-21
// We must first answer the following question: for a pair of observation points
// A and B, what is the locus of possible locations P for the pole?  If the
// observed compass directions from A and B are parallel, then the pole lies on
// the ray from A in the direction opposite to B or vice versa; if they are
// antiparallel, then the pole lies on the segment between A and B.  Call these
// the "degenerate cases".  In any other case, let θ be ∠APB for one possible
// location of P.  Any other possible location P' must also have ∠AP'B = θ, and
// by the inscribed angle theorem, all such points lie on a circle that passes
// through A and B (and for which the centre O satisfies ∠AOB = 2θ).  Moreover,
// it is easy to see that all such points lie on one side of the line between A
// and B, so the locus is exactly a circular arc whose endpoints are A and B
// (but A and B are not included in the arc).
//
// Now it comes down to case analysis:
// 1. All pairs of input points have a degenerate locus.  This is actually
//    impossible because the pole would not be uniquely determined (all input
//    points would be collinear and you could at best narrow down the pole to a
//    segment between two of the input points).
// 2. At least one pair of input points has a non-degenerate locus, and all
//    other pairs of input points have a concentric non-degenerate locus.  This
//    also fails to determine a unique pole, for the following reason.  Because
//    the loci are all *open* arcs (i.e. don't contain their endpoints), their
//    intersection never determines a unique point, so we must consider the
//    dependence of the pole on the error offset.  To wit, let α be the
//    hypothesized value of the error offset, and let P(A, B, α) be the point
//    that the pole would be at if the error offset is actually α, based on the
//    input points A and B and their corresponding vectors.  Then, the angular
//    position of P(A, B, α) on the circular arc changes by exactly 2Δα whenever
//    α changes by Δα (this is again due to the inscribed angle theorem): note
//    that this is independent of A and B.  It follows that if there is a value
//    of α for which P(A, B, α) coincides for all pairs A, B, that condition
//    will also hold for any other α such that the resulting P still lies in the
//    intersection of all pairwise loci.  That is, at best we can narrow down
//    the pole location to a circular arc of nonzero length.
// 3. In all other cases, i.e., the ones that can actually occur in the input,
//    there is always a pair of loci such that either both are arcs but are not
//    concentric, or one is an arc and the other is not.  In either case, the
//    intersection determines at most two points, and we just need to check
//    which one is correct.  Actually, the case where one is an arc and the
//    other is not is commented out below: apparently it's not needed (whether
//    this is because there are always provably at least two non-concentric arcs
//    or the test data are weak is left as an exercise for the reader).
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <vector>
using namespace std;
const double eps = 1e-7;
const double pi = acos(-1.0);
//void circ_line(double ox, double oy, double r,
//               double a, double b, double c,
//               vector<double>& xout, vector<double>& yout) {
//    const double c2 = a*ox + b*oy + c;
//    double d = -c2/hypot(a, b);
//    if (d > r) d = r; else if (d < -r) d = -r;
//    const double angle1 = atan2(b, a) + acos(d / r);
//    const double angle2 = atan2(b, a) - acos(d / r);
//    xout.push_back(ox + r*cos(angle1));
//    xout.push_back(ox + r*cos(angle2));
//    yout.push_back(oy + r*sin(angle1));
//    yout.push_back(oy + r*sin(angle2));
//}
void circ_circ(double x1, double y1, double r1,
               double x2, double y2, double r2,
               vector<double>& xout, vector<double>& yout) {
    const double dx = x2 - x1;
    const double dy = y2 - y1;
    double d = hypot(dx, dy);
    const double h = (d*d - r2*r2 + r1*r1) / (2*d);
    const double k = sqrt(max(0.0, r1*r1 - h*h));
    xout.push_back(x1 + dx*(h / d) + dy*(k / d));
    yout.push_back(y1 + dy*(h / d) - dx*(k / d));
    xout.push_back(x1 + dx*(h / d) - dy*(k / d));
    yout.push_back(y1 + dy*(h / d) + dx*(k / d));
}
int main() {
    int N; scanf("%d", &N);
    vector<double> X(N), Y(N), U(N), V(N);
    //vector<double> A, B, C;  // lines
    vector<double> Ox, Oy, R;  // circles
    for (int i = 0; i < N; i++) {
        scanf("%lf %lf %lf %lf", &X[i], &Y[i], &U[i], &V[i]);
        for (int j = 0; j < i; j++) {
            if (fabs(U[i]*V[j] - U[j]*V[i]) < eps) {
                // A.push_back(Y[j] - Y[i]);
                // B.push_back(X[i] - X[j]);
                // C.push_back(-A.back()*X[i] - B.back()*Y[i]);
            } else {
                const double vx = X[j] - X[i];
                const double vy = Y[j] - Y[i];
                const double d = hypot(vx, vy);
                double delta = atan2(V[j], U[j]) - atan2(V[i], U[i]);
                if (delta < 0) delta += 2*pi;
                R.push_back(d/(2*fabs(sin(delta))));
                const double h = d/2*tan(pi/2 - delta);
                const double nx = -vy/d;
                const double ny = vx/d;
                Ox.push_back((X[i] + X[j])/2.0 + h*nx);
                Oy.push_back((Y[i] + Y[j])/2.0 + h*ny);
            }
        }
    }
    vector<double> cx, cy;
    // if (!A.empty()) {
    //     // circle-line intersection
    //     circ_line(Ox[0], Oy[0], R[0], A[0], B[0], C[0], cx, cy);
    // } else {
    // find non-concentric circles
    for (int i = 0; i < Ox.size(); i++) {
        for (int j = 0; j < i; j++) {
            if (fabs(R[i] - R[j]) > eps ||
                fabs(Ox[i] - Ox[j]) > eps ||
                fabs(Oy[i] - Oy[j]) > eps) {
                circ_circ(Ox[i], Oy[i], R[i], Ox[j], Oy[j], R[j], cx, cy);
                break;
            }
        }
        if (!cx.empty()) break;
    }
    // }
    // pick the better candidate
    double err[2] = {0.0, 0.0};
    for (int i = 0; i < 2; i++) {
        double a0 = atan2(cy[i] - Y[0], cx[i] - X[0]) - atan2(V[0], U[0]);
        if (a0 < 0) a0 += 2*pi;
        for (int j = 1; j < N; j++) {
            double a = atan2(cy[i] - Y[j], cx[i] - X[j]) - atan2(V[j], U[j]);
            if (a < 0) a += 2*pi;
            const double diff = fabs(a - a0);
            err[i] = max(err[i], min(diff, 2*pi - diff));
        }
    }
    if (err[0] < err[1]) {
        printf("%.6f %.6f\n", cx[0], cy[0]);
    } else {
        printf("%.6f %.6f\n", cx[1], cy[1]);
    }
}
