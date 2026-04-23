// 2026-04-23
// The hardest part about this problem is figuring out how to achieve the
// desired precision.  The conceptual part is easy.  When the COM is above the
// water level, adding water shifts the COM toward the added water, lowering it.
// When the COM is at or below the water level, adding water similarly shifts it
// up.  Therefore the COM, as a function of water level, is a bitonic function
// and if it achieves a maximum in the interior of the interval, it must be at a
// point where it coincides with the top of the water level.  Note that the COM
// can be in the base of the glass, in which case no water should be added.  If
// it's not in the base, then we can add water in small increments of height,
// and stop once the water level reaches the current COM.
//
// Since the radius of the glass is never more than 100, a water height
// increment of dx can result in an increment to, at most, pi*100^2 dx.
// However, there is also a question of how accurately we can actually calculate
// the water level and the COM for a given water level.  The actual error bounds
// depend on being able to bound the derivative, which we cannot do because we
// are given arbitrary rational function inputs with no restrictions other than
// 0.1 <= T(x) < R(x) <= 100.  So I initially decided to allocate an equal error
// tolerance to both sources of error, and take the increment size to be
// 0.5/(pi*100^2).  But that got TLE on SPOJ, so I changed it to 1/(pi*100^2).
// (BTW, I cheated by looking at the official test data.  As a justification for
// my actions, I want to say that the way the problem is specified is kinda
// bad: in order to get the precisely correct rounded version of the result,
// you need potentially infinite precision.  It would be better if they said
// your output has to be within 0.001 of the correct answer.  But the truth is
// this problem is just a slog and I wanted to be done with it.)

#include <ctype.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;
const double pi = acos(-1.0);
const double width = 0.0001/pi;
const double D = 2.5;

struct Token {
    double value;  // used only for CONST
    enum Kind {
        X,
        CONST,
        ADD,
        NEG,
        SUB,
        MUL,
        DIV,
        LPAREN,  // does not appear in final result
    } kind;
    Token(Kind kind, double value = 0) : kind(kind), value(value) {}
};

vector<Token> parse(string expr) {
    vector<Token> result;
    vector<Token> opstack;
    expr = "(" + expr + ")";
    int i = 0;
    while (i < expr.size()) {
        if (isdigit(expr[i]) || expr[i] == '.') {
            int j = i + 1;
            while (j < expr.size() && (isdigit(expr[j]) || expr[j] == '.')) {
                j++;
            }
            string num = expr.substr(i, j - i);
            result.push_back(Token{Token::CONST, stod(num)});
            i = j;
            continue;
        }
        if (expr[i] == 'x') {
            result.push_back(Token::X);
        } else if (expr[i] == '(') {
            opstack.push_back(Token::LPAREN);
        } else if (expr[i] == ')') {
            while (opstack.back().kind != Token::LPAREN) {
                result.push_back(opstack.back());
                opstack.pop_back();
            }
            opstack.pop_back();
        } else if (expr[i] == '-' || expr[i] == '+') {
            // A minus is binary if preceded by a number or close parenthesis;
            // otherwise, it's unary
            Token::Kind kind = (expr[i] == '+'
                                ? Token::ADD
                                : (i > 0 &&
                                   (expr[i - 1] == ')' ||
                                    expr[i - 1] == '.' ||
                                    isdigit(expr[i - 1])))
                                  ? Token::SUB
                                  : Token::NEG);
            while (!opstack.empty() &&
                   opstack.back().kind != Token::LPAREN) {
                result.push_back(opstack.back());
                opstack.pop_back();
            }
            opstack.push_back(kind);
        } else if (expr[i] == '*' || expr[i] == '/') {
            Token::Kind kind = (expr[i] == '*' ? Token::MUL : Token::DIV);
            while (!opstack.empty() &&
                   (opstack.back().kind == Token::MUL ||
                    opstack.back().kind == Token::DIV)) {
                result.push_back(opstack.back());
                opstack.pop_back();
            }
            opstack.push_back(kind);
        }
        i++;
        // note that whitespace is just skipped; this handles carriage returns
        // or other similar BS
    }
    return result;
}

