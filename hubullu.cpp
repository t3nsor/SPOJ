// 2008-04-20
#include <iostream>
using namespace std;
int main()
{
	int i,t,N,p;
	cin >> t;
	for (i=0; i<t; i++)
	{
		cin >> N >> p;
		if (p==0)
			cout << "Airborne wins.\n";
		else
			cout << "Pagfloyd wins.\n";
	}
	return 0;
}
