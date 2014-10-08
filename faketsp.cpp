// 2009-10-29
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
using namespace std;
int main()
{
	char s[1000],t[1000];
	bool print=false;
	double X,Y,x,y,dist=0;
	for (;;)
	{
		s[0]=0;
		gets(s);
		int l=strlen(s);
		if (!*s) return 0;
		reverse(s,s+l);
		sscanf(s,".)%[^(](",&t);
		l=strlen(t);
		reverse(t,t+l);
		sscanf(t,"%lf ,%lf",&x,&y);
		if (print)
		{
			dist+=hypot(X-x,Y-y);
			printf("The salesman has traveled a total of %.3f kilometers.\n",dist);
		}
		else
			print=true;
		X=x; Y=y;
	}
}
