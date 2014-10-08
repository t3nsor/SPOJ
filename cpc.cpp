// 2008-06-13
#include <iostream>
#include <string>
#ifdef _MSC_VER
#define int64 __int64
#define getchar_unlocked getchar
#define putchar_unlocked putchar
#else
#define int64 long long
#endif
#define number(c) ((c)>='0' && (c)<='9')
#define letter(c) ((c)>='A' && (c)<='Z')
using namespace std;
//              A B  C D E F G H  I J K L  M N O  P  Q  R  S  T  U  V  W  X  Y  Z
int value[26]={-1,0,-1,1,2,3,4,5,-1,6,7,8,-1,9,10,-1,11,12,13,14,15,16,17,18,19,20};
int64 find_value(char s[7])
{
	int i;
	if (s[3]<'A')
	{
		//LLLDDDD
		for (i=0; i<3; i++)
			if (!letter(s[i]))
				return 0;
		for (i=3; i<7; i++)
			if (!number(s[i]))
				return 0;
		return 6760000*(s[0]-'A')+260000*(s[1]-'A')+10000*(s[2]-'A')+1000*(s[3]-'0')+100*(s[4]-'0')+10*(s[5]-'0')+s[6]-'0';
	}
	else
	{
		//LLLLLDD
		int i;
		for (i=0; i<5; i++)
		{
			if (!letter(s[i]))
				return 0;
			if (value[s[i]-'A']==-1)
				return 0;
		}
		for (i=5; i<7; i++)
			if (!number(s[i]))
				return 0;
		return 175760000+19448100*(value[s[0]-'A'])+926100*(value[s[1]-'A'])+44100*(value[s[2]-'A'])+2100*(value[s[3]-'A'])+100*(value[s[4]-'A'])+10*(s[5]-'0')+s[6]-'0';
	}
}
int input()
{
  int x=0;
  char c;
  for(;;)
  {
    c=getchar_unlocked();
    if (c==' '||c=='\n')
      return x;
    x=10*x+c-'0';
  }
}
int main()
{
	char s1[7],s2[7];
	int n,i;
	int64 v1,v2;
	for(;;)
	{
		for (i=0; i<7; i++)
		{
                  s1[i]=getchar_unlocked();
				  if (s1[i]=='*')
					  return 0;
		}
                getchar_unlocked();
                for (i=0; i<7; i++)
                  s2[i]=getchar_unlocked();
                getchar_unlocked();
                n=input();
		if (s1[0]=='*') return 0;
		v1=find_value(s1);
		v2=find_value(s2);
		if (v2>v1&&v2-v1<=n)
                        putchar_unlocked('Y');
		else
                        putchar_unlocked('N');
                putchar_unlocked('\n');
	}
}
