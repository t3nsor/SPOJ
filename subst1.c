// 2010-11-13
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_STRING_LENGTH 100003
// The author is profoundly indebted to Juha Karkkainen and Peter Sanders
#define GetI() (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2)

inline int leq2(int a1, int a2, int b1, int b2) {
	return a1 < b1 || a1 == b1 && a2 <= b2;
}

inline int leq3(int a1, int a2, int a3, int b1, int b2, int b3) {
	return a1 < b1 || a1 == b1 && leq2(a2,a3, b2,b3);
}

static void radix_pass(int* a, int* b, int* r, int n, int K) {
	static int c[MAX_STRING_LENGTH];
	int i, sum, t;
	for (i = 0;  i <= K;  i++) c[i] = 0;
	for (i = 0;  i < n;  i++) c[r[a[i]]]++;
	for (i = 0, sum = 0;  i <= K;  i++) {
		t = c[i];
		c[i] = sum;
		sum += t;
	}
	for (i = 0;  i < n;  i++) b[c[r[a[i]]]++] = a[i];
}
 
void suffix_array(int* s, int* SA, int n, int K) {
	s[n]=s[n+1]=s[n+2]=SA[n]=SA[n+1]=SA[n+2]=0;
	int n0 = (n+2)/3, n1 = (n+1)/3, n2 = n/3, n02 = n0+n2;
	int s12[n02 + 3];
	int SA12[n02 + 3];
	static int s0[(MAX_STRING_LENGTH+2)/3];
	static int SA0[(MAX_STRING_LENGTH+2)/3];
	int i, j, name, c0, c1, c2, p, t, k;
	s12[n02] = s12[n02+1] = s12[n02+2] = 0;
	SA12[n02] = SA12[n02+1] = SA12[n02+2] = 0;
	for (i=0, j=0; i < n+(n0-n1); i++) if (i%3 != 0) s12[j++] = i;
	radix_pass(s12, SA12, s+2, n02, K);
	radix_pass(SA12, s12, s+1, n02, K);
	radix_pass(s12, SA12, s, n02, K);
	name = 0, c0 = -1, c1 = -1, c2 = -1;
	for (i = 0; i < n02; i++) {
		if (s[SA12[i]] != c0 || s[SA12[i]+1] != c1 || s[SA12[i]+2] != c2) {
			name++;
			c0 = s[SA12[i]];
			c1 = s[SA12[i]+1];
			c2 = s[SA12[i]+2];
		}
		if (SA12[i] % 3 == 1) s12[SA12[i]/3] = name;
		else s12[SA12[i]/3 + n0] = name;
	}
	if (name < n02) {
		suffix_array(s12, SA12, n02, name);
		for (i = 0; i < n02; i++) s12[SA12[i]] = i + 1;
	}
	else for (i = 0; i < n02; i++) SA12[s12[i] - 1] = i;
	for (i=0, j=0; i < n02; i++) if (SA12[i] < n0) s0[j++] = 3*SA12[i];
	radix_pass(s0, SA0, s, n0, K);
	for (p=0, t=n0-n1, k=0; k < n; k++) {
		i = GetI();
		j = SA0[p];
		if (SA12[t] < n0 ?
				leq2(s[i], s12[SA12[t] + n0], s[j], s12[j/3]) :
				leq3(s[i], s[i+1], s12[SA12[t]-n0+1], s[j], s[j+1], s12[j/3+n0])) {
			SA[k] = i; t++;
			if (t == n02) for (k++; p < n0; p++, k++) SA[k] = SA0[p];
		}
		else {
			SA[k] = j; p++;
			if (p == n0) for (k++; t < n02; t++, k++) SA[k] = GetI();
		}
	}
}
int main()
{
	int T;
	scanf("%d\n",&T);
	char input[100000];
	while (T--)
	{
		gets(input);
		int N=strlen(input);
		int i,j,k;
		int V[N+3];
		int sa[N+3]; 
		for (i=0; i<N; ++i)
			V[i]=input[i];
		suffix_array(V,sa,N,256);
		int rank[N+1];
		for (i=0; i<N; ++i)
			rank[sa[i]]=i;
		long long res=N;
		res*=N+1;
		res/=2;
		k=0;
		for (i=0; i<N; ++i)
		{
			if (k>0) --k;
			if (rank[i]==N-1)
			{
				k=0;
				continue;
			}
			j=sa[rank[i]+1];
			while (i+k<N&&j+k<N&&V[i+k]==V[j+k]) ++k;
			res-=k;
		}
//		for (i=0; i<N; i++)
//			printf("%s\n",input+sa[i+1]);
		printf("%lld\n",res);
	}
	return 0;
}

