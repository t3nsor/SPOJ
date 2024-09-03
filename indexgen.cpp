// 2024-09-02
// Entries are sorted case-insensitively and we assume that each primary or
// secondary key only appears with one capitalization in the input.
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;
struct secondary {
    string name;
    vector<int> pages;
};
struct primary {
    string name;
    vector<int> pages;
    map<string, secondary> sec;
};
void add_page(vector<int>& pages, int page) {
    if (pages.empty() || pages.back() != page) pages.push_back(page);
}
void trim(string& s) {
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '\n') s[i] = ' ';
    }
    int i = 0;
    while (i < s.size() && s[i] == ' ') ++i;
    s = s.substr(i);
    while (!s.empty() && s.back() == ' ') s.pop_back();
}
string get_key(string s) {
    string result;
    for (const char c : s) {
        result.push_back(tolower(c));
    }
    return result;
}
void do_testcase(const string& input) {
    map<string, primary> index;
    int i = 0;
    int page = 1;
    while (i < input.size()) {
        if (input[i] == '&') {
            ++page;
            ++i;
            continue;
        }
        if (input[i] != '{') {
            ++i;
            continue;
        }
        int j = i + 1;
        while (input[j] != '}') ++j;
        const string marker = input.substr(i, j - i + 1);
        const int ppos = marker.find('%');
        const int dpos = marker.find('$');
        string text;
        string prim;
        string sec;
        if (ppos != string::npos) {
            text = marker.substr(1, ppos - 1);
            if (dpos != string::npos) {
                prim = marker.substr(ppos + 1, dpos - ppos - 1);
                sec = marker.substr(dpos + 1, marker.size() - dpos - 2);
            } else {
                prim = marker.substr(ppos + 1, marker.size() - ppos - 2);
            }
        } else {
            if (dpos != string::npos) {
                text = marker.substr(1, dpos - 1);
                sec = marker.substr(dpos + 1, marker.size() - dpos - 2);
            } else {
                text = marker.substr(1, marker.size() - 2);
            }
        }
        trim(text);
        trim(prim);
        trim(sec);
        if (prim.empty()) prim = text;
        auto& prim_entry = index[get_key(prim)];
        prim_entry.name = prim;
        if (!sec.empty()) {
            auto& sec_entry = prim_entry.sec[get_key(sec)];
            sec_entry.name = sec;
            add_page(sec_entry.pages, page);
        } else {
            add_page(prim_entry.pages, page);
        }
        i = j + 1;
    }
    for (const auto& kv1 : index) {
        const auto& prim = kv1.second;
        cout << prim.name;
        for (const int page : prim.pages) {
            cout << ", " << page;
        }
        cout << '\n';
        for (const auto& kv2 : prim.sec) {
            const auto& sec = kv2.second;
            cout << "+ " << sec.name;
            for (const int page : sec.pages) {
                cout << ", " << page;
            }
            cout << '\n';
        }
    }
}
int main() {
    int tc = 1;
    for (;;) {
        vector<string> lines;
        string line;
        getline(cin, line);
        int total_len = 0;
        if (line == "**") return 0;
        do {
            total_len += line.length();
            lines.push_back(line);
            getline(cin, line);
        } while (line != "*");
        printf("DOCUMENT %d\n", tc++);
        // concatenate all lines
        string input(total_len, 0);
        int pos = 0;
        for (const auto& line : lines) {
            for (int i = 0; i < line.size(); i++) {
                input[pos++] = line[i];
            }
        }
        do_testcase(input);
    }
}
