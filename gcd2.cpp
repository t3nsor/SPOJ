// 2008-08-04
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
int gcd(int x,int y)
{
	if (y==0)
		return x;
	return gcd(y,x%y);
}
int main()
{
	int t,A,i,rem;
	char B[300];
	scanf("%d",&t);
	while (t--)
	{
		scanf("%d %s",&A,B);
		if (!A)
		{
			printf("%s\n",B);
			continue;
		}
		rem=0;
		i=0;
		while (B[i])
			rem=(10*rem+B[i++]-'0')%A;
		printf("%d\n",gcd(A,rem));
	}
	return 0;
}
