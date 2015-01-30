// 2014-10-01
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;
struct bignum {
    typedef unsigned int uint;
    
    vector<uint> digits;
    static const uint RADIX = 1000000000;
    
    bignum(): digits(1, 0) {}
    
    bignum(const bignum& x): digits(x.digits) {}
    
    bignum(const char* x) {
        *this = x;
    }
    
    bignum& operator=(const char* s) {
        int slen=strlen(s),i,l;
        digits.resize((slen+8)/9);
        for (l=0; slen>0; l++,slen-=9) {
            digits[l]=0;
            for (i=slen>9?slen-9:0; i<slen; i++) {
                digits[l]=10*digits[l]+s[i]-'0';
            }
        }
        while (digits.size() > 1 && !digits.back()) digits.pop_back();
        return *this;
    }
    
    void mult(const bignum& x) {
        vector<unsigned long long> res(digits.size() + x.digits.size());
        for (int i=0; i<digits.size(); i++) {
            for (int j=0; j<x.digits.size(); j++) {
                unsigned long long p = digits[i]; p *= x.digits[j];
                res[i+j] += p;
                int k = i+j;
                while (res[k] > 17e18) {
                    unsigned long long quot = res[k]/RADIX;
                    res[k] -= RADIX*quot;
                    res[++k] += quot;
                }
            }
        }
        digits.clear(); digits.resize(res.size());
        unsigned long long carry = 0;
        for (int i = 0; i < digits.size(); i++) {
            unsigned long long new_carry = carry/RADIX;
            digits[i] += carry - RADIX*new_carry;
            unsigned long long quot = res[i]/RADIX;
            digits[i] += res[i] - RADIX*quot;
            while (digits[i] >= RADIX) {
                digits[i] -= RADIX;
                new_carry++;
            }
            carry = new_carry + quot;
        }
        while (digits.back() == 0 && digits.size() > 1) digits.pop_back();
    }

    string to_string() const {
        ostringstream oss;
        oss << digits.back();
        for (int i = digits.size() - 2; i >= 0; i--) {
            oss << setfill('0') << setw(9) << digits[i];
        }
        return oss.str();
    }
};

char num1[11111], num2[11111];

void get_num(char* s) {
    char c;
    do {
        c = getchar();
    } while (c <= 32);
    do {
        *s++ = c;
        c = getchar();
    } while (c > 32);
    *s = '\0';
}

int main() {
    int n; scanf("%d", &n);
    while (n--) {
        get_num(num1); get_num(num2);
        bignum l1(num1), l2(num2);
        l1.mult(l2);
//        string res = l1.to_string();
//        puts(res.c_str());
    }
}
