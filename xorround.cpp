// 2023-10-23
// we can probably find an explicit formula for the matrix power, but I decided
// to just be lazy
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
void multiply(vector<char>& A, const vector<char>& B) {
    const int N = A.size();
    vector<char> result(N);
    for (int k = 0; k < N; k++) {
        for (int j = 0; j < N; j++) {
            result[k] += A[j] * B[(k - j + N) % N];
        }
        result[k] %= 2;
    }
    swap(A, result);
}
void do_testcase() {
    int N; scanf("%d", &N);
    unsigned K; scanf("%u", &K);
    vector<char> M(N);
    M[0] = M[1] = M[N - 1] = 1;
    vector<char> P(N);
    P[0] = 1;
    unsigned exponent = 1;
    while ((-2*exponent)&K) exponent <<= 1;
    while (exponent) {
        multiply(P, P);
        if (exponent & K) multiply(P, M);
        exponent >>= 1;
    }
    vector<int> A(N);
    for (int i = 0; i < N; i++) scanf("%d", &A[i]);
    for (int i = 0; i < N; i++) {
        int result = 0;
        for (int j = 0; j < N; j++) {
            if (P[(j - i + N) % N]) {
                result ^= A[j];
            }
        }
        if (i > 0) putchar(' ');
        printf("%d", result);
    }
    putchar('\n');
}
int main() {
    int T; scanf("%d", &T);
    while (T--) do_testcase();
}
