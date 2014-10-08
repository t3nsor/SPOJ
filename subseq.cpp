// 2008-07-29
#include <cstdio>
#include <map>
#ifdef _MSC_VER
#define GC getchar
#else
#define GC getchar_unlocked
#endif
using namespace std;
int input()
{
	bool minus=false;
	char c;
	int x=0;
	do
		c=GC();
	while (c==' '||c=='\n');	
	for(;;)
	{
		if (c=='-')
			minus=true;
		else if (c==' '||c=='\n')
			return minus?-x:x;
		else
			x=10*x+c-'0';
		c=GC();
	}
}
int main()
{
	int T,i,j,N,s;
	long long psum,c;
//	freopen("subseq.in","r",stdin);
	T=input();
	for (i=0; i<T; i++)
	{
		N=input();
		map<long long,int> M;
		M[0]=1;
		psum=0;
		c=0;
		for (j=0; j<N; j++)
		{
			s=input();
			psum+=s;
			c+=M[psum-(long long)47];
			M[psum]++;
		}
		printf("%d\n",c);
	}
	return 0;
}
