// 2025-08-08
// The basic insight is that if K is the number of distinct sets that have ever
// been created (for a given test case) then each instruction can increase K by
// at most 1.  It follows that each set that is on the stack has cardinality
// less than the number of instructions that have been executed so far.  Since
// each test case has at most 2000 instructions, it's fine for each operation to
// take O(K) time or even O(K log K).  At that point it's just an implementation
// problem.  Each set, when created, is assigned a unique ID starting from 0 if
// it hasn't been seen before.  A set is represented as an ordered list of the
// IDs of its elements.  We use a hash map to find the ID of a set given the set
// itself.
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
struct Set {
    set<unsigned short> elements;
    friend bool operator==(const Set& s1, const Set& s2) {
        return s1.elements == s2.elements;
    }
};
namespace std {
template <> struct hash<Set> {
    size_t operator()(const Set& s) const {
        string str;
        for (unsigned short e : s.elements) {
            str.push_back(e >> 16);
            str.push_back(e & 0xff);
        }
        return hash<string>{}(str);
    }
};
}
void do_testcase() {
    int N; cin >> N;
    unordered_map<Set, int> M;
    vector<const Set*> sets;
    stack<int> stk;
    auto get_index = [&](Set&& s) {
        auto it = M.find(s);
        if (it == M.end()) {
            const int size = M.size();
            const auto p = M.insert(make_pair(move(s), size));
            sets.push_back(&(p.first->first));
            return size;
        } else {
            return it->second;
        }
    };
    while (N--) {
        string inst; cin >> inst;
        if (inst == "PUSH") {
            stk.push(get_index(Set()));
            cout << '0';
        } else if (inst == "DUP") {
            const int top = stk.top();
            stk.push(stk.top());
            cout << sets[top]->elements.size();
        } else if (inst == "UNION" || inst == "INTERSECT") {
            const int s1 = stk.top(); stk.pop();
            const int s2 = stk.top(); stk.pop();
            const Set& S1 = *sets[s1];
            const Set& S2 = *sets[s2];
            Set S;
            if (inst == "UNION") {
                set_union(S1.elements.begin(), S1.elements.end(),
                          S2.elements.begin(), S2.elements.end(),
                          inserter(S.elements, S.elements.end()));
            } else {
                set_intersection(S1.elements.begin(), S1.elements.end(),
                                 S2.elements.begin(), S2.elements.end(),
                                 inserter(S.elements, S.elements.end()));
            }
            cout << S.elements.size();
            stk.push(get_index(move(S)));
        } else {
            // ADD
            const int a = stk.top(); stk.pop();
            const int b = stk.top(); stk.pop();
            Set B = *sets[b];
            B.elements.insert(a);
            cout << B.elements.size();
            stk.push(get_index(move(B)));
        }
        cout << '\n';
    }
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) {
        do_testcase();
        cout << "***\n";
    }
}
