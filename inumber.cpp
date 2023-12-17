// 2023-12-16
#include <algorithm>
#include <deque>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;
struct node {
    int sum;
    int rem;
    int digit;
    node* parent;
};
void do_testcase() {
    int n; cin >> n;
    deque<node> D;
    queue<node*> Q;
    D.push_back(node{0, 0, -1, nullptr});
    Q.push(&D.back());
    vector<vector<char>> vis(n + 1, vector<char>(n, 0));
    vis[0][0] = 1;
    while (true) {
        node* p = Q.front();
        Q.pop();
        for (int i = 0; i < 10; i++) {
            int sum2 = p->sum + i;
            if (sum2 > n) continue;
            int rem2 = (10 * (p->rem) + i) % n;
            if (vis[sum2][rem2]) continue;
            vis[sum2][rem2] = 1;
            D.push_back(node{sum2, rem2, i, p});
            if (sum2 == n && rem2 == 0) goto done;
            Q.push(&D.back());
        }
    }
done:
    string result;
    node* p = &D.back();
    while (p->digit >= 0) {
        result.push_back(p->digit + '0');
        p = p->parent;
    }
    reverse(result.begin(), result.end());
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
