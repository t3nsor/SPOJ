// 2008-06-15
/*Code by Vaibhavi--------ORIGINAL--------*/
#include<bits/stdc++.h>

using namespace std;

typedef long long int ll;
ll pts[100005][6]={0};

ll mnht(ll pt[100005][6], int kth, int idx, int d)
{
    ll dist=0;

    for(int i=0;i<d;i++)
    {
        dist+=abs(pt[kth][i]-pt[idx][i]);
    }

    return dist;
}
   
ll max_cmb(ll cmbn[6], ll pt[100005][6], int n, int d)
{
    ll pk=0;ll idx=0;

   for(int i=0;i<n;i++)
   {
       ll val=0;

       for(int j=0;j<d;j++)
       {
           val+=(1-2*cmbn[j])*pt[i][j];
       }

       if(val>pk)
       {
           pk=val;
           idx=i;
       }
   }

   return idx;
}

int main()
{
   int n, d;
   cin>>n>>d;

   for(int i=0;i<n;i++)
   {
       for(int j=0;j<d;j++)
        cin>>pts[i][j];
   }

   ll pk[2<<d];

   for(int j=0;j< 1<<(d);j++)
   {
       ll cmbn[6]={0};
       for(int dig=0;dig< d; dig++)
       {
           if(j & 1<<dig)
             cmbn[dig]=1;
          // cout<<cmbn[dig]<<" ";
       }
//cout<<endl;
       pk[j]=max_cmb(cmbn, pts, n, d);
   }

   ll maxd=0;
   for(int i=0;i<n;i++)
   {
       for(int j=0;j< 1<<(d);j++)
       maxd=max(maxd, mnht(pts, i, pk[j], d));
   }

   cout<<maxd<<endl;
   return 0;
}

/*
#include <cstdio>
#ifdef _MSC_VER
#define GC getchar
#else
#define GC getchar_unlocked
#endif
int input()
{
	int x=0;
	char c;
	bool minus=false;
	for(;;)
	{
		c=GC();
		if (c=='-')
		{
			minus=true;
			continue;
		}
		if (c==' '||c=='\n')
			if (minus)
				return -x;
			else
				return x;
		x=10*x+c-'0';
	}
}
void fix()
{
	char c;
	for(;;)
	{
		c=GC();
		if (c==-1||c!=' '&&c!='\n')
			break;
	}
	if (c!=-1)
		ungetc(c,stdin);
}
int main()
{
	int N,i,j,k,d;
	int a[6];
	int m[32];
	int M[32];
	for (i=0; i<32; i++)
	{
		m[i]=1000000000;
		M[i]=-1000000000;
	}
	scanf("%d %d",&N,&d);
	fix();
	for (i=0; i<N; i++)
	{
		for (j=0; j<d; j++)
			a[j]=input();//,fix();
		for (j=0; j<1<<(d-1); j++)
		{
			int x=a[0];
			for (k=0; k<d-1; k++)
				if (j&(1<<k))
					x+=a[k+1];
				else
					x-=a[k+1];
			if (m[j]>x)
				m[j]=x;
			if (M[j]<x)
				M[j]=x;
		}
	}
	int ans=0;
	for (i=0; i<1<<(d-1); i++)
		if (ans<M[i]-m[i])
			ans=M[i]-m[i];
	printf("%d\n",ans);
	return 0;
}
*/
