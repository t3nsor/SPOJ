// 2008-01-29
#include <iostream>
#include <string>
using namespace std;
string convert(string e)
{
	int i,depth,j;
	e.erase(0,1);
	e.erase(e.length()-1,1);
	string l="",r="",o;
	if (e[0]!='(')
	{
		l=e[0];
		o=e[1];
	}
	else
	{
		i=0;
		depth=1;
		do
		{
			i++;
			if (e[i]=='(')
				depth++;
			else if (e[i]==')')
				depth--;
		}
		while (depth>0);
		for (j=0; j<=i; j++)
			l=l+e[j];
		o=e[l.length()];
		l=convert(l);
	}
	if (e[e.length()-1]!=')')
		r=e[e.length()-1];
	else
	{
		i=e.length()-1;
		depth=1;
		do
		{
			i--;
			if (e[i]==')')
				depth++;
			else if (e[i]=='(')
				depth--;
		}
		while (depth>0);
		for (j=i; j<e.length(); j++)
			r=r+e[j];
		r=convert(r);
	}
	return l+r+o;
}
int main()
{
	int t,i;
	string s,S;
	cin >> t;
	for (i=0; i<t; i++)
	{
		cin >> s;
		S=convert(s);
		cout << S << endl;
	}
	return 0;
}
