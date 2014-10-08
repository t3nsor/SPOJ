// 2008-06-08
#include <iostream>
using namespace std;
int main()
{
	int i,j,k,m,n,t;
	int a[200][200];
	cin >> t;
	for (i=0; i<t; i++)
	{
		cin >> n;
		for (j=0; j<n; j++)
			for (k=0; k<n; k++)
				cin >> a[j][k];
		for (j=0; j<n; j++)
			for (k=j+1; k<n; k++)
			{
				bool b=true;
				for (m=0; m<n; m++)
				{
					if (m==j||m==k)
						continue;
					if (a[j][m]+a[m][k]==a[j][k])
					{
						b=false;
						break;
					}
				}
				if (b)
					cout << j+1 << ' ' << k+1 << endl;
			}
	}
	return 0;
}
