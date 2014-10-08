// 2008-02-02
#include <stdio.h>
void sort(int a[],int l,int u)
{
	int i,j,k;
	unsigned int x;
	for (i=l+1; i<=u; i++)
	{
		j=i;
		x=a[i];
		while ((j>l)&&(a[j-1]>x)) j--;
		for (k=i; k>j; k--)
			a[k]=a[k-1];
		a[j]=x;
	}
}
int main()
{
	int t,i,j,N,total;
	int M[1000];
	int W[1000];
	scanf("%d",&t);
	for (i=0; i<t; i++)
	{
		scanf("%d",&N);
		for (j=0; j<N; j++)
			scanf("%d",&M[j]);
		for (j=0; j<N; j++)
			scanf("%d",&W[j]);
		sort(M,0,N-1);
		sort(W,0,N-1);
		total=0;
		for (j=0; j<N; j++)
			total+=M[j]*W[j];
		printf("%d\n",total);
	}
	return 0;
}
