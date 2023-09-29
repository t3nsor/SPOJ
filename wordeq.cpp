// 2023-09-28
#include <iostream>
#include <string>
#include <vector>
using namespace std;
struct bignum {
    string rep;
    bignum() : rep("\001") {}

    void double_me() {
        int carry = 0;
        for (int i = 0; i < rep.size(); i++) {
            if (10 <= (rep[i] = 2 * rep[i] + carry)) {
                rep[i] -= 10;
                carry = 1;
            } else {
                carry = 0;
            }
        }
        if (carry) rep.push_back(1);
    }

    void print() {
        for (int i = rep.size() - 1; i >= 0; i--) {
            putchar(rep[i] + '0');
        }
        putchar('\n');
    }
};

vector<int> translate(const string& s, const vector<int>& L,
                      const vector<int>& Lsum) {
    vector<int> result;
    for (const char c : s) {
        if (c == '0') {
            result.push_back(-1);
        } else if (c == '1') {
            result.push_back(-2);
        } else {
            for (int i = 0; i < L[c - 'a']; i++) {
                result.push_back(i + Lsum[c - 'a']);
            }
        }
    }
    return result;
}

void dfs(vector<int>& cc, const vector<vector<int>>& adjlist,
         int v, int ccnt) {
    cc[v] = ccnt;
    for (const auto w : adjlist[v]) {
        if (cc[w] < 0) dfs(cc, adjlist, w, ccnt);
    }
}

void do_testcase() {
    int k; cin >> k;
    vector<int> L(k);
    vector<int> Lsum(k + 1);
    for (int i = 0; i < k; i++) {
        cin >> L[i];
        Lsum[i + 1] = Lsum[i] + L[i];
    }
    int unused_len;
    string lstr, rstr;
    cin >> unused_len;
    cin >> lstr;
    cin >> unused_len;
    cin >> rstr;
    vector<int> left = translate(lstr, L, Lsum);
    vector<int> right = translate(rstr, L, Lsum);
    if (left.size() != right.size()) {
        puts("0"); return;
    }
    // build adjacency list
    vector<vector<int>> adjlist(Lsum[k]);
    vector<int> is0(Lsum[k]);
    vector<int> is1(Lsum[k]);
    for (int i = 0; i < left.size(); i++) {
        if (left[i] == -1 && right[i] >= 0) {
            is0[right[i]] = true;
        } else if (left[i] == -2 && right[i] >= 0) {
            is1[right[i]] = true;
        } else if (left[i] >= 0 && right[i] == -1) {
            is0[left[i]] = true;
        } else if (left[i] >= 0 && right[i] == -2) {
            is1[left[i]] = true;
        } else if (left[i] >= 0 && right[i] >= 0) {
            adjlist[left[i]].push_back(right[i]);
            adjlist[right[i]].push_back(left[i]);
        } else if (left[i] != right[i]) {
            puts("0");
            return;
        }
    }
    // assign connected components
    int ccnt = 0;
    vector<int> cc(Lsum[k], -1);
    for (int i = 0; i < Lsum[k]; i++) {
        if (cc[i] == -1) {
            dfs(cc, adjlist, i, ccnt);
            ++ccnt;
        }
    }
    vector<int> cis0(ccnt);
    vector<int> cis1(ccnt);
    for (int i = 0; i < Lsum[k]; i++) {
        if (is0[i]) cis0[cc[i]] = true;
        if (is1[i]) cis1[cc[i]] = true;
    }
    int unknown = 0;
    for (int i = 0; i < ccnt; i++) {
        if (cis0[i] && cis1[i]) {
            puts("0");
            return;
        }
        if (!cis0[i] && !cis1[i]) ++unknown;
    }
    bignum result;
    while (unknown--) {
        result.double_me();
    }
    result.print();
}

int main() {
    int t; cin >> t;
    while (t--) do_testcase();
}
