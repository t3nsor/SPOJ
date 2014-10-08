// 2011-04-05
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;
int main()
{
    int T;
    scanf("%d",&T);
    int i;
    static int a[1000000];
    for (i=0; i<T; i++)
    {
        int x,n,j;
        printf("Scenario #%d:\n",i+1);
        scanf("%d %d",&x,&n);
        for (j=0; j<n; j++)
            scanf("%d",a+j);
        sort(a,a+n,greater<int>());
        int sum=0;
        j=0;
        while (j<n&&sum<x)
            sum+=a[j++];
        if (sum>=x) printf("%d\n",j);
        else puts("impossible");
        putchar('\n');
    }
    return 0;
}
