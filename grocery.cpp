// 2025-05-31
// The idea is to first write a slow solution, which lets us then determine that
// all the outputs fall within certain bounds, so we don't need to check
// anything outside those bounds.
#include <stdio.h>
#define ll long long
int main() {
    ll M=1e6;
#define F(i,s) for(ll i=s;i<2000;i++)
    F(a,1) F(b,a) F(c,b) {
        if (a>125||b>592||c>884) break;
        ll p=M*(a+b+c),q=a*b*c-M;
        if (q&&!(p%q)&&p/q>0) {
            ll d = p/q;
#define D(v) int(v/100),int(v%100)
            if (c<=d&&a+b+c+d<=2000)
                printf("%d.%02d %d.%02d %d.%02d %d.%02d\n",
                       D(a),D(b),D(c),D(d));
        }
    }
}
