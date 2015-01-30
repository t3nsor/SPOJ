// 2008-06-10
#include <cstdio>
#ifndef _MSC_VER
#define getchar getchar_unlocked
#endif
int input()
{
	int result=0;
	char c;
	for(;;)
	{
		c=getchar();
		if (c==' '||c=='\n') break;
		result=10*result+(c-'0');
	}
	//scanf("%d",&result);
	return result;
}
int main()
{
	int T,i,j,NG,NM,maxG,maxM,x;
	T=input();
	for (i=0; i<T; i++)
	{
		getchar();
		NG=input();
		NM=input();
		maxG=0;
		maxM=0;
		for (j=0; j<NG; j++)
		{
			x=input();
			if (x>maxG) maxG=x;
		}
		for (j=0; j<NM; j++)
		{
			x=input();
			if (x>maxM) maxM=x;
		}
		if (maxG>=maxM)
			printf("\nGodzilla");
		else
			printf("\nMechaGodzilla");
	}
	return 0;
}
