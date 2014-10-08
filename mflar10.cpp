// 2014-04-25
#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
using namespace std;
int main() {
    for (;;) {
        string s;
        getline(cin, s);
        if (s == "*") return 0;
        if (s == "") continue;
        istringstream iss(s);
        string word; iss >> word;
        char letter = tolower(word[0]); bool ok = true;
        for (;;) {
            string word; iss >> word;
            if (word == "") break;
            ok &= tolower(word[0]) == letter;
        }
        cout << (ok ? "Y" : "N") << endl;
    }
}
