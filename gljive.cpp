// 2014-04-25
#include <iostream>
#include <cstdlib>
using namespace std;
int main() {
    int sum = 0;
    for (int i = 0; i < 10; i++) {
        int x;
        cin >> x;
        if (sum + x <= 100) sum += x;
        else if (abs(100 - sum) > abs(100 - (sum + x))) sum += x;
        else if (sum < 100 && sum + x - 100 == 100 - sum) sum += x;
        else {
            cout << sum << endl;
            return 0;
        }
    }
    cout << sum << endl;
}
