// 2010-11-15
// Thanks to Frederick Akalin
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
template <class RAI1,class RAI2>
void fastLongestPalindromes(RAI1 seq,RAI1 seqEnd,RAI2 out)
{
	int seqLen=seqEnd-seq;
	int i=0,j,d,s,e,lLen,k=0;
	int palLen=0;
	while (i<seqLen)
	{
		if (i>palLen && seq[i-palLen-1]==seq[i])
		{
			palLen+=2;
			i++;
			continue;
		}
		out[k++]=palLen;
		s=k-2;
		e=s-palLen;
		bool b=true;
		for (j=s; j>e; j--)
		{
			d=j-e-1;
			if (out[j]==d)
			{
				palLen=d;
				b=false;
				break;
			}
			out[k++]=min(d,out[j]);
		}
		if (b)
		{
			palLen=1;
			i++;
		}
	}
	out[k++]=palLen;
	lLen=k;
	s=lLen-2;
	e=s-(2*seqLen+1-lLen);
	for (i=s; i>e; i--)
	{
		d=i-e-1;
		out[k++]=min(d,out[i]);
	}
}

int main()
{
	int V[66666];
	int N,i,k;
	scanf("%d\n",&k);
	char s[33333]; N=strlen(gets(s));
	fastLongestPalindromes(s,s+N,V);
	int cnt=0;
	for (i=0; i<=2*N; i++)
		if (V[i]>=k&&!((V[i]-k)&1)) cnt++;
	printf("%d\n",cnt);
	return 0;
}

