// 2008-06-22
#include <cstdio>
#ifdef _MSC_VER
#define PC putchar
#else
#define PC putchar_unlocked
#endif
static int adjlist[100000][10];
static int list[1100000];
int listsize;
void find_tour(int v)
{
	bool b=false;
	int i,j;
	for(;;)
	{
		i=0;
		bool found=false;
		while (i<10&&!found)
		{
			j=adjlist[v][i];
			if (j>=0)
				found=true;
			i++;
		}
		if (!found) break;
		adjlist[v][i-1]=-1;
		find_tour(j);
	}
	list[listsize++]=v%10;
}
int main()
{
	int n,i,j;
	//freopen("code.out","w",stdout);
	for(;;)
	{
		scanf("%d",&n);
		if (n==0)
			return 0;
		if (n==1)
		{
			printf("0123456789\n");
			continue;
		}
		int p10=1;
		for (i=1; i<n; i++)
			p10*=10;
		//use this many nodes, and create adjlists
		for (i=0; i<p10; i++)
			for (j=0; j<10; j++)
				adjlist[i][j]=10*(i%(p10/10))+j;
		listsize=0;
		find_tour(0);
		for (i=0; i<n-2; i++)
			PC('0');
		for (i=listsize-1; i>=0; i--)
			PC(list[i]+'0');
		PC('\n');
	}
	return 0;
}
