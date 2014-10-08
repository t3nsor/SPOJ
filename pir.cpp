// 2008-02-02
#include <algorithm>
#include <iostream>
#include <cmath>
using namespace std;
#define int64 long long
int main()
{
	int matrix[5][5];
	int A[5]={0,1,2,3,4};
	int64 det;
	int t,i,j,k;
	int a,b,c,d,e,f;
	cin >> t;
	for (i=0; i<t; i++)
	{
		cin >> a >> b >> c >> d >> e >> f;
		matrix[0][0]=0;  matrix[0][1]=1;  matrix[0][2]=1;  matrix[0][3]=1;  matrix[0][4]=1;
		matrix[1][0]=1;  matrix[1][1]=0;  matrix[1][2]=a*a;matrix[1][3]=b*b;matrix[1][4]=c*c;
		matrix[2][0]=1;  matrix[2][1]=a*a;matrix[2][2]=0;  matrix[2][3]=d*d;matrix[2][4]=e*e;
		matrix[3][0]=1;  matrix[3][1]=b*b;matrix[3][2]=d*d;matrix[3][3]=0;  matrix[3][4]=f*f;
		matrix[4][0]=1;  matrix[4][1]=c*c;matrix[4][2]=e*e;matrix[4][3]=f*f;matrix[4][4]=0;
		det=0;
		while (next_permutation(A,A+5))
		{
			int test=1;
			for (j=0; j<4; j++)
				for (k=j+1; k<5; k++)
					test*=A[j]-A[k];
			int64 product=1;
			for (j=0; j<5; j++)
				product*=matrix[j][A[j]];
			if (test>0)
				det+=product;
			else
				det-=product;
		}
		double V=sqrt((double)det/288);
		printf("%.4f\n",V);
	}
	return 0;
}
