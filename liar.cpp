// 2024-08-15
// Consider the possible scenarios. If at least one student is a truth-teller,
// then we know the entire scenario (each student is what that student says) and
// we can check whether it's consistent. So we can consider all scenarios with
// at least one truth-teller by considering each student in turn as a potential
// truth-teller. If all students are liars, this scenario is always possible
// unless at least one student says all students are liars (in which case it
// contradicts the hypothesis that that student is a liar). So it is easy to
// check all scenarios.
//
// The hardest part of this problem is that the problem description is wrong. In
// the judge output, there's no space in "at least" and "at most". In other
// words, the program has to output "atleast" and "atmost" to get AC.
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    for (int _cs = 1; _cs <= T; _cs++) {
        int N; cin >> N;
        vector<string> matrix(N);
        vector<int> poss;
        for (int i = 0; i < N; i++) {
            cin >> matrix[i];
        }
        for (int i = 0; i < N; i++) {
            // check whether student 'i' can be a truth-teller
            bool fail = false;
            for (int j = 0; j < N; j++) {
                if ((matrix[i][j] == 'T') ^ (matrix[i] == matrix[j])) {
                    fail = true;
                    break;
                }
            }
            if (!fail) {
                poss.push_back(count(matrix[i].begin(), matrix[i].end(), 'L'));
            }
        }
        // check whether all students can be liars
        bool fail = false;
        for (int i = 0; i < N; i++) {
            if (matrix[i].find('T') == string::npos) {
                fail = true;
                break;
            }
        }
        if (!fail) poss.push_back(N);
        sort(poss.begin(), poss.end());
        cout << "Class Room#" << _cs << " ";
        if (poss.empty()) {
            cout << "is paradoxical";
        } else {
            cout << "contains atleast "
                 << poss.front()
                 << " and atmost "
                 << poss.back()
                 << " liars";
        }
        cout << '\n';
    }
}
