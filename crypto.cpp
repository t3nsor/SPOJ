// 2026-04-13
// Here's how I decoded the text.  First, since "u" is the most frequent letter,
// it means "e", unless the text is highly unusual.  Now, notice that the
// trigraph "ntu" is particularly common; this is almost certainly the word
// "the".  Past this point, some guesswork is required as the ranking of the
// frequencies in the ciphertext may not exactly line up with the overall
// rankings of letter frequencies in English text due to the small size of the
// ciphertext.  From this point onward, I'll use capital letters for cleartext
// (frequencies for lowercase letters refer to their frequencies in the
// ciphertext, while frequencies for uppercase letters refer to their
// frequencies in English text at large).  The third most frequent letter, "f",
// appears as part of the trigrams "ffH" and "EfE", which make it unlikely that
// "f" maps to "A", "O", "I", or "N", the four most frequent unassigned letters,
// so we guess that "f" maps to "S", which is next in line.  The letter "x",
// which is the fifth most common, appears in the context "TTxTHE", which makes
// "O" overwhelmingly more likely than "A", "I", or "N".  The letters "a" and
// "o", which are the two most frequent unassigned ciphertext letters, are
// likely to map to "A" and "I" (the same for the cleartext) in some order; the
// substring "aToTaO" suggests "ITATIO", which looks like a substring of some
// English word, while "ATITAO" seems less likely.  Assigning "y" -> "L" (based
// on both being the next most frequent unassigned letter) yields something that
// looks unproblematic, and the substring "ILLLESSTHAl" strongly suggests that
// it contains the word "LESS" and the following word is "THAN", so "l" -> "N".
// After this point it's pretty easy to guess the remaining letters by examining
// substrings that are mostly deciphered and I'll skip the remaining details.
//
// So it turns out that the problem is asking us to compute the sums of given
// pairs of integers A and B, where "A and B are integers and less than max
// long int" (spacing and punctuation uncertain).  It turns out this is a bit of
// a trick question because there is no lower bound on the values of A and B.
// Because the input file is large, we can't use Python, and even my attempts to
// solve it in Haskell and Julia got TLE.  So C++ it is, then.  Bignum code was
// adapted from m00pair.cpp (and I simplified it too much initially, resulting
// in a large number of WAs and SIGSEGVs).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <utility>
#include <vector>
using namespace std;
typedef unsigned int uint;
struct bignum {
    vector<uint> digits;
    uint length;
    bignum(const string& x, int s) {
        int slen=x.size(),i;
        digits.resize(s);
        for (length=0; slen>0; length++,slen-=9) {
            digits[length]=0;
            for (i=slen>9?slen-9:0; i<slen; i++) {
                digits[length]=10*digits[length]+x[i]-'0';
            }
        }
    }
    void add(const bignum& x) {
        int l=max(length,x.length);
        int d;
        int carry=0,sum;
        for (d=0; d<=l; d++) {
            sum=carry;
            if (d<length) sum+=digits[d];
            if (d<x.length) sum+=x.digits[d];
            digits[d]=sum;
            if (digits[d]>=1000000000) {
                digits[d]-=1000000000;
                carry=1;
            } else { carry=0; }
        }
        length=l;
        if (digits[length]) length++;
    }
    // undefined behavior if result is negative
    void sub(const bignum& x) {
        int d;
        int borrow=0;
        for (d=0; d<=length; d++) {
            digits[d]-=borrow;
            if (d<x.length) digits[d]-=x.digits[d];
            if (digits[d]>>31) {digits[d]+=1000000000;borrow=1;} else borrow=0;
        }
        while (length>0&&!digits[length-1]) length--;
        if (length==0) length=1;
    }
    void print() {
        int i;
        for (i=length-1; i+1&&!digits[i]; i--);
        if (i<0) i=0;
        printf("%u",digits[i--]);
        while (i+1) printf("%.9u",digits[i--]);
    }
    bool operator<(const bignum& y) {
        int i;
        if (length<y.length) return true;
        if (length>y.length) return false;
        for (i=length-1; i>=0; i--) {
            if (digits[i]<y.digits[i]) {
                return true;
            } else if (digits[i]>y.digits[i]) {
                return false;
            }
        }
        return false;
    }
};

pair<bool, string> get() {
    pair<bool, string> result;
    result.first = false;
    int c;
    do {
        c = getchar_unlocked();
        if (c == EOF) exit(0);
    } while (c <= 32);
    do {
        if (c == '-') result.first = true;
        // else if (c == '+') throw;
        else result.second.push_back(c);
        c = getchar_unlocked();
    } while (c > 32);
    // check for negative zeroes
    // bool iszero = true;
    // for (const char c : result.second) { if (c != '0') iszero = false; }
    // if (iszero && result.first) throw;
    return result;
}

int main() {
    for (;;) {
        auto a = get();
        auto b = get();
        int size = (max(a.second.size(), b.second.size()) + 1 + 8)/9;
        bignum A(a.second, size);
        bignum B(b.second, size);
        if (a.first == b.first) {
            A.add(B);
            if (a.first) putchar('-');
            A.print();
        } else {
            if (a.first) swap(A, B);
            // a is positive, b is negative
            if (A < B) {
                B.sub(A);
                putchar('-');
                B.print();
            } else {
                A.sub(B);
                A.print();
            }
        }
        putchar('\n');
    }
}
