// This is a version of the "max subarray sum" problem, but I've changed the
// algorithm slightly since I thought it was easier to do it this way than to
// figure out how to incorporate the tiebreaker rules into the usual algorithm.
// To get the best subarray sum ending at each index, we need to know the
// prefix sum at the current index and the smallest prefix sum previously seen.
// Since we want to break ties by subarray length, it follows that in the case
// of a tie between prefix sums, we keep the earlier one.
#include <stdio.h>
using namespace std;
void do_testcase(int testcase_num) {
    int n; scanf("%d", &n);
    int bestval = -1;
    int bestlen;
    int bestend;
    int leastpsum = 0;
    int leastpsumidx = -1;
    int psum = 0;
    for (int i = 0; i < n - 1; i++) {
        int val; scanf("%d", &val);
        psum += val;
        if (psum - leastpsum > bestval ||
            (psum - leastpsum == bestval && i - leastpsumidx > bestlen)) {
            bestval = psum - leastpsum;
            bestlen = i - leastpsumidx;
            bestend = i;
        }
        if (psum < leastpsum) {
            leastpsum = psum;
            leastpsumidx = i;
        }
    }
    if (bestval == -1) {
        printf("Route %d has no nice parts\n", testcase_num);
    } else {
        printf("The nicest part of route %d is between stops %d and %d\n",
               testcase_num, bestend - bestlen + 2, bestend + 2);
    }
}
int main() {
    int b; scanf("%d", &b);
    for (int i = 1; i <= b; i++) {
        do_testcase(i);
    }
}
