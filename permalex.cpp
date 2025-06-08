// 2025-06-07
#include <algorithm>
#include <iostream>
#include <limits.h>
#include <map>
#include <string>
#include <vector>
using namespace std;
int nCr[31][31];
long long fact[31];
int nperm(vector<int> freq) {
    int n = 0;
    int result = 1;
    for (int i = 0; i < freq.size(); i++) {
        n += freq[i];
        result *= nCr[n][freq[i]];
    }
    return result;
}
void do_testcase(string s) {
    map<char, int> freq;
    for (const char c : s) freq[c]++;
    int result = 0;
    for (int i = 0; i < s.size(); i++) {
        for (auto it = freq.begin(); it->first < s[i]; ++it) {
            vector<int> freqv;
            for (const auto& kv : freq) {
                freqv.push_back(kv.second - (kv.first == it->first));
            }
            result += nperm(freqv);
        }
        freq[s[i]]--;
    }
    printf("% 10d\n", result + 1);
}
int main() {
    for (int n = 0; n <= 30; n++) {
        nCr[n][0] = nCr[n][n] = 1;
        for (int k = 1; k < n; k++) {
            nCr[n][k] = nCr[n-1][k-1] + nCr[n-1][k];
        }
    }
    fact[0] = 1;
    for (int n = 1; n <= 30; n++) {
        fact[n] = n*fact[n-1];
        if (fact[n] > INT_MAX) fact[n] = INT_MAX;
    }
    for (;;) {
        string s;
        getline(cin, s);
        if (s == "#") break;
        do_testcase(s);
    }
}
