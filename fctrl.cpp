// 2007-10-19
#include <iostream>

using namespace std;

int main()
{
     int i,N,x,y;
     cin >> N;
     for (i=0; i<N; i++)
     {
          cin >> x;
          y=0;
          do
          {
               x/=5;
               y+=x;
          }
          while (x>0);
          cout << y << endl;
     }
}   
