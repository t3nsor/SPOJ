// 2008-01-29
#include <iostream>
#define MAX(x,y) ((x)>(y)?(x):(y))
using namespace std;
struct number
{
	int digits[160];
	int length;
	number()
	{
		length=1;
		memset(digits,0,sizeof(digits));
	}
	number(const number& x)
	{
		length=x.length;
		memcpy(digits,x.digits,sizeof(digits));
	}
	void shift()
	{
		int carry=0;
		int d;
		for (d=0; d<=length; d++)
		{
			digits[d]=(digits[d]<<1)+carry;
			if (digits[d]>=10) {digits[d]-=10;carry=1;} else carry=0;
		}
		if (digits[length]) length++;
	}
	void add(number x)
	{
		int l=MAX(length,x.length);
		int d;
		int carry=0;
		for (d=0; d<=l; d++)
		{
			digits[d]=digits[d]+x.digits[d]+carry;
			if (digits[d]>=10) {digits[d]-=10;carry=1;} else carry=0;
		}
		length=l;
		if (digits[length]) length++;
	}
	void mult(int x)
	{
		number y;
		number z(*this);
		while (x>0)
		{
			if (x%2)
				y.add(z);
			z.shift();
			x/=2;
		}
		*this=y;
	}
	void print()
	{
		int i;
		for (i=length-1; i>=0; i--)
			putchar(digits[i]+48);
	}
};
int main()
{
	int t,i,n,j;
	cin >> t;
	for (i=0; i<t; i++)
	{
		cin >> n;
		number f;
		f.digits[0]=1;
		for (j=n; j>1; j--)
			f.mult(j);
		f.print();
		cout << endl;
	}
	return 0;
}
