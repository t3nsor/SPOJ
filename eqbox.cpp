// 2008-07-07
// modified 2010-10-07
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
typedef long long i64;
inline i64 SQR(i64 x)
{
	return x*x;
}
int main()
{
	int i,t;
	i64 A,B,X,Y;
	scanf("%d",&t);
	for (i=0; i<t; i++)
	{
		scanf("%lld %lld %lld %lld", &A, &B, &X, &Y);
		if (A<B) swap(A,B);
		if (X<Y) swap(X,Y);
		if (A>X)
			if (B>Y)
				printf("Escape is possible.\n");
			else
				printf("Box cannot be dropped.\n");
		else if (Y>=B)
			printf("Box cannot be dropped.\n");
		else if (B*(X*X+Y*Y)-2*A*X*Y>(X*X-Y*Y)*sqrt(X*X+Y*Y-A*A))
			printf("Escape is possible.\n");
		else
			printf("Box cannot be dropped.\n");
	}
	return 0;
}
