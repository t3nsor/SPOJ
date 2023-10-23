// 2023-10-22
#include <algorithm>
#include <array>
#include <stack>
#include <stdio.h>
#include <utility>
#include <vector>
using namespace std;
void do_testcase(int n) {
    if (n == 1) {
        puts("0123456789");
        return;
    }
    int V = 1; for (int i = 0; i < n; i++) V *= 10;
    vector<array<int, 10>> adj(V / 10);
    for (int i = 0; i < V / 10; i++) {
        for (int j = 0; j < 10; j++) {
            adj[i][j] = 10 * (i % (V / 100)) + j;
        }
    }
    stack<pair<int, int>> S;
    vector<int> out;
    int cur = 0;
    for (;;) {
        int neighbor = -1;
        for (int i = 0; i < 10; i++) {
            if (adj[cur][i] >= 0) {
                neighbor = i;
                break;
            }
        }
        if (neighbor >= 0) {
            S.emplace(cur, neighbor);
            int next = adj[cur][neighbor];
            adj[cur][neighbor] = -1;
            cur = next;
        } else {
            if (S.empty()) break;
            auto p = S.top();
            S.pop();
            out.push_back(p.second);
            cur = p.first;
        }
    }
    reverse(out.begin(), out.end());
    for (int i = 0; i < n - 1; i++) {
        putchar('0');
    }
    for (const int d : out) {
        putchar('0' + d);
    }
    putchar('\n');
}
int main() {
    int n;
    for (;;) {
        scanf("%d", &n);
        if (n == 0) break; else do_testcase(n);
    }
}
