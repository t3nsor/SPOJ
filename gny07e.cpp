// 2008-08-23
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
using namespace std;
int a[100];
char side[30];
int pancake[30];
int steps;
int N;
void flip(int x)
{
	int i;
	for (i=0; i<x; i++)
		if (side[i]=='+')
			side[i]='-';
		else
			side[i]='+';
	reverse(pancake,pancake+x);
	reverse(side,side+x);
}
void go(int x)
{
	int i;
	for (i=0; i<N; i++)
		if (pancake[i]==x)
			break;
	a[steps++]=i+1;
	flip(i+1);
	if (side[0]=='+')
	{
		a[steps++]=1;
		side[0]='-';
	}
	a[steps++]=x;
	flip(x);
}
int main()
{
	int t,i,j;
	scanf("%d",&t);
	for (i=1; i<=t; i++)
	{
		scanf("%d",&N);
		for (j=0; j<N; j++)
		{
			getchar();
			side[j]=getchar();
			scanf("%d",pancake+j);
		}
		steps=0;
		for (j=N; j>1; j--)
			go(j);
		if (side[0]=='-')
			a[steps++]=1;
		printf("%d ",i);
		printf("%d ",steps);
		for (j=0; j<steps-1; j++)
			printf("%d ",a[j]);
		if (steps)
			printf("%d\n",a[steps-1]);
	}
	return 0;
}
