// 2008-06-23
#include <iostream>
#include <string>
#include <map>
//#include <rope.h>
using namespace std;
int result(int x,int y) //-1 = first player wins, 0 = tie, 1 = second player wins
{
	if (x==y)
		return 0;
	else if ((x+1)%3==y)
		return -1;
	else
		return 1;
}
/*bool operator<(string s1,string s2)
{
	int i;
	while (i<s1.length()&&i<s2.length()&&s1[i]==s2[i])
		i++;
	if (i==s2.length())
		return false; //second string longer or same
	else if (i==s1.length())
		return true;
	else if (s1[i]<s2[i])
		return true;
	else
		return false;
}*/
int main()
{
	map<string,int> moves;
	moves["Kamen"]=0; moves["Nuzky"]=1; moves["Papir"]=2;
	moves["Rock"]=0; moves["Scissors"]=1; moves["Paper"]=2;
	moves["Pierre"]=0; moves["Ciseaux"]=1; moves["Feuille"]=2;
	moves["Stein"]=0; moves["Schere"]=1; moves["Papier"]=2;
	moves["Ko"]=0; moves["Koe"]=0; moves["Ollo"]=1; moves["Olloo"]=1; moves["Papir"]=2;
	moves["Sasso"]=0; moves["Roccia"]=0; moves["Forbice"]=1; moves["Carta"]=2; moves["Rete"]=2;
	moves["Guu"]=0; moves["Choki"]=1; moves["Paa"]=2;
	moves["Kamien"]=0; moves["Nozyce"]=1; moves["Papier"]=2;
	moves["Piedra"]=0; moves["Tijera"]=1; moves["Papel"]=2;
	int game;
	string s,s1,s2;
	string name1,name2;
	int wins1,wins2;
	for (game=1;;game++)
	{
		cin >> s >> name1;
		cin >> s >> name2;
		wins1=0;
		wins2=0;
		for (;;)
		{
			cin >> s1;
			if (s1=="-")
				break;
			if (s1==".")
				break;
			cin >> s2;
			if (result(moves[s1],moves[s2])==-1)
				wins1++;
			else if (result(moves[s1],moves[s2])==1)
				wins2++;
		}
		cout << "Game #" << game << ":\n";
		cout << name1 << ": " << wins1 << " point";
		if (wins1!=1) cout << 's';
		cout << endl;
		cout << name2 << ": " << wins2 << " point";
		if (wins2!=1) cout << 's';
		cout << endl;
		if (wins1==wins2)
			cout << "TIED GAME\n";
		else if (wins1>wins2)
			cout << "WINNER: " << name1 << endl;
		else
			cout << "WINNER: " << name2 << endl;
		if (s1==".") break;
	}
	return 0;
}
