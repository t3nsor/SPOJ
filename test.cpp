// 2007-10-13
#include <iostream>

using namespace std;

int main()
{
     int x;
     int b=false;
     do
     {
          cin >> x;
          if (x==42)
               break;
          else
          {
               if (b)
                    cout << endl;
               else b=true;
               cout << x;
          }
     }
     while(true);
     return 0;
}
