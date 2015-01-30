// 2008-10-28
#include <iostream>
#define GC getchar()
using namespace std;
char l[3][3],f[3][3],r[3][3],b[3][3],u[3][3],d[3][3];
bool check()
{
	int i,j;
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			if (l[i][j]-l[1][1])
				return false;
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			if (f[i][j]-f[1][1])
				return false;
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			if (r[i][j]-r[1][1])
				return false;
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			if (b[i][j]-b[1][1])
				return false;
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			if (u[i][j]-u[1][1])
				return false;
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			if (d[i][j]-d[1][1])
				return false;
	return true;
}
void cycle4(char& a,char& b, char& c, char& d)
{
	char t=a;
	a=b;
	b=c;
	c=d;
	d=t;
}
void L()
{
	cycle4(l[0][0],l[0][2],l[2][2],l[2][0]);
	cycle4(l[1][0],l[0][1],l[1][2],l[2][1]);
	cycle4(u[0][0],b[2][2],d[0][0],f[0][0]);
	cycle4(u[0][1],b[2][1],d[0][1],f[0][1]);
	cycle4(u[0][2],b[2][0],d[0][2],f[0][2]);
}
void F()
{
	cycle4(f[0][0],f[0][2],f[2][2],f[2][0]);
	cycle4(f[1][0],f[0][1],f[1][2],f[2][1]);
	cycle4(u[0][2],l[2][2],d[2][0],r[0][0]);
	cycle4(u[1][2],l[2][1],d[1][0],r[0][1]);
	cycle4(u[2][2],l[2][0],d[0][0],r[0][2]);
}
void R()
{
	cycle4(r[0][0],r[0][2],r[2][2],r[2][0]);
	cycle4(r[1][0],r[0][1],r[1][2],r[2][1]);
	cycle4(u[2][2],f[2][2],d[2][2],b[0][0]);
	cycle4(u[2][1],f[2][1],d[2][1],b[0][1]);
	cycle4(u[2][0],f[2][0],d[2][0],b[0][2]);
}
void B()
{
	cycle4(b[0][0],b[0][2],b[2][2],b[2][0]);
	cycle4(b[1][0],b[0][1],b[1][2],b[2][1]);
	cycle4(u[2][0],r[2][2],d[0][2],l[0][0]);
	cycle4(u[1][0],r[2][1],d[1][2],l[0][1]);
	cycle4(u[0][0],r[2][0],d[2][2],l[0][2]);
}
void U()
{
	cycle4(u[0][0],u[0][2],u[2][2],u[2][0]);
	cycle4(u[1][0],u[0][1],u[1][2],u[2][1]);
	cycle4(f[0][0],r[0][0],b[0][0],l[0][0]);
	cycle4(f[1][0],r[1][0],b[1][0],l[1][0]);
	cycle4(f[2][0],r[2][0],b[2][0],l[2][0]);
}
void D()
{
	cycle4(d[0][0],d[0][2],d[2][2],d[2][0]);
	cycle4(d[1][0],d[0][1],d[1][2],d[2][1]);
	cycle4(f[0][2],l[0][2],b[0][2],r[0][2]);
	cycle4(f[1][2],l[1][2],b[1][2],r[1][2]);
	cycle4(f[2][2],l[2][2],b[2][2],r[2][2]);
}
int main()
{
	int i,T,n;
	scanf("%d",&T);
	while (T--)
	{
		GC;
		GC; GC; GC; GC; GC; GC;
		u[0][0]=GC; GC; u[1][0]=GC; GC; u[2][0]=GC; GC;
		GC; GC; GC; GC; GC; GC;
		u[0][1]=GC; GC; u[1][1]=GC; GC; u[2][1]=GC; GC;
		GC; GC; GC; GC; GC; GC;
		u[0][2]=GC; GC; u[1][2]=GC; GC; u[2][2]=GC; GC;
		l[0][0]=GC; GC; l[1][0]=GC; GC; l[2][0]=GC; GC;
		f[0][0]=GC; GC; f[1][0]=GC; GC; f[2][0]=GC; GC;
		r[0][0]=GC; GC; r[1][0]=GC; GC; r[2][0]=GC; GC;
		b[0][0]=GC; GC; b[1][0]=GC; GC; b[2][0]=GC;
		do l[0][1]=GC; while (l[0][1]<=32); GC; l[1][1]=GC; GC; l[2][1]=GC; GC;
		f[0][1]=GC; GC; f[1][1]=GC; GC; f[2][1]=GC; GC;
		r[0][1]=GC; GC; r[1][1]=GC; GC; r[2][1]=GC; GC;
		b[0][1]=GC; GC; b[1][1]=GC; GC; b[2][1]=GC; GC;
		l[0][2]=GC; GC; l[1][2]=GC; GC; l[2][2]=GC; GC;
		f[0][2]=GC; GC; f[1][2]=GC; GC; f[2][2]=GC; GC;
		r[0][2]=GC; GC; r[1][2]=GC; GC; r[2][2]=GC; GC;
		b[0][2]=GC; GC; b[1][2]=GC; GC; b[2][2]=GC; GC;
		GC; GC; GC; GC; GC; GC;
		d[0][0]=GC; GC; d[1][0]=GC; GC; d[2][0]=GC; GC;
		GC; GC; GC; GC; GC; GC;
		d[0][1]=GC; GC; d[1][1]=GC; GC; d[2][1]=GC; GC;
		GC; GC; GC; GC; GC; GC;
		d[0][2]=GC; GC; d[1][2]=GC; GC; d[2][2]=GC; GC;
		for(;;)
		{
			scanf("%d",&n);
			if (!n) break;
			switch(n)
			{
				case 1:L(); break;
				case 2:F(); break;
				case 3:R(); break;
				case 4:B(); break;
				case 5:U(); break;
				case 6:D(); break;
				case -1:L(),L(),L(); break;
				case -2:F(),F(),F(); break;
				case -3:R(),R(),R(); break;
				case -4:B(),B(),B(); break;
				case -5:U(),U(),U(); break;
				case -6:D(),D(),D(); break;
			}
		}
		if (check())
			printf("Yes, grandpa!\n");
		else
			printf("No, you are wrong!\n");
	}
	return 0;
}
