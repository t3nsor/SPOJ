#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    int soma, n, t, temp, input;
    vector<int> levelsman, levelswoman;
    cin >> t;
    
    while(t-- > 0)
    {
        cin >> n;
        temp = n;
        while(temp-- > 0)
        {
            cin >> input;
            levelsman.push_back(input);
        }
        temp = n;
        while(temp-- > 0)
        {
            cin >> input;
            levelswoman.push_back(input);
        }
        sort(levelswoman.begin(), levelswoman.end());
        sort(levelsman.begin(), levelsman.end());
        
        soma = 0;
        for(int i = 0; i < n; i++)
        {
            soma += levelsman[i]*levelswoman[i];
        }
        
        cout << soma << "\n";
        levelsman.clear();
        levelswoman.clear();
    }
    return 0;
}