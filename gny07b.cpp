// 2008-08-21
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
int main()
{
	int N,i;
	double x;
	char s[100];
	scanf("%d",&N);
	for (i=1; i<=N; i++)
	{
		scanf("%lf",&x);
		getchar();
		gets(s);
		if (s[0]=='k')
			printf("%d %.4f lb\n",i,x*2.2046);
		else if (s[1]=='b')
			printf("%d %.4f kg\n",i,x*0.4536);
		else if (s[0]=='l')
			printf("%d %.4f g\n",i,x*0.2642);
		else
			printf("%d %.4f l\n",i,x*3.7854);
	}
	return 0;
}
