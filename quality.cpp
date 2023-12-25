// 2010-11-15
#include <iostream>
#include <cstdio>
using namespace std;
int main()
{
	int t1,t2,t3,p1,p2,p3,cnt=0;
	while (scanf("%d %d %d %d %d %d",&t1,&t2,&t3,&p1,&p2,&p3)==6)
	{
		int solved=(!!t1)+(!!t2)+(!!t3);
		int penalty=
			(t1?t1+1200*(p1-1):0)+
			(t2?t2+1200*(p2-1):0)+
			(t3?t3+1200*(p3-1):0);
		printf("team %d: %d, %d\n",++cnt,solved,penalty);
	}
	return 0;
}

