// 2009-06-28
#include <iostream>
#include <string>
using namespace std;
bool dnpalin(string s)
{
	int cnt=0,i,j;
	for (i=0,j=s.length()-1; i<j; i++,j--)
		if (s[i]!=s[j])
			cnt++;
	if (s.length()%2)
		return cnt<=1;
	else
		return cnt==1;
}
int main()
{
	for(;;)
	{
		string s;
		int i;
		cin >> s;
		if (s=="*END*")
			return 0;
		bool ok=false;
		for (i=1; i<s.length(); i++)
			ok|=dnpalin(s.substr(0,i))&&dnpalin(s.substr(i));
		cout << s << " is";
		if (!ok) cout << " not";
		cout << " a double near palindrome." << endl;
	}
}