double eval(const vector<Token>& expr, double x) {
    vector<double> S;
    for (const Token& token: expr) {
        if (token.kind == Token::X) {
            S.push_back(x);
        } else if (token.kind == Token::CONST) {
            S.push_back(token.value);
        } else if (token.kind == Token::NEG) {
            S.back() = -S.back();
        } else {
            const double b = S.back();
            S.pop_back();
            const double a = S.back();
            if (token.kind == Token::ADD) {
                S.back() = a + b;
            } else if (token.kind == Token::SUB) {
                S.back() = a - b;
            } else if (token.kind == Token::MUL) {
                S.back() = a*b;
            } else {
                S.back() = a/b;
            }
        }
    }
    return S[0];
}

string getl() {
    string result;
    char c;
    do {
        c = getchar();
    } while (c <= 32);
    do {
        result.push_back(c);
        c = getchar();
    } while (c != '\n');
    return result;
}

void do_testcase(double H, double B) {
    auto Rf = parse(getl());
    auto Tf = parse(getl());
    vector<double> R;
    vector<double> r;
    for (int i = 0; i * width <= H; i++) {
        double Rval = eval(Rf, i * width);
        R.push_back(Rval);
        if (i * width >= B) {
            double tval = eval(Tf, i * width);
            r.push_back(Rval - tval);
        } else {
            r.push_back(0);
        }
    }
    // calculate mass and COM of empty cup
    double M0 = 0;
    double Mh0 = 0;
    for (int i = 1; i < R.size(); i++) {
        double h = (i - 0.5) * width;
        double out = R[i-1]*R[i-1] + R[i-1]*R[i] + R[i]*R[i];
        double in = r[i-1]*r[i-1] + r[i-1]*r[i] + r[i]*r[i];
        double dM = D*width*pi*(out-in)/3;
        M0 += dM;
        Mh0 += dM*h;
    }
    if (Mh0/M0 <= B) {
        puts("Pour 0.000 litres / 0.000 cm of water.");
        return;
    }
    // handle the partial first slice
    int i = 0; while (i*width < B) ++i;
    // paranoia: B is very close to H
    if (i*width > H) {
        puts("Pour 0.000 litres / 0.000 cm of water.");
        return;
    }
    double w = i*width - B;
    double rl = (1 - w)*r[i] + w*r[i - 1];
    double vol = rl*rl + rl*R[i] + R[i]*R[i];
    double M1 = w*pi*vol/3;
    double Mh1 = M1 * (i*width - w/2);
    if ((Mh0 + Mh1) / (M0 + M1) <= i*width) {
        // we may have overshot already; hope this is close enough anyway
        printf("Pour %.3f litres / %.3f cm of water.\n", M1 / 1000, w);
        return;
    }
    while (i + 1 < r.size()) {
        double vol = r[i]*r[i] + r[i]*r[i+1] + r[i+1]*r[i+1];
        double dM = width*pi*vol/3;
        double dMh = dM*((i + 0.5)*width);
        double com2 = (Mh0 + Mh1 + dMh) / (M0 + M1 + dM);
        if (com2 < (i + 1)*width) {
            // improve accuracy by taking the weighted avg
            double com1 = (Mh0 + Mh1) / (M0 + M1);
            double d1 = com1 - i*width;
            double d2 = (i+1)*width - com2;
            printf("Pour %.3f litres / %.3f cm of water.\n",
                   (d2*M1 + d1*(M1 + dM)) / (1000*(d1 + d2)),
                   (d2*i*width + d1*(i+1)*width) / (d1 + d2) - B);
            return;
        }
        M1 += dM;
        Mh1 += dMh;
        ++i;
    }
    printf("Pour %.3f litres / %.3f cm of water.\n", M1 / 1000, i*width - B);
}

int main() {
    for (;;) {
        double H, B; cin >> H >> B;
        if (H == 0 && B == 0) break;
        do_testcase(H, B);
    }
}
