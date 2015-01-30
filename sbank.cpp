// 2008-01-30
#include <stdio.h>
struct number
{
	int control;
	int bank;
	int owner1;
	int owner2;
	int owner3;
	int owner4;
};
inline bool operator>(const number& n1,const number& n2)
{
	if (n1.control<n2.control)
		return false;
	if (n1.control>n2.control)
		return true;
	if (n1.bank<n2.bank)
		return false;
	if (n1.bank>n2.bank)
		return true;
	if (n1.owner1<n2.owner1)
		return false;
	if (n1.owner1>n2.owner1)
		return true;
	if (n1.owner2<n2.owner2)
		return false;
	if (n1.owner2>n2.owner2)
		return true;
	if (n1.owner3<n2.owner3)
		return false;
	if (n1.owner3>n2.owner3)
		return true;
	if (n1.owner4<n2.owner4)
		return false;
	if (n1.owner4>n2.owner4)
		return true;
	return false;
}
inline bool operator>=(const number& n1,const number& n2)
{
	if (n1.control<n2.control)
		return false;
	if (n1.control>n2.control)
		return true;
	if (n1.bank<n2.bank)
		return false;
	if (n1.bank>n2.bank)
		return true;
	if (n1.owner1<n2.owner1)
		return false;
	if (n1.owner1>n2.owner1)
		return true;
	if (n1.owner2<n2.owner2)
		return false;
	if (n1.owner2>n2.owner2)
		return true;
	if (n1.owner3<n2.owner3)
		return false;
	if (n1.owner3>n2.owner3)
		return true;
	if (n1.owner4<n2.owner4)
		return false;
	if (n1.owner4>n2.owner4)
		return true;
	return true;
}
inline bool operator<=(const number& n1,const number& n2)
{
	if (n1.control<n2.control)
		return true;
	if (n1.control>n2.control)
		return false;
	if (n1.bank<n2.bank)
		return true;
	if (n1.bank>n2.bank)
		return false;
	if (n1.owner1<n2.owner1)
		return true;
	if (n1.owner1>n2.owner1)
		return false;
	if (n1.owner2<n2.owner2)
		return true;
	if (n1.owner2>n2.owner2)
		return false;
	if (n1.owner3<n2.owner3)
		return true;
	if (n1.owner3>n2.owner3)
		return false;
	if (n1.owner4<n2.owner4)
		return true;
	if (n1.owner4>n2.owner4)
		return false;
	return true;
}
/*inline number& operator=(const number& n)
{
	number N;
	N.bank=n.bank;
	N.control=n.control;
	N.owner1=n.owner1;
	N.owner2=n.owner2;
	N.owner3=n.owner3;
	N.owner4=n.owner4;
	return N;
}*/
inline bool operator==(const number& n1,const number& n2)
{
	if (n1.bank!=n2.bank) return false;
	if (n1.control!=n2.control) return false;
	if (n1.owner1!=n2.owner1) return false;
	if (n1.owner2!=n2.owner2) return false;
	if (n1.owner3!=n2.owner3) return false;
	if (n1.owner4!=n2.owner4) return false;
	return true;
}
void quicksort(number a[],int l,int u)
{
	number t;
	if (u-l<=0)
		return;
	if (u-l==1)
	{
		if (a[l]>a[u])
		{
			t=a[u];
			a[u]=a[l];
			a[l]=t;
		}
		return;
	}
	number key=a[l];
	int L=l;
	int R=u;
	do
	{
		while ((R>L)&&(a[R]>=key))
			R--;
		if (R==L) break;
		a[L]=a[R];
		while ((R>L)&&(a[L]<=key))
			L++;
		if (R==L) break;
		a[R]=a[L];
	}
	while (true);
	a[L]=key;
	if (L-l>1)
		quicksort(a,l,L-1);
	if (u-R>1)
		quicksort(a,R+1,u);
}
int main()
{
	int i,j,t,m,n;
	char c[32];
	static number a[100000];
	#ifdef DEBUG
	FILE* f=fopen("SBANK.in","r");
	fscanf(f,"%d",&t);
	#else
	scanf("%d",&t);
	#endif
	for (i=0; i<t; i++)
	{
		if (i>0) putchar('\n');
		#ifdef DEBUG
		fscanf(f,"%d",&n);
		#else
		scanf("%d",&n);
		#endif
		for (j=0; j<n; j++)
		#ifdef DEBUG
			fscanf(f,"%d %d %d %d %d %d",&a[j].control,&a[j].bank,
				&a[j].owner1,&a[j].owner2,&a[j].owner3,&a[j].owner4);
		#else
			scanf("%d %d %d %d %d %d",&a[j].control,&a[j].bank,
				&a[j].owner1,&a[j].owner2,&a[j].owner3,&a[j].owner4);
		#endif
		quicksort(a,0,n-1);
		int k;
		for (j=0; j<n;)
		{
			k=j;
			do
				k++;
			while ((k<n)&&(a[k]==a[j]));
			c[0]=0;
			sprintf(c,"%2d_%8d_%4d_%4d_%4d_%4d",a[j].control,a[j].bank,
				a[j].owner1,a[j].owner2,a[j].owner3,a[j].owner4);
			for (m=0; m<31; m++)
				if (c[m]==' ')
					putchar('0');
				else if (c[m]=='_')
					putchar(' ');
				else
					putchar(c[m]);
			printf(" %d\n",k-j);
			j=k;
		}
	}
	return 0;
}
