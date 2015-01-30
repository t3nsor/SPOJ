// 2008-01-29
#include <stdio.h>
inline void swap(int&x,int&y)
{
	int t=x;
	x=y;
	y=t;
}
int main()
{
	int N[101];
	int D[101];
	int k[101];
	int n[101];
	int i,d,j,carry,remainder,borrow;
	bool begin;
	char c;
	for (i=0; i<10; i++)
	{
		d=0;
		while ((c=getchar())!='\n')
			N[d++]=c-48;
		for (j=0; j<d/2; j++)
			swap(N[j],N[d-j-1]);
		for (j=d; j<101; j++)
			N[j]=0;
		d=0;
		while ((c=getchar())!='\n')
			D[d++]=c-48;
		for (j=0; j<d/2; j++)
			swap(D[j],D[d-j-1]);
		for (j=d; j<101; j++)
			D[j]=0;
		d=0;
		carry=0;
		while (d<=100)
		{
			N[d]=N[d]+D[d]+carry;
			if (N[d]>=10){N[d]-=10;carry=1;}
			else carry=0;
			d++;
		}
		remainder=0;
		begin=false;
		d=100;
		while (d>=0)
		{
			k[d]=(N[d]+remainder)/2;
			if (k[d]) begin=true;
			if (begin) putchar(k[d]+48);
			if ((N[d]+remainder)%2==1)
				remainder=10;
			else
				remainder=0;
			d--;
		}
		putchar('\n');
		d=0;
		borrow=0;
		while (d<=100)
		{
			n[d]=k[d]-D[d]-borrow;
			if (n[d]<0){n[d]+=10;borrow=1;}
			else borrow=0;
			d++;
		}
		for (d=100;!n[d]&&d>=0;d--);
		if (d==-1) putchar('0');
		for (;d>=0;d--)
			putchar(n[d]+48);
		putchar('\n');		
	}
	return 0;
}
