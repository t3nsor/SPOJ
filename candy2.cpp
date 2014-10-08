// 2008-04-30
#include <iostream>
#include <algorithm>
using namespace std;
struct str
{
	int number;
	int bag;
};
bool operator<(str s1,str s2)
{
	return (s1.number<s2.number);
}
int main()
{
	int N,sum,i,j,k,c,s,b,C,S,B;
	str str1[5000],str2[5000],str3[5000];
	cin >> N;
	sum=0;
	for (i=0; i<N; i++)
	{
		cin >> c >> s >> b;
		sum+=c+s+b;
		str1[i].bag=i; str2[i].bag=i; str3[i].bag=i;
		str1[i].number=c; str2[i].number=s; str3[i].number=b;
	}
	sort(str1,str1+N); reverse(str1,str1+N);
	sort(str2,str2+N); reverse(str2,str2+N);
	sort(str3,str3+N); reverse(str3,str3+N);
	int moves=(1<<31)-1;
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			for (k=0; k<3; k++)
			{
				if (str1[i].bag==str2[j].bag||
					str1[i].bag==str3[k].bag||
					str2[j].bag==str3[k].bag)
					continue;
				if (moves>sum-str1[i].number-str2[j].number-str3[k].number)
				{
					moves=sum-str1[i].number-str2[j].number-str3[k].number;
					C=str1[i].bag;
					S=str2[j].bag;
					B=str3[k].bag;
				}
			}
	cout << C << endl << S << endl << B << endl;
	return 0;
}
