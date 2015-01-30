// 2008-12-11
#include <iostream>
#include <algorithm>
#include <string>
using namespace std;
int ceil(double x)
{
	x=x+1e-8;
	double z=x-int(x);
	if (z<1e-8)
		return int(x);
	else
		return int(x)+1;
}
int main()
{
	int t,i,n,j;
	string s;
	cin >> t;
	double a[1000];
	for (i=0; i<t; i++)
	{
		cin >> n;
		for (j=0; j<n; j++)
			cin >> s >> a[j];
		sort(a,a+n);
		double maxdiff=0.0;
		for (j=0; j<n; j++)
		{
			double diff=a[j]-a[(j+n-1)%n];
			if (diff>maxdiff)
				maxdiff=diff;
			else if (diff<0.0 && diff+360.0>maxdiff)
				maxdiff=diff+360.0;
		}
		double seconds=12.0*(360.0-maxdiff);
		cout << ceil(seconds) << endl;
	}
	return 0;
}
