// 2025-06-29
// You don't need to do anything clever, just make your sieve fast enough. The
// one thing to watch out for is that you need to stop trial division once you
// reach the square root of the remaining number. That is to say, you have to
// optimize for the case where most of the numbers are significantly smaller
// than 10^16 (I hate it when the problem setters bundle a lot of test cases
// together like this). Without this optimization I get TLE.
#include <stdio.h>
#include <string.h>

// Use Yarin's sieve (not my code)
#define MAXSIEVE 100000007 // All prime numbers up to this
#define MAXSIEVEHALF (MAXSIEVE/2)
#define MAXSQRT 5000 // sqrt(MAXSIEVE)/2
char a[MAXSIEVE/16+2];
#define isprime(n) (a[(n)>>4]&(1<<(((n)>>1)&7))) // Works when n is odd

int nprime;
int prime[5800000];

int main() {
    memset(a,255,sizeof(a));
    a[0]=0xFE;
    for(int i=1;i<MAXSQRT;i++)
        if (a[i>>3]&(1<<(i&7)))
            for(int j=i+i+i+1;j<MAXSIEVEHALF;j+=i+i+1)
                a[j>>3]&=~(1<<(j&7));

    nprime = 1;
    prime[0] = 2;
    for (int i = 3; i <= MAXSIEVE; i += 2) {
        if (isprime(i)) {
            prime[nprime++] = i;
        }
    }

    int T; scanf("%d", &T);
    while (T--) {
        long long n; scanf("%lld", &n);
        long long orig_n = n;
        long long result = 1;
        int pidx = 0;
        while (n > 1) {
            while ((long long)prime[pidx] * prime[pidx] <= n &&
                   n % prime[pidx]) ++pidx;
            long long p;
            if ((long long)prime[pidx] * prime[pidx] <= n) p = prime[pidx];
            else p = n;
            long long pwr = 1;
            while (0 == n % p) { n /= p; pwr *= p; }
            pwr += (pwr - 1) / (p - 1);
            result *= pwr;
            ++pidx;
        }
        result -= orig_n;
        printf("%lld\n", result);
    }
}
