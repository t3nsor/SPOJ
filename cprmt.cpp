// 2008-04-19
#include <iostream>
#include <string>
#define MIN(a,b) ((a)<(b)?(a):(b))
using namespace std;
int main()
{
	string s1,s2;
	int i;
	char c;
	int let1[256];
	int let2[256];
	while (!cin.eof())
	{
		cin >> s1 >> s2;
		memset(let1,0,sizeof(let1));
		memset(let2,0,sizeof(let2));
		for (i=0; i<s1.length(); i++)
			let1[s1[i]]++;
		for (i=0; i<s2.length(); i++)
			let2[s2[i]]++;
		string result="";
		for (c='a'; c<='z'; c++)
			for (i=0; i<MIN(let1[c],let2[c]); i++)
				result+=c;
		cout << result << endl;
	}
	return 0;
}
