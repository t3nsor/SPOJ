// 2009-06-12
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
int main()
{
	int T,i,cnt,res,cur;
	int lengths[10000];
	char s[100000];
	cin >> T; cin.getline(s,10000);
	while (T--)
	{
		cin.getline(s,100000);
		istringstream ISS(s);
		string t;
		cnt=0;
		while (ISS >> t)
			lengths[cnt++]=t.length();
		lengths[cnt++]=0;
		res=0;
		cur=1;
		for (i=1; i<cnt; i++)
			if (lengths[i]==lengths[i-1])
				cur++;
			else
			{
				if (cur>res) res=cur;
				cur=1;
			}
		printf("%d\n",res);
	}
	return 0;
}
