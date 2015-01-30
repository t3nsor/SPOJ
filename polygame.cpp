// 2008-08-23
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
int main()
{
	int t,V,x,y,z,i,w,n1,n2,n3;
	scanf("%d",&t);
	while (t--)
	{
		scanf("%d",&V);
		scanf("%d %d %d",&x,&y,&z);
		for (i=0; i<V-3; i++)
			scanf("%d %d %d",&w,&w,&w);
		if (x>y)
			swap(x,y);
		if (y>z)
			swap(y,z);
		if (x>y)
			swap(x,y);
		n1=y-x-1;
		n2=z-y-1;
		n3=x-z+V-1;
		if (n1>n2)
			swap(n1,n2);
		if (n2>n3)
			swap(n2,n3);
		if (n1>n2)
			swap(n1,n2);
		if (n1==0&&n2==0)
			printf("YES\n");
		else if ((n1+n2+n3)&1)
			printf("YES\n");
		else
			printf("NO\n");
	}
	return 0;
}
