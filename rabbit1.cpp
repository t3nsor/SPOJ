// 2008-06-11
#include <iostream>
using namespace std;
int main()
{
	static int F[1572864];
	int i,C,N,M;
	F[0]=1;
	F[1]=1;
	for (i=2; i<1572864; i++)
		F[i]=(F[i-1]+F[i-2])%1048576;
	cin >> C;
	for (i=0; i<C; i++)
	{
		cin >> N >> M;
		cout << F[N%1572864]%(1<<M) << endl;
	}
	return 0;
}
