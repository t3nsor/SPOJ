// 2024-08-12
// The obvious approach is, every time we need to put a car on the floor and the
// floor is already full, evict the car that will be used the furthest in the
// future. So my first solution attempt used `std::set` to represent the cars on
// the floor, where the cars are ordered by the next time in the future they're
// needed. Unfortunately, this gave TLE, so I started wondering whether there's
// a linear time solution. After not being able to find one, I looked at the
// official solutions, but it turns out that you really just have to make sure
// your constant factor isn't too big.
//
// This version of the code uses a segment tree. An additional optimization to
// avoid a vector-of-vectors storing the list of occurrences of each car in the
// sequence is to have a single vector that has all the occurrences of car 0
// followed by all the occurrences of car 1 and so on, with an additional vector
// that gives the current index into the occurrences vector for each car.
#include <algorithm>
#include <stdio.h>
#include <vector>
using namespace std;
int tidx[262144];
int tmax[262144];
void do_testcase() {
    int n, k, p; scanf("%d %d %d", &n, &k, &p);
    int N = 1, e = 0; while (N < n) { e++; N <<= 1; }
    fill(tidx, tidx + 2*N, -1);
    fill(tmax, tmax + 2*N, -1);
    vector<int> seq(p);
    vector<int> freq(n);
    for (int i = 0; i < p; i++) {
        scanf("%d", &seq[i]);
        --seq[i];
        ++freq[seq[i]];
    }
    vector<int> cfreq(n + 1);
    for (int i = 0; i < n; i++) {
        cfreq[i + 1] = cfreq[i] + freq[i];
    }
    vector<int> occidx = cfreq;
    vector<int> occ(p);
    for (int i = 0; i < p; i++) {
        occ[occidx[seq[i]]++] = i;
    }
    occidx = cfreq;
    int floorcnt = 0;
    int result = 0;
    for (int i = 0; i < p; i++) {
        const int x = seq[i];
        occidx[x]++;
        if (tidx[N + x] < 0 && floorcnt == k) {
            // remove a car
            int j = N + tidx[1];
            tidx[j] = tmax[j] = -1;
            for (int k = 0; k < e; k++) {
                const int nextj = j / 2;
                if (tmax[2*nextj] >= tmax[2*nextj + 1]) {
                    tidx[nextj] = tidx[2*nextj];
                    tmax[nextj] = tmax[2*nextj];
                } else {
                    tidx[nextj] = tidx[2*nextj + 1];
                    tmax[nextj] = tmax[2*nextj + 1];
                }
                j = nextj;
            }
            --floorcnt;
        }
        if (tidx[N + x] < 0) {
            ++result;
            ++floorcnt;
        }
        int j = N + x;
        tidx[j] = x;
        if (occidx[x] == cfreq[x + 1]) {
            tmax[j] = 1e6;
        } else {
            tmax[j] = occ[occidx[x]];
        }
        for (int k = 0; k < e; k++) {
            const int nextj = j / 2;
            if (tmax[2*nextj] >= tmax[2*nextj + 1]) {
                tidx[nextj] = tidx[2*nextj];
                tmax[nextj] = tmax[2*nextj];
            } else {
                tidx[nextj] = tidx[2*nextj + 1];
                tmax[nextj] = tmax[2*nextj + 1];
            }
            j = nextj;
        }
    }
    printf("%d\n", result);
}
int main() {
    int T; scanf("%d", &T);
    while (T--) do_testcase();
}
