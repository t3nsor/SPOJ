// 2008-04-21
#include <iostream>
#include <string>
using namespace std;
bool java(string s)
{
	int i;
	for (i=0; i<s.length(); i++)
		if (s[i]=='_')
			return false;
	if (s[0]<='Z')
		return false;
	return true;
}
bool cpp(string s)
{
	int i;
	for (i=0; i<s.length(); i++)
		if (s[i]<='Z')
			return false;
	if (s[0]=='_')
		return false;
	if (s[s.length()-1]=='_')
		return false;
	for (i=0; i<s.length()-2; i++)
		if (s[i]=='_' && s[i+1]=='_')
			return false;
	return true;
}
string translate_to_cpp(string s) //Java to C++
{
	int i;
	for (i=0; i<s.length(); i++)
		if (s[i]<='Z')
		{
			char c=s[i];
			s.insert(i,"_");
			s[i+1]=c+32;
		}
	return s;
}
string translate_to_java(string s) //C++ to Java
{
	int i;
	for (i=0; i<s.length(); i++)
		if (s[i]=='_')
		{
			s.erase(i,1);
			s[i]=s[i]-32;
		}
	return s;
}
int main()
{
	string s;
	while (!cin.eof())
	{
		cin >> s;
		if (java(s))
			cout << translate_to_cpp(s) << endl;
		else if (cpp(s))
			cout << translate_to_java(s) << endl;
		else
			cout << "Error!\n";
	}
	return 0;
}
