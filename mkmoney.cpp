// 2008-04-20
#include <iostream>
#include <math.h>
using namespace std;
int main()
{
	int i,j,C;
	double P,P0,I,I0;
	for(j=1;;j++)
	{
		cin >> P >> I >> C;
		P0=P;
		I0=I;
		if (C==0)
			return 0;
		I=I/100.0/C;
		for (i=0; i<C; i++)
			P=int(100.0*P*(1+I)+1e-8)/100.0;
		printf("Case %d. $%.2lf at %.2lf%% APR compounded %d times yields $%.2lf\n",j,P0,I0,C,P);
	}
}
