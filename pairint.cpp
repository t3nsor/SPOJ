// 2024-07-29
// This problem was never updated to allow C++14, so we have to do a few things
// the hard way.
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
using namespace std;
// return all pairs X + Y = N where Y is X with the least significant digit
// deleted
vector<pair<long long, long long> > solve(long long N) {
    vector<pair<long long, long long> > result;
    for (int i = 0; i <= 9; i++) {
        // try with `i` as the last digit of X
        long long X = i;
        long long Y = 0;
        long long P = 1;
        while (N > X + Y) {
            int digit = ((N - X) / P) % 10;
            Y += P * digit;
            P *= 10;
            X += P * digit;
        }
        if (N == X + Y) {
            result.push_back(make_pair(X, Y));
        }
    }
    return result;
}
void do_testcase() {
    long long N; cin >> N;
    const vector<pair<long long, long long> > v = solve(N);
    set<pair<long long, long long> > result(v.begin(), v.end());
    for (long long P = 10; P <= N; P *= 10) {
        if (N % 2 != 0) continue;
        for (int j = 0; j <= 1; j++) {
            const long long N2 = (N / P) - j;
            const long long rem = (N % P) + (j * P);
            if (N2 <= 0) continue;
            const vector<pair<long long, long long> > v = solve(N2);
            for (int k = 0; k < v.size(); k++) {
                result.insert(make_pair(v[k].first * P + (rem / 2),
                                        v[k].second * P + (rem / 2)));
            }
        }
    }
    cout << result.size() << '\n';
    for (set<pair<long long, long long> >::iterator i = result.begin();
         i != result.end();
         i++) {
        ostringstream ss;
        ss << i->first;
        const string first = ss.str();
        ss.str("");
        ss << i->second;
        string second = ss.str();
        second.insert(0, first.size() - second.size() - 1, '0');
        cout << first << " + " << second << " = " << N << '\n';
    }
}
int main() {
    int t; cin >> t;
    while (t--) do_testcase();
}
