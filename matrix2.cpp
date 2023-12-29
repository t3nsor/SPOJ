// 2023-12-29
#include <deque>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;
void do_testcase() {
    int Ro, Co, Rm, Cm, Ri, Ci;
    scanf("%d %d %d %d %d %d", &Ro, &Co, &Rm, &Cm, &Ri, &Ci);
    vector<vector<int>> M(Ro, vector<int>(Co));
    for (int i = 0; i < Ro; i++) {
        scanf("%d", &M[i][0]);
        for (int j = 1; j < Co; j++) {
            M[i][j] = (M[i][j - 1] * 71 + 17) % 100 + 1;
        }
    }
    // Step 1: compute prefix submatrix sums.
    vector<vector<int>> sum(Ro + 1, vector<int>(Co + 1, 0));
    for (int i = 0; i < Ro; i++) {
        for (int j = 0; j < Co; j++) {
            sum[i + 1][j + 1] = sum[i + 1][j] + sum[i][j + 1]
                              - sum[i][j] + M[i][j];
        }
    }
    // returns M[r1][c1] + ... + M[r2 - 1][c2 - 1]
    auto rectsum = [&sum](int r1, int c1, int r2, int c2) {
        return sum[r2][c2] - sum[r2][c1] - sum[r1][c2] + sum[r1][c1];
    };

    // Rs x Cs is the number of positions at which an Ri x Ci box can be
    // placed within M. `rectsum` allows us to calculate the sum of an Ri x Ci
    // box with upper-left corner at (i, j) where 0 <= i < Rs and 0 <= j < Cs:
    // namely, rectsum(i, j, i + Ri, j + Ci).
    const int Rs = Ro - Ri + 1;
    const int Cs = Co - Ci + 1;

    // Step 2: For each given box of size Rm x Cm, the number of possible
    // positions for the inner box is (Rm - Ri - 1) x (Cm - Ci - 1). Call these
    // dimensions Rp x Cp. So we compute the min and max within each Rp x Cp
    // box of rectsum values.
    const int Rp = Rm - Ri - 1;
    const int Cp = Cm - Ci - 1;
    vector<vector<int>> rmin(Rs, vector<int>(Cs - Cp + 1));
    vector<vector<int>> rmax(Rs, vector<int>(Cs - Cp + 1));
    for (int i = 0; i < Rs; i++) {
        vector<int> row(Cs);
        for (int j = 0; j < Cs; j++) {
            row[j] = rectsum(i, j, i + Ri, j + Ci);
        }
        deque<int> Dmin;
        deque<int> Dmax;
        for (int j = 0; j < Cp; j++) {
            while (!Dmin.empty() && row[Dmin.back()] >= row[j]) {
                Dmin.pop_back();
            }
            Dmin.push_back(j);
            while (!Dmax.empty() && row[Dmax.back()] <= row[j]) {
                Dmax.pop_back();
            }
            Dmax.push_back(j);
        }
        rmin[i][0] = row[Dmin.front()];
        rmax[i][0] = row[Dmax.front()];
        for (int j = Cp; j < Cs; j++) {
            while (!Dmin.empty() && row[Dmin.back()] >= row[j]) {
                Dmin.pop_back();
            }
            Dmin.push_back(j);
            if (Dmin.front() == j - Cp) {
                Dmin.pop_front();
            }
            rmin[i][j - Cp + 1] = row[Dmin.front()];
            while (!Dmax.empty() && row[Dmax.back()] <= row[j]) {
                Dmax.pop_back();
            }
            Dmax.push_back(j);
            if (Dmax.front() == j - Cp) {
                Dmax.pop_front();
            }
            rmax[i][j - Cp + 1] = row[Dmax.front()];
        }
    }
    vector<vector<int>> bmin(Rs - Rp + 1, vector<int>(Cs - Cp + 1));
    vector<vector<int>> bmax(Rs - Rp + 1, vector<int>(Cs - Cp + 1));
    for (int j = 0; j < Cs - Cp + 1; j++) {
        vector<int> mincol(Rs);
        vector<int> maxcol(Rs);
        for (int i = 0; i < Rs; i++) {
            mincol[i] = rmin[i][j];
            maxcol[i] = rmax[i][j];
        }
        deque<int> Dmin;
        deque<int> Dmax;
        for (int i = 0; i < Rp; i++) {
            while (!Dmin.empty() && mincol[Dmin.back()] >= mincol[i]) {
                Dmin.pop_back();
            }
            Dmin.push_back(i);
            while (!Dmax.empty() && maxcol[Dmax.back()] <= maxcol[j]) {
                Dmax.pop_back();
            }
            Dmax.push_back(i);
        }
        bmin[0][j] = mincol[Dmin.front()];
        bmax[0][j] = maxcol[Dmax.front()];
        for (int i = Rp; i < Rs; i++) {
            while (!Dmin.empty() && mincol[Dmin.back()] >= mincol[i]) {
                Dmin.pop_back();
            }
            Dmin.push_back(i);
            if (Dmin.front() == i - Rp) {
                Dmin.pop_front();
            }
            bmin[i - Rp + 1][j] = mincol[Dmin.front()];
            while (!Dmax.empty() && maxcol[Dmax.back()] <= maxcol[i]) {
                Dmax.pop_back();
            }
            Dmax.push_back(i);
            if (Dmax.front() == i - Rp) {
                Dmax.pop_front();
            }
            bmax[i - Rp + 1][j] = maxcol[Dmax.front()];
        }
    }
    // Note that bmin[i][j] is the minimum of the box sums between (i, j) and
    // (i + Rp - 1, j + Cp - 1), where the box sum at (i, j) is M[i][j] + ...
    // + M[i + Ri - 1][j + Ci - 1].

    // Step 3: For each box of size Rm x Cm, find its sum and find the min and
    // max box sums that are strictly inside it using the `bmin` and `bmax`
    // arrays previously computed.
    int best = 0;
    for (int i = 0; i + Rm <= Ro; i++) {
        for (int j = 0; j + Cm <= Co; j++) {
            const int sum = rectsum(i, j, i + Rm, j + Cm);
            best = max(best, abs(sum - bmin[i + 1][j + 1]));
            best = max(best, abs(sum - bmax[i + 1][j + 1]));
        }
    }
    printf("%d\n", best);
}
int main() {
    int t; scanf("%d", &t);
    while (t--) do_testcase();
}
