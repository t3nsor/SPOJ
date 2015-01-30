// 2008-04-20
#include <iostream>
using namespace std;
int main()
{
	double x;
	for(;;)
	{
		cin >> x;
		if (x==0.0)
			return 0;
		double sum=0.0;
		int i=1;
		while (sum<x+1)
		{
			sum+=1.0/i;
			i++;
		}
		cout << i-2 << " card(s)\n";
	}
}
