// 2010-11-13
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <map>
#include <string>
using namespace std;
int main()
{
	int T,i,j,k;
	cin >> T;
	map<char,int> rankrank;
	map<char,int> suitsuit;
	for (i=2; i<10; i++)
		rankrank[i+'0']=i-2;
	rankrank['T']=8;
	rankrank['J']=9;
	rankrank['Q']=10;
	rankrank['K']=11;
	rankrank['A']=12;
	suitsuit['D']=0;
	suitsuit['C']=1;
	suitsuit['H']=2;
	suitsuit['S']=3;
	while (T--)
	{
		int rankcount[13]={0};
		int suitcount[4]={0};
		for (i=0; i<5; i++)
		{
			string s;
			cin >> s;
			rankcount[rankrank[s[0]]]++;
			suitcount[suitsuit[s[1]]]++;
		}
		//royal flush?
		bool b=false;
		for (i=0; i<4; i++)
			if (suitcount[i]==5)
			{
				bool ok=true;
				for (j=rankrank['T']; j<=rankrank['A']; j++)
					if (!rankcount[j]) ok=false;
				if (ok) b=true;
			}
		if (b)
		{
			printf("royal flush\n");
			continue;
		}
		//straight flush?
		for (i=0; i<4; i++)
			if (suitcount[i]==5)
				for (j=0; j<=10; j++)
				{
					bool ok=true;
					for (k=0; k<5; k++)
						if (!rankcount[(j+k+rankrank['A'])%13])
							ok=false;
					if (ok) b=true;
				}
		if (b)
		{
			printf("straight flush\n");
			continue;
		}
		//four of a kind?
		for (i=0; i<13; i++)
			if (rankcount[i]==4)
				b=true;
		if (b)
		{
			printf("four of a kind\n");
			continue;
		}
		//full house?
		for (i=0; i<13; i++)
			for (j=0; j<13; j++)
				if (rankcount[i]==3&&rankcount[j]==2)
					b=true;
		if (b)
		{
			printf("full house\n");
			continue;
		}
		//flush?
		for (i=0; i<4; i++)
			if (suitcount[i]==5)
				b=true;
		if (b)
		{
			printf("flush\n");
			continue;
		}
		//straight?
		for (j=0; j<=10; j++)
		{
			bool ok=true;
			for (k=0; k<5; k++)
				if (!rankcount[(j+k+rankrank['A'])%13])
					ok=false;
			if (ok) b=true;
		}
		if (b)
		{
			printf("straight\n");
			continue;
		}
		//3 of a kind?
		for (i=0; i<13; i++)
			if (rankcount[i]==3)
				b=true;
		if (b)
		{
			printf("three of a kind\n");
			continue;
		}
		//2 pairs
		for (i=0; i<13; i++)
			for (j=0; j<i; j++)
				if (rankcount[i]==2&&rankcount[j]==2)
					b=true;
		if (b)
		{
			printf("two pairs\n");
			continue;
		}
		//pair
		for (i=0; i<13; i++)
			if (rankcount[i]==2)
				b=true;
		if (b)
			printf("pair\n");
		else
			printf("high card\n");
	}
	return 0;
}

