// 2024-03-02
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int dist(const string& s1, const string& s2) {
    int rev1[5], rev2[5];
    for (int i = 0; i < 5; i++) {
        rev1[s1[i] - 'A'] = i;
        rev2[s2[i] - 'A'] = i;
    }
    int result = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < i; j++) {
            result += (rev1[i] > rev1[j]) ^ (rev2[i] > rev2[j]);
        }
    }
    return result;
}
void do_testcase(int n) {
    vector<string> v;
    for (int i = 0; i < n; i++) {
        v.emplace_back(); cin >> v.back();
    }
    string m = "ABCDE";
    string bestr;
    int bestv = 1e6;
    do {
        int value = 0;
        for (const auto& s : v) {
            value += dist(s, m);
        }
        if (value < bestv) {
            bestv = value;
            bestr = m;
        }
    } while (next_permutation(m.begin(), m.end()));
    cout << bestr << " is the median ranking with value " << bestv << ".\n";
}
int main() {
    ios::sync_with_stdio(false);
    int n;
    for (;;) {
        cin >> n;
        if (n == 0) return 0;
        do_testcase(n);
    }
}
