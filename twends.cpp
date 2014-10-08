// 2010-11-13
// modified 2014-10-07
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
int input()
{
	int x=0;
	char c;
	do
		c=getchar_unlocked();
	while (c<=32);
	do
	{
		x=(x<<1)+(x<<3)+c-'0';
		c=getchar_unlocked();
	}
	while (c>32);
	return x;
}
int a[1000];
bool memo[1000][1000][2];
int answer[1000][1000][2];
int f(int first,int last,int player)
{
	if (first==last)
		return a[first];
	if (memo[first][last][player])
		return answer[first][last][player];
	memo[first][last][player]=true;
	if (player==0) //first player, smart
	{
		answer[first][last][0]=max(a[first]-f(first+1,last,1),a[last]-f(first,last-1,1));
		return answer[first][last][0];
	}
	else
	{
		if (a[first]>=a[last])
			answer[first][last][1]=a[first]-f(first+1,last,0);
		else
			answer[first][last][1]=a[last]-f(first,last-1,0);
		return answer[first][last][1];
	}
}
int main()
{
	int _count=0;
	int N,i,j;
	for(;;)
	{
		_count++;
		N=input();
		if (N==0) return 0;
		for (i=0; i<N; i++)
			a[i]=input();
		for (i=0; i<N; i++)
			for (j=0; j<N; j++)
			{
				memo[i][j][0]=false;
				memo[i][j][1]=false;
			}
		int result=f(0,N-1,0);
		printf("In game %d, the greedy strategy might lose by as many as %d points.\n",_count,f(0,N-1,0));
	}
}
