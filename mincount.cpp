// 2008-05-15
#include <iostream>
using namespace std;
int main()
{
	int N,i;
	cin >> N;
	unsigned long long L;
	for (i=0; i<N; i++)
	{
		cin >> L;
		switch(L%6)
		{
			case 0:cout << (L/6)*(L+1); break;
			case 1:cout << (L-1)/6*(L+2); break;
			case 2:cout << (L/2)*((L+1)/3); break;
			case 3:cout << (L/3)*((L+1)/2); break;
			case 4:cout << (L-1)/3*((L+2)/2); break;
			case 5:cout << L*((L+1)/6);
		}
		cout << endl;
	}
	return 0;
}
