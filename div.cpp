// 2024-04-15
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;
bool isprime[500000];
bool is_rsa_number(int x) {
    int pf1 = 0;
    for (int i = 2; i <= x; i++) {
        if (x % i == 0 && isprime[i]) {
            if (pf1 == 0) pf1 = i;
            else return x == pf1 * i;
        }
    }
    return false;
}
int main() {
    vector<int> primes;
    memset(isprime, 1, sizeof(isprime));
    for (int i = 2; i < 500000; i++) {
        if (!isprime[i]) continue;
        primes.push_back(i);
        for (int j = 2*i; j < 500000; j += i) {
            isprime[j] = false;
        }
    }
    vector<int> result;
    // find examples with 2 prime factors
    for (int i = 1; i < primes.size(); i++) {
        vector<int> powers1 = {1};
        while (powers1.back() <= 1000000 / primes[i]) {
            powers1.push_back(primes[i] * powers1.back());
        }
        for (int j = 0; j < i; j++) {
            if (primes[j] > 1000000 / primes[i]) break;
            vector<int> powers2 = {1};
            while (powers2.back() <= 1000000 / primes[j]) {
                powers2.push_back(primes[j] * powers2.back());
            }
            for (int k = 1; k < powers1.size(); k++) {
                if (!isprime[k + 1]) continue;
                for (int m = 1; m < powers2.size(); m++) {
                    if (!isprime[m + 1] || k == m) continue;
                    if (powers1[k] > 1000000 / powers2[m]) continue;
                    result.push_back(powers1[k] * powers2[m]);
                }
            }
        }
    }
    // find examples with 1 prime factor
    for (int i = 0; i < primes.size(); i++) {
        if (primes[i] > 1000000 / primes[i]) break;
        int exponent = 1;
        int power = primes[i];
        for (;;) {
            if (is_rsa_number(exponent + 1)) {
                result.push_back(power);
            }
            if (power > 1000000 / primes[i]) break;
            power *= primes[i];
            ++exponent;
        }
    }
    sort(result.begin(), result.end());
    for (int i = 8; i < result.size() && result[i] <= 1000000; i += 9) {
        printf("%d\n", result[i]);
    }
}
