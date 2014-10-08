// 2010-11-15
#include <cstdio> 
#include <cstring> 
#include <cstdlib> 
class Tester{
private:
	char str[100001],sec[99];
	int cnt,n;
public:
	Tester(){cnt=0,n=strlen(gets(str)),gets(sec);}
	inline int getN(){return n;}
	inline bool isSame(int a,int b){return ++cnt<=4*n&&a>=0&&a<n&&b>=0&&b<n?str[a]==str[b]:(printf("ERROR while calling isSame()\n"),exit(1),false);}
	inline void answer(int a){printf("%d\n%s\n",a,sec),exit(0);}
}tester;
inline int getN(){return tester.getN();}
inline bool isSame(int a,int b){return tester.isSame(a,b);}
inline void answer(int a){tester.answer(a);}
#define tester _tester_
//---------------------
//real code starts here
//---------------------
// Thanks to Frederick Akalin
#include <algorithm>
using namespace std;
int N;
int out[222222]={0};
int fastLongestPalindromes()
{
	int i=0,j,d,s,e,lLen,k=0;
	int palLen=0;
	int res=0;
	while (i<N)
	{
		if (i>palLen && isSame(i-palLen-1,i))
		{
			palLen+=2;
			i++;
			continue;
		}
		out[k++]=palLen;
		res=max(res,out[k-1]);
		s=k-2;
		e=s-palLen;
		bool b=true;
		for (j=s; j>e; j--)
		{
			d=j-e-1;
			if (out[j]==d)
			{
				palLen=d;
				b=false;
				break;
			}
			out[k++]=min(d,out[j]);
		}
		if (b)
		{
			palLen=1;
			i++;
		}
	}
	out[k++]=palLen;
	res=max(res,out[k-1]);
	return res;
}

int main()
{
	N=getN();
	answer(fastLongestPalindromes());
}

