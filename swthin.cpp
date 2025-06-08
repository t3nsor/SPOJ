// 2025-06-07
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <stdio.h>
#include <vector>
using namespace std;
int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    return a == 0 ? b : gcd(b % a, a);
}
struct Line {
    int A;
    int B;
    int C;
    Line(int a, int b, int c) {
        int g = gcd(a, gcd(b, c));
        if (a < 0 || (a == 0 && b < 0)) g = -g;
        A = a / g;
        B = b / g;
        C = c / g;
    }
};
void do_testcase(int cs, int N) {
    vector<pair<int, int>> points(N);
    for (int i = 0; i < N; i++) {
        scanf("%d %d", &points[i].first, &points[i].second);
    }
    int result = 0;
    for (int i = 0; i < N; i++) {
        unordered_map<int, int> M;
        for (int j = 0; j < N; j++) {
            if (j == i) continue;
            int dx = points[j].first - points[i].first;
            int dy = points[j].second - points[i].second;
            int g = gcd(dx, dy);
            if (dx < 0 || (dx == 0 && dy < 0)) g = -g;
            dx /= g;
            dy /= g;
            const int k = 30000 * dx + dy;
            M[k]++;
        }
        for (const auto& kv : M) {
            result = max(result, kv.second + 1);
        }
    }
    if (result < 4) result = 0;
    printf("Photo %d: %d points eliminated\n", cs, result);
}
int main() {
    for (int cs = 1;; cs++) {
        int N; scanf("%d", &N);
        if (!N) break;
        do_testcase(cs, N);
    }
}
