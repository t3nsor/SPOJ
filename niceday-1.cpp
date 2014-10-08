// 2008-09-01
// niceday-1.cpp uses misof's "staircase" data structure.
// See niceday-2.cpp for another approach.
#include <cstdio>
#include <algorithm>
#include <map>
using namespace std;
#define MAX(a,b) ((a)>(b)?(a):(b))
#ifdef _MSC_VER
#define GC getchar
#else
#define GC getchar_unlocked
#endif
#define SIZE 100000
int input()
{
	int x=0;
	char c;
	for(;;)
	{
		c=GC();
		if (c==' '||c=='\n')
			return x;
		x=10*x+c-'0';
	}
}
static pair<int,pair<int,int> > C[SIZE];
int main()
{
	int i,t,N,j,excellent;
	t=input();
	for (i=0; i<t; i++)
	{
		N=input();
		map<int,int> M;
		for (j=0; j<N; j++)
		{
			C[j].first=input();
			C[j].second.first=input();
			C[j].second.second=input();
		}
		excellent=1;
		sort(C,C+N);
		map<int,int>::iterator I1,I2;
		M.insert(C[0].second);
		int x,y;
		for (j=1; j<N; j++)
		{
			x=C[j].second.first;
			y=C[j].second.second;
			I1=M.lower_bound(x);
			if (I1!=M.begin())
			{
				I1--;
				if (I1->second<y)
					continue;
				I1++;
			}
			I2=I1;
			while (I2!=M.end()&&I2->second>y)
				I2++;
			M.erase(I1,I2);
			M.insert(C[j].second);
			excellent++;
		}
		printf("%d\n",excellent);
	}
	return 0;
}
