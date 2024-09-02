// 2024-09-02
// This problem is easy once you figure out what it means; the only possibly
// tricky aspect is avoiding floating-point imprecision, e.g, a planet whose
// export costs 0.95 has exactly the same value as a planet whose export costs
// 1.00 but is one connection more distant, and we need to actually distinguish
// between equality and small differences. To handle this we initially scale all
// prices to integers so that any multiplication by 0.95 that yields an integer
// result will be exact.
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;
void do_testcase(int N) {
    int dist[27][27];
    int price[26];
    for (int i = 0; i < 26; i++) {
        price[i] = -1;
        for (int j = 0; j < 26; j++) {
            dist[i][j] = i == j ? 0 : 100;
        }
    }
    for (int i = 0; i < N; i++) {
        string name, price_str, adj;
        cin >> name >> price_str >> adj;
        price[name[0] - 'A'] = 100 * (price_str[0] - '0') +
                               10 * (price_str[2] - '0') +
                               (price_str[3] - '0');
        for (const char c : adj) {
            if (c == '*') {
                dist[name[0] - 'A'][26] = dist[26][name[0] - 'A'] = 1;
            } else {
                dist[name[0] - 'A'][c - 'A'] = dist[c - 'A'][name[0] - 'A'] = 1;
            }
        }
    }
    for (int k = 0; k < 27; k++) {
        for (int i = 0; i < 27; i++) {
            for (int j = 0; j < 27; j++) {
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }
    double best_price = -1;
    char best_planet;
    for (int i = 0; i < 26; i++) {
        double p = price[i];
        if (dist[26][i] >= 100) continue;
        for (int j = 0; j < dist[26][i]; j++) {
            p = 19*p / 20;
        }
        if (p > best_price) {
            best_price = p;
            best_planet = i + 'A';
        }
    }
    cout << "Import from " << best_planet << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int N;
    while (cin >> N) do_testcase(N);
}
