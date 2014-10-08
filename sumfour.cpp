// 2008-06-06
#include <iostream>
#include <algorithm>
#define MAX 16000000
using namespace std;
int main()
{
	int i,j,i2,j2,n;
	int A[4000],B[4000],C[4000],D[4000];
	static int E[MAX],F[MAX];
	cin >> n;
	for (i=0; i<n; i++)
		cin >> A[i] >> B[i] >> C[i] >> D[i];
	for (i=0; i<n; i++)
		for (j=0; j<n; j++)
		{
			E[i+n*j]=A[i]+B[j];
			F[i+n*j]=C[i]+D[j];
		}
	sort(E,E+n*n);
	sort(F,F+n*n);
	i=0;
	i2=0;
	j=n*n-1;
	j2=n*n-1;
	int __count=0;
	while (i<n*n&&j>=0)
	{
		if (i2==i)
			while (i2<n*n&&E[i2]==E[i]) i2++;
		if (j2==j)
			while (j2>=0&&F[j2]==F[j]) j2--;
		if (E[i]+F[j]==0)
		{
			__count+=(i2-i)*(j-j2);
			i=i2;
			j=j2;
		}
		else if (E[i]+F[j]>0)
			j=j2;
		else
			i=i2;
	}
	cout << __count << endl;
	return 0;
}
