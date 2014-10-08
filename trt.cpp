// 2008-06-27
// modified 2014-10-07
#include <cstdio>
typedef long long i64;
inline i64 MAX(i64 x,i64 y)
{
	if (x>y) return x; else return y;
}
int main()
{
	int N,i,j;
	int treat[2000];
	static i64 dp[2000][2000];
	scanf("%d",&N);
	for (i=0; i<N; i++)
		scanf("%d",treat+i);
	for (i=0; i<N; i++)
		dp[i][i]=(i64)N*(i64)treat[i];
	for (i=1; i<N; i++)
		for (j=0; j<N-i; j++)
			dp[j][j+i]=MAX((i64)(N-i)*treat[j]+dp[j+1][j+i],dp[j][j+i-1]+(i64)(N-i)*treat[j+i]);
	printf("%lld\n",dp[0][N-1]);
	return 0;
}
