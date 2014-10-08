// 2010-09-04
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#define CUTOFF 20
using namespace std;
int buf[200000];
int in()
{
    char c;
    do
	c=getchar_unlocked();
    while (c<=32);
    int res=0;
    do
    {
	res=(res<<1)+(res<<3)+c-'0';
	c=getchar_unlocked();
    }
    while (c>32);
    return res;
}
long long mergesort(int* begin,int* end)
{
    long long res=0;
    if (end-begin<=CUTOFF)
    {
	for (int* i=begin; i<end; i++)
	    for (int* j=begin; j<i; j++)
		if (*j>*i)
		    res++;
	sort(begin,end);
	return res;
    }
    else //recursive
    {
	int* m=begin+(end-begin)/2;
	res+=mergesort(begin,m);
	res+=mergesort(m,end);
	int* i=begin;
	int* j=m;
	int* k=buf;
	while (i<m&&j<end)
	    if (*i<*j)
	    {
		*k++=*i++;
		res+=(j-m);
	    }
	    else
		*k++=*j++;
	memcpy(k,i,(m-i)*sizeof(int)); k+=(m-i); res+=(j-m)*(m-i);
	memcpy(k,j,(end-j)*sizeof(int)); 
	memcpy(begin,buf,(end-begin)*sizeof(int));
	return res;
    }
}
int main()
{
    int T,N,i;
    int a[200000];
    T=in();
    while (T--)
    {
	N=in();
	for (i=0; i<N; i++)
	    a[i]=in();
	printf("%lld\n",mergesort(a,a+N));
    }
    return 0;
}
