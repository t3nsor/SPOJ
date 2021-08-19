// 2021-08-18
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
void do_testcase() {
    int K, L; cin >> K >> L;
    string keys, letters; cin >> keys >> letters;
    vector<int> freq(L);
    for (int i = 0; i < L; i++) {
        cin >> freq[i];
    }
    // price[i][j] -> price to assign the first i letters to the first j keys,
    // where key j-1 is complete (no more letters can be added)
    struct dp_entry {
        int price = 0;
        int i_prev = 0;
    };
    vector<vector<dp_entry>> dp(L + 1, vector<dp_entry>(K + 1));
    for (int i = 1; i <= L; i++) {
        dp[i][0].price = 1e9;
        for (int j = 1; j <= K; j++) {
            dp[i][j] = {dp[i][j-1].price, i};
            int freq_sum = 0;
            int key_cost = 0;
            for (int m = 1; m <= i; m++) {
                // Try assigning the last m letters to key j-1
                freq_sum += freq[i-m];
                key_cost += freq_sum;
                const auto p = dp[i-m][j-1].price + key_cost;
                if (p <= dp[i][j].price) {
                    dp[i][j].price = p;
                    dp[i][j].i_prev = i - m;
                }
            }
        }
    }
    vector<int> letters_per_key(K);
    int i = L;
    for (int j = K; j > 0; j--) {
        letters_per_key[j-1] = i - dp[i][j].i_prev;
        i = dp[i][j].i_prev;
    }
    int j = 0;
    for (int i = 0; i < K; i++) {
        cout << keys[i] << ": ";
        while (letters_per_key[i]--) {
            cout << letters[j];
            ++j;
        }
        cout << '\n';
    }
    cout << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    for (int i = 1; i <= T; i++) {
        cout << "Keypad #" << i << ":\n";
        do_testcase();
    }
}
