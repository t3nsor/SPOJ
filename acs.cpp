// 2008-06-20
#include <iostream>
using namespace std;
int main()
{
	int x[5678];
	int X[5678];
	int y[1234];
	int Y[1234];
	int _x,_y,z,a,b,i;
	char c;
	for (i=0; i<1234; i++)
	{
		y[i]=i;
		Y[i]=i; 
	}
	for (i=0; i<5678; i++)
	{
		x[i]=i;
		X[i]=i;
	}
	while (!cin.eof())
	{
		c=getchar();
		if (c=='C')
		{
			scanf("%d %d",&a,&b);
			swap(X[x[a-1]],X[x[b-1]]);
			swap(x[a-1],x[b-1]);
		}
		else if (c=='R')
		{
			scanf("%d %d",&a,&b);
			swap(Y[y[a-1]],Y[y[b-1]]);
			swap(y[a-1],y[b-1]);
		}
		else if (c=='Q')
		{
			scanf("%d %d",&a,&b);
			printf("%d\n",5678*y[a-1]+x[b-1]+1);
		}
		else if (c=='W')
		{
			scanf("%d",&z);
			_x=(z-1)%5678;
			_y=(z-1)/5678;
			printf("%d %d\n",Y[_y]+1,X[_x]+1);
		}
		else if (c!=' '&&c!='\n')
			break;
	}
	return 0;
}
