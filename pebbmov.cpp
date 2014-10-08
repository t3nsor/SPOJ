// 2009-05-05
#include <iostream>
#include <set>
#include <string>
using namespace std;
int main()
{
	int N,i;
	string s;
	for(;;)
	{
		set<string> S;
		if (scanf("%d",&N)!=1)
			return 0;
		for (i=0; i<N; i++)
		{
			int x;
			cin >> s;
			if (S.find(s)==S.end())
				S.insert(s);
			else
				S.erase(s);
		}
		if (S.empty())
			puts("second player");
		else
			puts("first player");
	}
}
