// 2023-10-20
#include <iostream>
#include <limits.h>
#include <queue>
#include <string.h>
#include <tuple>
#include <utility>
#include <vector>
using namespace std;
int dist[50][50][50][50];
void do_testcase() {
    int n; cin >> n;
    int cap[4] = {0};
    int dest[4];
    for (int i = 0; i < n; i++) {
        cin >> cap[i];
    }
    for (int i = 0; i < n; i++) {
        cin >> dest[i];
    }
    for (int i = n; i < 4; i++) {
        cap[i] = 0;
        dest[i] = 0;
    }
    for (int i = 0; i <= cap[0]; i++) {
        for (int j = 0; j <= cap[1]; j++) {
            for (int k = 0; k <= cap[2]; k++) {
                memset(dist[i][j][k], 0x7f, sizeof(int) * (1 + cap[3]));
            }
        }
    }
    queue<tuple<int, int, int, int, int>> Q;
    Q.emplace(cap[0], cap[1], cap[2], cap[3], 0);
    while (!Q.empty()) {
        auto t = Q.front();
        Q.pop();
        if (dist[get<0>(t)][get<1>(t)][get<2>(t)][get<3>(t)] < 0x7f7f7f7f) {
            continue;
        }
        dist[get<0>(t)][get<1>(t)][get<2>(t)][get<3>(t)] = get<4>(t);
        if (get<0>(t) == dest[0] && get<1>(t) == dest[1] &&
            get<2>(t) == dest[2] && get<3>(t) == dest[3]) {
            cout << get<4>(t) << '\n';
            return;
        }
        get<4>(t)++;
        if (get<0>(t) > 0) {
            auto t2 = t;
            get<0>(t2) = 0;
            Q.push(t2);
        }
        if (get<1>(t) > 0) {
            auto t2 = t;
            get<1>(t2) = 0;
            Q.push(t2);
        }
        if (get<2>(t) > 0) {
            auto t2 = t;
            get<2>(t2) = 0;
            Q.push(t2);
        }
        if (get<3>(t) > 0) {
            auto t2 = t;
            get<3>(t2) = 0;
            Q.push(t2);
        }
        if (get<0>(t) > 0) {
            const int pour = min(get<0>(t), cap[1] - get<1>(t));
            auto t2 = t;
            get<0>(t2) -= pour;
            get<1>(t2) += pour;
            Q.push(t2);
        }
        if (get<0>(t) > 0) {
            const int pour = min(get<0>(t), cap[2] - get<2>(t));
            auto t2 = t;
            get<0>(t2) -= pour;
            get<2>(t2) += pour;
            Q.push(t2);
        }
        if (get<0>(t) > 0) {
            const int pour = min(get<0>(t), cap[3] - get<3>(t));
            auto t3 = t;
            get<0>(t3) -= pour;
            get<3>(t3) += pour;
            Q.push(t3);
        }
        if (get<1>(t) > 0) {
            const int pour = min(get<1>(t), cap[0] - get<0>(t));
            auto t2 = t;
            get<1>(t2) -= pour;
            get<0>(t2) += pour;
            Q.push(t2);
        }
        if (get<1>(t) > 0) {
            const int pour = min(get<1>(t), cap[2] - get<2>(t));
            auto t2 = t;
            get<1>(t2) -= pour;
            get<2>(t2) += pour;
            Q.push(t2);
        }
        if (get<1>(t) > 0) {
            const int pour = min(get<1>(t), cap[3] - get<3>(t));
            auto t3 = t;
            get<1>(t3) -= pour;
            get<3>(t3) += pour;
            Q.push(t3);
        }
        if (get<2>(t) > 0) {
            const int pour = min(get<2>(t), cap[0] - get<0>(t));
            auto t2 = t;
            get<2>(t2) -= pour;
            get<0>(t2) += pour;
            Q.push(t2);
        }
        if (get<2>(t) > 0) {
            const int pour = min(get<2>(t), cap[1] - get<1>(t));
            auto t2 = t;
            get<2>(t2) -= pour;
            get<1>(t2) += pour;
            Q.push(t2);
        }
        if (get<2>(t) > 0) {
            const int pour = min(get<2>(t), cap[3] - get<3>(t));
            auto t2 = t;
            get<2>(t2) -= pour;
            get<3>(t2) += pour;
            Q.push(t2);
        }
        if (get<3>(t) > 0) {
            const int pour = min(get<3>(t), cap[0] - get<0>(t));
            auto t2 = t;
            get<3>(t2) -= pour;
            get<0>(t2) += pour;
            Q.push(t2);
        }
        if (get<3>(t) > 0) {
            const int pour = min(get<3>(t), cap[1] - get<1>(t));
            auto t2 = t;
            get<3>(t2) -= pour;
            get<1>(t2) += pour;
            Q.push(t2);
        }
        if (get<3>(t) > 0) {
            const int pour = min(get<3>(t), cap[2] - get<2>(t));
            auto t2 = t;
            get<3>(t2) -= pour;
            get<2>(t2) += pour;
            Q.push(t2);
        }
    }
    cout << "NO\n";
}
int main() {
    int t; cin >> t;
    while (t--) do_testcase();
}
