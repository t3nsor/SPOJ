// 2025-06-03
#include <iostream>
using namespace std;
void do_testcase() {
    long long v0; cin >> v0;
    int years; cin >> years;
    int ops; cin >> ops;
    long long result = v0;
    while (ops--) {
        int compound; cin >> compound;
        double ratef; cin >> ratef;
        ratef *= 8192;
        const int raten = ratef + 0.5;  // rate is `raten` divided by 8192
        int fee; cin >> fee;
        long long A = v0;
        if (compound == 0) {
            long long interest = 0;
            for (int i = 0; i < years; i++) {
                // according to the example calculation in the problem
                // description, we round toward 0
                const long long B = (A * raten) / 8192;
                interest += B;
                A -= fee;
            }
            A += interest;
        } else {
            for (int i = 0; i < years; i++) {
                const long long B = (A * raten) / 8192;
                A += B - fee;
            }
        }
        result = max(result, A);
    }
    cout << result << '\n';
}
int main() {
    int T; cin >> T; while (T--) do_testcase();
}
