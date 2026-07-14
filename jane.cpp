// 2026-07-14
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <vector>
using namespace std;
int N, D;
vector<int> v;
set<pair<int, int>> vis;
void dfs(int i, int j) {
    if (i > j) swap(i, j);
    if (!vis.emplace(i, j).second) return;
    for (int d1 = -1; d1 <= 1; d1 += 2) {
        for (int d2 = -1; d2 <= 1; d2 += 2) {
            const int i2 = i + d1;
            const int j2 = j + d2;
            if (i2 >= 0 && i2 < N && j2 >= 0 && j2 < N &&
                abs(v[i2] - v[j2]) <= D) {
                dfs(i2, j2);
            }
        }
    }
}
int main() {
    scanf("%d %d", &N, &D);
    for (int i = 0; i < N; i++) { int x; scanf("%d", &x); v.push_back(x); }
    for (int i = 0; i < N; i++) {
        dfs(i, i);
        if (i + 1 < N && abs(v[i] - v[i + 1]) <= D) dfs(i, i + 1);
    }
    for (const auto& p : vis) {
        if (p.first == p.second) continue;
        printf("%d %d\n", p.first + 1, p.second + 1);
    }
}
