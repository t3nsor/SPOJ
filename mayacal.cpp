// 2022-09-14
#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
using namespace std;

// return a % b (positive value)
int mod(int a, int b) {
  return ((a%b)+b)%b;
}

int extended_euclid(int a, int b, int &x, int &y) {
  int xx = y = 0;
  int yy = x = 1;
  while (b) {
    int q = a/b;
    int t = b; b = a%b; a = t;
    t = xx; xx = x-q*xx; x = t;
    t = yy; yy = y-q*yy; y = t;
  }
  return a;
}

// Chinese remainder theorem (special case): find z such that
// z % x = a, z % y = b.  Here, z is unique modulo M = lcm(x,y).
// Return (z,M).  On failure, M = -1.
pair<int, int> chinese_remainder_theorem(int x, int a, int y, int b) {
  int s, t;
  int d = extended_euclid(x, y, s, t);
  if (a%d != b%d) return make_pair(0, -1);
  return make_pair(mod(s*b*x+t*a*y,x*y)/d, x*y/d);
}

vector<string> tzolkin_daynames = {
    "Imix", "Ik", "Akbal", "Kan", "Chikchan", "Kimi", "Manik", "Lamat",
    "Muluk", "Ok", "Chuen", "Eb", "Ben", "Ix", "Men", "Kib", "Kaban",
    "Etznab", "Kawak", "Ajaw"
};
vector<string> haab_monthnames = {
    "Pohp", "Wo", "Sip", "Zotz", "Sek", "Xul", "Yaxkin", "Mol", "Chen", "Yax",
    "Sak", "Keh", "Mak", "Kankin", "Muan", "Pax", "Kayab", "Kumku", "Wayeb"
};
vector<pair<int, string>> generate_tzolkin() {
    int coefficient = 9;
    int day_idx = find(tzolkin_daynames.begin(),
                       tzolkin_daynames.end(),
                       "Ajaw") - tzolkin_daynames.begin();
    vector<pair<int, string>> result;
    for (int i = 0; i < 260; i++) {
        result.emplace_back(coefficient, tzolkin_daynames[day_idx]);
        coefficient++; if (coefficient == 14) coefficient = 1;
        day_idx = (day_idx + 1) % tzolkin_daynames.size();
    }
    return result;
}
int tzolkin_index(int coefficient, string dayname) {
    static const vector<pair<int, string>> combinations = generate_tzolkin();
    const int result =
      find(combinations.begin(),
           combinations.end(),
           make_pair(coefficient, dayname)) - combinations.begin();
    if (result == combinations.size()) {
        // illegal date
        throw domain_error("illegal tzolkin date");
    }
    return result;
}
int haab_index_raw(int day, string monthname) {
    const int month_idx = find(haab_monthnames.begin(),
                               haab_monthnames.end(),
                               monthname) - haab_monthnames.begin();
    if (day < 1 || day > 20) {
        throw domain_error("illegal haab day");
    }
    if (month_idx == haab_monthnames.size()) {
        // unknown month
        throw domain_error("illegal haab month");
    }
    if (haab_monthnames[month_idx] == "Wayeb" && day > 5) {
        throw domain_error("Wayeb only has 5 days");
    }
    return 20 * month_idx + (day - 1);
}
int haab_index(int day, string monthname) {
    static const int offset = haab_index_raw(3, "Sip");
    return mod(haab_index_raw(day, monthname) - offset, 365);
}
const int days_per_baktun = 144000;
string long_count_str(int x) {
    const int baktun = 8 + x / days_per_baktun;
    x %= days_per_baktun;
    const int katun = x / 7200;
    x %= 7200;
    const int tun = x / 360;
    x %= 360;
    const int winal = x / 20;
    const int kin = x % 20;
    ostringstream oss;
    oss << baktun << '.' << katun << '.' << tun << '.' << winal << '.' << kin;
    return oss.str();
}

int main() {
    ios::sync_with_stdio(false);
    int d; cin >> d;
    while (d--) {
        int tzolkin_coefficient, haab_day;
        string tzolkin_dayname, haab_month;
        cin >> tzolkin_coefficient >> tzolkin_dayname
            >> haab_day >> haab_month;
        try {
            auto crt_result = chinese_remainder_theorem(
              260, tzolkin_index(tzolkin_coefficient, tzolkin_dayname),
              365, haab_index(haab_day, haab_month)
            );
            if (crt_result.second < 0) {
                cout << "0\n";
                continue;
            }
            vector<string> result;
            while (crt_result.first < 2 * days_per_baktun) {
                result.push_back(long_count_str(crt_result.first));
                crt_result.first += crt_result.second;
            }
            cout << result.size() << '\n';
            for (const auto& s : result) cout << s << '\n';
        } catch (...) {
            cout << "0\n";
        }
    }
}
