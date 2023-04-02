// 2023-04-02
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <utility>
#include <vector>
using namespace std;
constexpr int SIZE = 100000;
struct Segtree {
   vector<int> a = vector<int>(4 * SIZE, 0); 
   int K;
   Segtree(int K) : K(K) {}
   void load(int node, int begin, int end, int& r, int v) {
       if (end - begin == 1) {
           const int fit = (K - a[node]) / v;
           const int use = min(fit, r);
           r -= use;
           a[node] += v * use;
           return;
       }
       const int mid = (begin + end) / 2;
       const int left = 2 * node;
       const int right = 2 * node + 1;
       if (K - a[left] >= v) {
           load(left, begin, mid, r, v);
       }
       if (r > 0 && K - a[right] >= v) {
           load(right, mid, end, r, v);
       }
       a[node] = min(a[left], a[right]);
   }
   pair<int, long long> calc_waste(int node, int begin, int end) {
       if (end - begin == 1) {
           if (a[node]) {
               return {1, K - a[node]};
           } else {
               return {0, 0};
           }
       }
       const int mid = (begin + end) / 2;
       const int left = 2 * node;
       const int right = 2 * node + 1;
       const auto lresult = calc_waste(left, begin, mid);
       const auto rresult = calc_waste(right, mid, end);
       return {lresult.first + rresult.first, lresult.second + rresult.second};
   }
};
void do_testcase() {
    int K; scanf("%d", &K);  // capacity of each warehouse
    int n; scanf("%d", &n);  // total number of containers
    Segtree segtree(K);
    while (n > 0) {
        char c;
        int r, v;
        do {
            c = getchar();
        } while (isspace(c));
        if (c == 'b') {
            scanf("%d %d", &r, &v);
        } else {
            ungetc(c, stdin);
            r = 1;
            scanf("%d", &v);
        }
        n -= r;
        segtree.load(1, 0, SIZE, r, v);
        if (r != 0) throw;
    }
    const auto result = segtree.calc_waste(1, 0, SIZE);
    printf("%d %lld\n", result.first, result.second);
}
int main() {
    int T; scanf("%d", &T);
    while (T--) do_testcase();
}
