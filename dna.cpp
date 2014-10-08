// 2008-07-23
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
string amino[64]={
	"Phe","Ser","Tyr","Cys","Phe","Ser","Tyr","Cys","Leu","Ser","---","---","Leu","Ser","---","Trp",
	"Leu","Pro","His","Arg","Leu","Pro","His","Arg","Leu","Pro","Gln","Arg","Leu","Pro","Gln","Arg",
	"Ile","Thr","Asn","Ser","Ile","Thr","Asn","Ser","Ile","Thr","Lys","Arg","Met","Thr","Lys","Arg",
	"Val","Ala","Asp","Gly","Val","Ala","Asp","Gly","Val","Ala","Glu","Gly","Val","Ala","Glu","Gly"};
string translate(char c1,char c2,char c3)
{
	int n1,n2,n3;
	if (c1=='U') n1=0;
	else if (c1=='C') n1=1;
	else if (c1=='A') n1=2;
	else n1=3;
	if (c2=='U') n2=0;
	else if (c2=='C') n2=1;
	else if (c2=='A') n2=2;
	else n2=3;
	if (c3=='U') n3=0;
	else if (c3=='C') n3=1;
	else if (c3=='A') n3=2;
	else n3=3;
	return amino[n2+4*n3+16*n1];
}
string complement(string s)
{
	string result;
	result.resize(s.length());
	int i;
	for (i=0; i<s.length(); i++)
	{
		if (s[i]=='A')
			result[i]='T';
		else if (s[i]=='C')
			result[i]='G';
		else if (s[i]=='G')
			result[i]='C';
		else
			result[i]='A';
	}
	return result;
}
string protein(string s)
{
	int i,j;
	string result,s2;
	if (s.length()<3)
		return "";
	for (i=0; i<s.length(); i++)
	{
		if (s[i]=='A')
			s[i]='U';
		else if (s[i]=='C')
			s[i]='G';
		else if (s[i]=='G')
			s[i]='C';
		else
			s[i]='A';
	}
	for (i=0; i<s.length()-2; i++)
	{
		if (translate(s[i],s[i+1],s[i+2])!="Met")
			continue;
		result="";
		for (j=i+3; j<s.length()-2; j+=3)
		{
			s2=translate(s[j],s[j+1],s[j+2]);
			if (s2=="---")
				break;
			result=result+"-"+s2;
		}
		if (s2!="---")
			continue;
		if (result.length())
		{
			result.erase(0,1);
			return result;
		}
	}
	return "";
}
int main()
{
	string s,result;
	for(;;)
	{
		cin >> s;
		if (s=="*")
			return 0;
		result=protein(complement(s));
		if (result.length())
		{
			cout << result << endl;
			continue;
		}
		reverse(s.begin(),s.end());
		result=protein(complement(s));
		if (result.length())
		{
			cout << result << endl;
			continue;
		}
		result=protein(s);
		if (result.length())
		{
			cout << result << endl;
			continue;
		}
		reverse(s.begin(),s.end());
		result=protein(s);
		if (result.length())
		{
			cout << result << endl;
			continue;
		}
		cout << "*** No translatable DNA found ***" << endl;
	}
}
