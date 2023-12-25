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
    cin.sync_with_stdio(false);
    for (;;)
    {
        string s;
        int n,i,j,k;
        cin >> s; if (s==".") return 0;
        cin >> n;
        for (i=0; i<s.length(); i++)
        {
            for (j=0; j<n; j++)
                for (k=0; k<s.length(); k++)
                    putchar(s[(k+i)%s.length()]);
            putchar('\n');
        }
    }
}

