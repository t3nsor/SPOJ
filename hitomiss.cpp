// 2024-03-02
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
void do_testcase() {
    int N; cin >> N;
    vector<queue<int>> deck(N);
    vector<int> pos(N, 1);
    for (int i = 0; i < 52; i++) {
        int card; cin >> card; deck[0].push(card);
    }
    vector<int> last(N);
    int done = 0;
    int counter = 0;
    while (done < N) {
        if (counter == 663) {
            // (51 times 13)
            cout << " unwinnable\n";
            return;
        }
        ++counter;
        vector<int> passed(N, 0);
        for (int i = 0; i < N; i++) {
            if (deck[i].empty()) continue;
            if (deck[i].front() == pos[i]) {
                passed[i] = pos[i];
                counter = 0;
            } else {
                const int card = deck[i].front();
                deck[i].push(card);
            }
            pos[i]++;
            if (pos[i] == 14) pos[i] = 1;
            deck[i].pop();
        }
        if (deck[done].empty()) ++done;
        for (int i = 0; i < N; i++) {
            if (passed[i] > 0) {
                last[i] = passed[i];
                if (i + 1 < N) {
                    deck[i + 1].push(passed[i]);
                }
            }
        }
    }
    for (int i = 0; i < N; i++) {
        cout << ' ' << last[i];
    }
    cout << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    for (int i = 1; i <= T; i++) {
        cout << "Case " << i << ':';
        do_testcase();
    }
}
