// 2024-09-23
// See https://math.stackexchange.com/q/1871783/24692 for explanation
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
void print_cycle(vector<int> cycle) {
    cout << ' ' << cycle[0] + 1 << '-' << cycle[1] + 1;
}
void gen_first_perm(vector<vector<int>>& result, const vector<int>& cycle) {
    const int N = cycle.size();
    result.push_back({cycle[0], cycle[1]});
    int i = 2;
    int j = N - 1;
    while (i < j) {
        result.push_back({cycle[i], cycle[j]});
        i++;
        j--;
    }
}
void gen_second_perm(vector<vector<int>>& result, const vector<int>& cycle) {
    const int N = cycle.size();
    int i = 1;
    int j = N - 1;
    while (i < j) {
        result.push_back({cycle[i], cycle[j]});
        i++;
        j--;
    }
}
int main() {
    int n; cin >> n;
    vector<int> v(n);
    for (int i = 0; i < n; i++) {
        cin >> v[i]; --v[i];
    }
    vector<int> vis(n, 0);
    vector<vector<int>> cycles;
    int maxlen = 0;
    for (int i = 0; i < n; i++) {
        if (vis[i]) continue;
        vis[i] = 1;
        vector<int> cycle = {i};
        for (int j = v[i]; j != i; j = v[j]) {
            cycle.push_back(j);
            vis[j] = 1;
        }
        maxlen = max(maxlen, (int)cycle.size());
        if (cycle.size() > 1) cycles.push_back(move(cycle));
    }
    if (maxlen == 1) {
        cout << "0\n";
        return 0;
    }
    if (maxlen == 2) {
        cout << "1\n" << cycles.size();
        for (const auto& cycle : cycles) print_cycle(cycle);
        cout << '\n';
        return 0;
    }
    vector<vector<int>> result[2];
    for (const auto& cycle : cycles) {
        gen_first_perm(result[0], cycle);
        gen_second_perm(result[1], cycle);
    }
    cout << "2\n";
    cout << result[0].size();
    for (const auto& cycle : result[0]) print_cycle(cycle);
    cout << '\n' << result[1].size();
    for (const auto& cycle : result[1]) print_cycle(cycle);
    cout << '\n';
}
