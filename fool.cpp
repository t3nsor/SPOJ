// 2014-05-02
#include <iostream>
#include <string>
using namespace std;
bool set[201][201];
bool element_list[201][201];
bool list[201][201];
bool element[201][201];
int main() {
    int T; cin >> T;
    for (int cs = 1; cs <= T; cs++) {
        string s; cin >> s;
        for (int i = 0; i <= s.length(); i++) {
            element_list[i][i] = true;
        }
        for (int i = 1; i <= s.length(); i++) {
            for (int j = 0; j <= s.length() - i; j++) {
                set[j][i+j] = (i >= 2 && s[j] == '{' && s[j+i-1] == '}' &&
                              element_list[j+1][i+j-1]);
                element[j][i+j] = (i == 1 || set[j][i+j]);
                // is it a list?
                if (element[j][i+j]) {
                    list[j][i+j] = true;
                } else if (i >= 3) {
                    list[j][i+j] = false;
                    // find comma
                    for (int k = 1; k < i-1; k++) {
                        if (s[j+k] == ',' && element[j][j+k] &&
                            list[j+k+1][i+j]) {
                            list[j][i+j] = true;
                            break;
                        }
                    }
                } else {
                    list[j][i+j] = false;
                }
                element_list[j][i+j] = list[j][i+j];
            }
        }
        cout << "Word #" << cs << ": "
             << (set[0][s.length()] ? "Set" : "No Set") << endl;
    }
}
