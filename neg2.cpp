// 2008-02-02
#include <iostream>
#include <string>
using namespace std;
int main()
{
	int N;
	cin >> N;
	string s="";
	while (N!=0)
	{
		if (N%2==0)
		{
			s='0'+s;
			N/=-2;
		}
		else
		{
			s='1'+s;
			N=-(N-1)/2;
		}
	}
	if (s=="") cout << "0\n"; else cout << s << endl;
	return 0;
}
