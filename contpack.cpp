// 2023-09-28
// This is a pretty simple greedy algorithm. First, pack all containers of size
// 0 (i.e. volume 1) with boxes of size 0, if possible. When packing containers
// of size 1, we can either use boxes of size 1 or pairs of boxes of size 0,
// whichever is cheaper. So we pair up all remaining boxes of size 0 in
// ascending order of value, treating each such pair as if it were a box of size
// 1. Then, if any boxes of size 1 are unused after filling containers of size
// 1 (including boxes that were synthesized from boxes of size 0) we combine
// pairs of these into boxes of size 2, and so on. This works because if at any
// point we have an unpaired box of a given size, it will be impossible to
// combine it with any number of unpaired boxes of a larger size in order to
// fill a container of yet larger size (2^{a_0} + ... + 2^{a_n} < 2^{a_n + 1})
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
void do_testcase() {
    int n; cin >> n;
    using Q = priority_queue<int, vector<int>, greater<int>>;
    vector<Q> boxes;
    while (n--) {
        int size, value;
        cin >> size >> value;
        if (boxes.size() <= size) boxes.resize(size + 1);
        boxes[size].push(value);
    }
    int q; cin >> q;
    vector<int> containers;
    while (q--) {
        int size, count;
        cin >> size >> count;
        if (containers.size() <= size) containers.resize(size + 1);
        containers[size] += count;
    }
    if (boxes.size() < containers.size()) boxes.resize(containers.size());
    int result = 0;
    for (int i = 0; i < containers.size(); i++) {
        if (i > 0) {
            while (boxes[i - 1].size() >= 2) {
                int v1 = boxes[i - 1].top();
                boxes[i - 1].pop();
                int v2 = boxes[i - 1].top();
                boxes[i - 1].pop();
                boxes[i].push(v1 + v2);
            }
        }
        while (containers[i]--) {
            if (boxes[i].empty()) {
                cout << "No\n";
                return;
            }
            result += boxes[i].top();
            boxes[i].pop();
        }
    }
    cout << result << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) do_testcase();
}
