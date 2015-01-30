// 2008-07-24
#include <iostream>
#include <string>
using namespace std;
int XWins(string s[])
{
	int c=0;
	if (s[0][0]=='X'&&s[1][0]=='X'&&s[2][0]=='X')
		c++;
	if (s[0][1]=='X'&&s[1][1]=='X'&&s[2][1]=='X')
		c++;
	if (s[0][2]=='X'&&s[1][2]=='X'&&s[2][2]=='X')
		c++;
	if (s[0][0]=='X'&&s[0][1]=='X'&&s[0][2]=='X')
		c++;
	if (s[1][0]=='X'&&s[1][1]=='X'&&s[1][2]=='X')
		c++;
	if (s[2][0]=='X'&&s[2][1]=='X'&&s[2][2]=='X')
		c++;
	if (s[0][0]=='X'&&s[1][1]=='X'&&s[2][2]=='X')
		c++;
	if (s[0][2]=='X'&&s[1][1]=='X'&&s[2][0]=='X')
		c++;
	return c;
}
int OWins(string s[])
{
	int c=0;
	if (s[0][0]=='O'&&s[1][0]=='O'&&s[2][0]=='O')
		c++;
	if (s[0][1]=='O'&&s[1][1]=='O'&&s[2][1]=='O')
		c++;
	if (s[0][2]=='O'&&s[1][2]=='O'&&s[2][2]=='O')
		c++;
	if (s[0][0]=='O'&&s[0][1]=='O'&&s[0][2]=='O')
		c++;
	if (s[1][0]=='O'&&s[1][1]=='O'&&s[1][2]=='O')
		c++;
	if (s[2][0]=='O'&&s[2][1]=='O'&&s[2][2]=='O')
		c++;
	if (s[0][0]=='O'&&s[1][1]=='O'&&s[2][2]=='O')
		c++;
	if (s[0][2]=='O'&&s[1][1]=='O'&&s[2][0]=='O')
		c++;
	return c;
}
int NumberOfX(string r[])
{
	int i,j;
	int c=0;
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			if (r[i][j]=='X')
				c++;
	return c;
}
int NumberOfO(string r[])
{
	int i,j;
	int c=0;
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
			if (r[i][j]=='O')
				c++;
	return c;
}
int main()
{
	int N,i,j,k;
	string s[3];
	cin >> N;
	for (i=0; i<N; i++)
	{
		for (j=0; j<3; j++)
			cin >> s[j];
		//if X wins, there must be one more X than the # of Os
		//if O wins, there must be the same number of Os as Xs
		int Xw=XWins(s);
		int Ow=OWins(s);
		int Xs=NumberOfX(s);
		int Os=NumberOfO(s);
		if (Xw&&Ow||Os>Xs||Os<Xs-1||Xs==Os&&Xw||Xs==Os+1&&Ow)
			cout << "no";
		else if (Xw)
		{
			bool b=true;
			for (j=0; j<3; j++)
				for (k=0; k<3; k++)
				{
					if (s[j][k]!='X')
						continue;
					s[j][k]='.';
					int z=XWins(s);
					if (z==0)
						b=true;
					s[j][k]='X';
				}
			if (b)
				cout << "yes";
			else
				cout << "no";
		}
		else if (Ow)
		{
			bool b=false;
			for (j=0; j<3; j++)
				for (k=0; k<3; k++)
				{
					if (s[j][k]!='O')
						continue;
					s[j][k]='.';
					int z=XWins(s);
					if (z==0)
						b=true;
					s[j][k]='O';
				}
			if (b)
				cout << "yes";
			else
				cout << "no";
		}
		else
			cout << "yes";
		cout << endl;
	}
	return 0;
}
