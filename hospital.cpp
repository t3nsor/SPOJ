// 2024-02-24
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
string ftime(int t) {
    char buf[6];
    sprintf(buf, "%2d:%02d", t / 60, t % 60);
    return buf;
}
string fratio(int n, int d) {
    n *= 10000;
    const int r = n % d;
    int q = n / d;
    // The problem statement doesn't specify rounding mode, but 4 down / 5 up is
    // consistent with the sample output and gets accepted. (Round toward 0 is
    // not consistent with the sample output)
    if (2*r >= d) q++;
    char buf[7];
    sprintf(buf, "%3d.%02d", q / 100, q % 100);
    return buf;
}
void do_testcase(int NO) {
    int NR, H, TT, PO, PR, NP; cin >> NR >> H >> TT >> PO >> PR >> NP;
    vector<string> name(NP);
    vector<int> TO(NP), TR(NP);
    for (int i = 0; i < NP; i++) {
        cin >> name[i] >> TO[i] >> TR[i];
    }
    int t = 60 * H;
    int step = 0;
    vector<int> ao(NO, t);  // when operating room becomes available
    vector<int> ar(NR, t);  // when recovery room becomes available
    vector<int> uo(NO, 0);  // utilization of operating room
    vector<int> ur(NR, 0);  // utilization of recovery room
    vector<int> ro(NP, -1);  // room where patient had surgery
    vector<int> to(NP, -1);  // time when patient had surgery
    vector<int> rr(NP, -1);  // room where patient recovered
    int te = t;  // ending time
    while (step < NP) {
        // check for patients that are leaving surgery now
        vector<int> v;
        for (int i = 0; i < NP; i++) {
            if (to[i] >= 0 && to[i] + TO[i] == t) {
                v.push_back(i);
            }
        }
        // sort the patients by their surgery room number
        sort(v.begin(), v.end(),
             [&](int p1, int p2) { return ro[p1] < ro[p2]; });
        // assign patients to recovery rooms
        int r = 0;
        for (const int p : v) {
            while (ar[r] > t) {
                r++;
            }
            rr[p] = r;
            te = max(te, t + TT + TR[p]);
            ar[r] = t + TT + TR[p] + PR;
            ur[r] += TR[p];
            r++;
            step++;
        }
        // check for patients awaiting surgery
        for (int i = 0; i < NP; i++) {
            if (ro[i] >= 0) continue;
            for (int j = 0; j < NO; j++) {
                if (ao[j] != t) continue;
                ro[i] = j;
                to[i] = t;
                uo[j] += TO[i];
                ao[j] = t + TO[i] + PO;
                break;
            }
        }
        t++;
    }
    cout << " Patient          Operating Room          Recovery Room\n";
    cout << " #  Name     Room#  Begin   End      Bed#  Begin    End\n";
    cout << " ------------------------------------------------------\n";
    char buf[1000];
    for (int i = 0; i < NP; i++) {
        sprintf(buf, "%2d  %-8s  %2d   %s   %s     %2d   %s   %s",
                i + 1, name[i].c_str(), ro[i] + 1, ftime(to[i]).c_str(),
                ftime(to[i] + TO[i]).c_str(), rr[i] + 1,
                ftime(to[i] + TO[i] + TT).c_str(),
                ftime(to[i] + TO[i] + TT + TR[i]).c_str());
        cout << buf << '\n';
    }
    cout << "\nFacility Utilization\n";
    cout << "Type  # Minutes  % Used\n";
    cout << "-------------------------\n";
    const int td = te - 60 * H;
    for (int i = 0; i < NO; i++) {
        sprintf(buf, "Room %2d    %4d  %s",
                i + 1, uo[i], fratio(uo[i], td).c_str());
        cout << buf << '\n';
    }
    for (int i = 0; i < NR; i++) {
        sprintf(buf, "Bed  %2d    %4d  %s",
                i + 1, ur[i], fratio(ur[i], td).c_str());
        cout << buf << '\n';
    }
}
int main() {
    ios::sync_with_stdio(false);
    int NO;
    while (cin >> NO) {
        do_testcase(NO);
        cout << '\n';
    }
}
