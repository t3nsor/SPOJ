// 2009-07-09
#include <cstring>
#include <iostream>
using namespace std;
int pos[25];
int memo[7776];
int n[6];
int rec(int x)
{
	int i,z;
	for (i=0; i<5; i++)
		if (n[i+1]>n[i]) return 0;
	z=n[1]+6*n[2]+36*n[3]+216*n[4]+1296*n[5];
	if (~memo[z])
		return memo[z];
	memo[z]=0;
	if (pos[x]==-1)
		for (i=1; i<=5; i++)
		{
			n[i]++;
			memo[z]+=rec(x+1);
			n[i]--;
		}
	else
	{
		int row=pos[x]/5;
		int col=pos[x]%5;
		if (n[row+1]<n[row]&&col==n[row+1])
		{
			n[row+1]++;
			memo[z]+=rec(x+1);
			n[row+1]--;
		}
	}
	return memo[z];
}
int calc() //based on pos[]
{
	memset(n,0,sizeof(n));
	n[0]=5;
	memset(memo,-1,sizeof(memo));
	memo[7775]=1;
	return rec(0);
}
void num_to_grid(int x,char* grid)
{
	int i,j,z;
	x--;
	int cur=0;
	for (i=0; i<25; i++)
	{
		j='A';
		while (cur<=x)
		{
			if (!~pos[j-'A'])
			{
				pos[j-'A']=i;
				cur+=(z=calc());
				pos[j-'A']=-1;
			}
			j++;
		}
		j--;
		pos[j-'A']=i;
		grid[i]=j;
		cur-=z;
	}
	grid[25]=0;
}
int grid_to_num(const char* grid)
{
	int res=0;
	int i,j;
	for (i=0; i<25; i++)
	{
		for (j='A'; j<grid[i]; j++)
			if (!~pos[j-'A'])
			{
				pos[j-'A']=i;
				res+=calc();
				pos[j-'A']=-1;
			}
		pos[grid[i]-'A']=i;
	}
	return res+1;
}
int main()
{
	char c;
	int x;
	char grid[100];
	memset(pos,-1,sizeof(pos));
	scanf("%d\n",&x);
	num_to_grid(x,grid);
	puts(grid);
	memset(pos,-1,sizeof(pos));
	scanf("%s",grid);
	printf("%d\n",grid_to_num(grid));
	return 0;
}
