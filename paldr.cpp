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
	static int V[2222222];
	static char s[1111111];
	int T;
	scanf("%d",&T); while (getchar()!='\n');
	while (T--)
	{
		int N;
		N=strlen(gets(s));
		if (s[N-1]=='\r') N--;
		fastLongestPalindromes(s,s+N,V);
		int i=0; //starting from here
		while (i<N)
		{
			int j=i+1;
			while (j<N&&V[2*j]<2*(j-i)) j++;
			if (j==N) break;
			else i=2*j-i;
		}
		if (i==N) puts("YES"); else puts("NO");
	}
	return 0;
}

