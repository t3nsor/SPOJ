// 2026-04-13
// Needs mild constant optimization to pass.  There are accepted solutions with
// 0.00 runtime, so they probably found some way that doesn't require explicitly
// computing the PDF.  We compute the PDF using "brute force", i.e. by explicit
// convolution, treating each unit interval separately (because the PDF will be
// a polynomial on each unit interval).  Convolutions are tedious in general,
// but it's not too bad if one operand is a monomial and the other is a
// constant, which is all we need.
#include <array>
#include <math.h>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
struct Poly : array<double, 10> {
    Poly() { for (int i = 0; i < 10; i++) (*this)[i] = 0; }

    static Poly unity() {
        Poly result;
        result[0] = 1;
        return result;
    }
};

Poly operator+(const Poly& p1, const Poly& p2) {
    Poly result;
    for (int i = 0; i < 10; i++) result[i] = p1[i] + p2[i];
    return result;
}

Poly& operator+=(Poly& lhs, const Poly& rhs) {
    for (int i = 0; i < 10; i++) lhs[i] += rhs[i];
    return lhs;
}

Poly operator-(const Poly& p1, const Poly& p2) {
    Poly result;
    for (int i = 0; i < 10; i++) result[i] = p1[i] - p2[i];
    return result;
}

Poly operator*(double k, const Poly& p) {
    Poly result;
    for (int i = 0; i < 10; i++) result[i] = k*p[i];
    return result;
}

// p := (x + a)p
void mul(Poly& p, double a) {
    for (int i = 9; i > 0; i--) {
        p[i] = a * p[i] + p[i - 1];
    }
    p[0] *= a;
}

Poly operator/(const Poly& p, double k) {
    Poly result;
    for (int i = 0; i < 10; i++) result[i] = p[i]/k;
    return result;
}

// Computes the convolution of f and g, where:
// f(x) = | p(x) if a <= x <= a + 1
//        | 0    otherwise
//
// g(x) = | 1    if b <= x <= b + 1
//        | 0 otherwise
// The result is returned as a pair of polynomials P, Q representing the actual
// convolution h:
// h(x) = | P(x) if a + b <= x <= a + b + 1,
//        | Q(x) if a + b + 1 <= x <= a + b + 2
//        | 0    otherwise
pair<Poly, Poly> conv(const Poly& p, int a, int b) {
    pair<Poly, Poly> result;
    Poly pow1 = Poly::unity();
    Poly pow2 = Poly::unity();
    for (int i = 0; i < 9; i++) {
        mul(pow1, -b);
        mul(pow2, -b-1);
        result.first += (p[i] / (i + 1)) * pow1;
        result.first[0] -= (p[i] / (i + 1)) * pow(a, i + 1);
        result.second += (-p[i] / (i + 1)) * pow2;
        result.second[0] += (p[i] / (i + 1)) * pow(a + 1, i + 1);
    }
    return result;
}

// Represents a probability distribution function supported on [left, right].
// `pieces[i]` is the polynomial that provides the value on the interval
// [left + i, left + i + 1].
struct PDF {
    int left = 0;
    int right = 0;
    vector<Poly> pieces;
};

// Computes the convolution of `pdf` with the function g defined by
// g(x) = | val if b <= x <= b + 1
//        | 0   otherwise
PDF conv(const PDF& pdf, int b, double val) {
    PDF result;
    result.left = pdf.left + b;
    result.right = pdf.right + b + 1;
    result.pieces.resize(result.right - result.left);
    for (int a = pdf.left; a < pdf.right; ++a) {
        const auto c = conv(pdf.pieces[a - pdf.left], a, b);
        result.pieces[a - pdf.left] += val*c.first;
        result.pieces[a - pdf.left + 1] += val*c.second;
    }
    return result;
}

PDF operator+(const PDF& pdf1, const PDF& pdf2) {
    PDF result;
    result.left = min(pdf1.left, pdf2.left);
    result.right = max(pdf1.right, pdf2.right);
    result.pieces.resize(result.right - result.left);
    for (int i = result.left; i < result.right; i++) {
        if (i >= pdf1.left && i < pdf1.right) {
            result.pieces[i - result.left] += pdf1.pieces[i - pdf1.left];
        }
        if (i >= pdf2.left && i < pdf2.right) {
            result.pieces[i - result.left] += pdf2.pieces[i - pdf2.left];
        }
    }
    return result;
}

// Computes the convolution of `pdf` with the function `g` defined by
// g(x) = | val if a <= x <= b
//        | 0   otherwise
PDF conv(const PDF& pdf, int a, int b, double val) {
    PDF result;
    while (a < b) {
        result = result + conv(pdf, a, val);
        ++a;
    }
    return result;
}

void do_testcase() {
    int N, A, B; scanf("%d %d %d", &N, &A, &B);
    int X0; scanf("%d", &X0);
    PDF pdf;
    pdf.left = -X0;
    pdf.right = X0;
    pdf.pieces.resize(2*X0);
    for (auto& p : pdf.pieces) p[0] = 0.5/X0;
    for (int i = 1; i < N; i++) {
        int X; scanf("%d", &X);
        pdf = conv(pdf, -X, X, 0.5/X);
    }
    A = max(A, pdf.left);
    B = min(B, pdf.right);
    double result = 0;
    while (A < B) {
        const Poly& piece = pdf.pieces[A - pdf.left];
        for (int i = 0; i < 10; i++) {
            result += piece[i] * (pow(A + 1, i + 1) - pow(A, i + 1)) / (i + 1);
        }
        ++A;
    }
    printf("%.9f\n", result);
}

int main() {
    int T; scanf("%d", &T); while (T--) do_testcase();
}
