// 2008-07-23
#include <cstdio>
#ifdef ONLINE_JUDGE
#define GC getchar_unlocked
#else
#define GC getchar
#endif
int input()
{
	int x;
	scanf("%d",&x);
	return x;
}
int main()
{
	int id[10000];
	int sz[10000];
	int V,i,E,v1,v2;
	V=input();
	for (i=0; i<V; i++)
	{
		id[i]=i;
		sz[i]=1;
	}
	E=input();
	if (E>=V)
	{
		printf("NO\n");
		return 0;
	}
	for (i=0; i<E; i++)
	{
		v1=input()-1;
		v2=input()-1;
		while (v1!=id[v1])
		{
			if (id[v1]!=id[id[v1]])
				sz[id[v1]]-=sz[v1];
			v1=id[v1]=id[id[v1]];
		}
		while (v2!=id[v2])
		{
			if (id[v2]!=id[id[v2]])
				sz[id[v2]]-=sz[v2];
			v1=id[v2]=id[id[v2]];
		}
		if (v1==v2)
		{
			printf("NO\n");
			return 0;
		}
		else
		{
			if (sz[v1]<sz[v2])
			{
				id[v1]=v2;
				sz[v2]+=sz[v1];
			}
			else
			{
				id[v2]=v1;
				sz[v1]+=sz[v2];
			}
		}
	}
	printf("YES\n");
	return 0;
}
