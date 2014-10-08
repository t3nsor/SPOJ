// 2014-05-27
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cmath>
using namespace std;
int main() {
    long long R, B; cin >> R >> B;
    long long prod = R + B;
    long long sum = (prod - B)/2 + 2;
    long long diff = sqrt(sum*sum - 4*prod);
    long long x = (sum+diff)/2;
    long long y = abs(sum-diff)/2;
    cout << max(x, y) << ' ' << min(x, y) << endl;
}
