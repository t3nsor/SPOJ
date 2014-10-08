// 2009-01-18
#include <iostream>
#include <cstring>
using namespace std;
struct node
{
	char c;
	node* l;
	node* r;
	void print()
	{
		if (c>='a')
			putchar(c);
		else if (c=='+')
		{
			l->print();
			putchar('+');
			r->print();
		}
		else if (c=='-') //
		{
			l->print();
			putchar('-');
			if (r->c=='+'||r->c=='-')
			{
				putchar('(');
				r->print();
				putchar(')');
			}
			else
				r->print();
		}
		else if (c=='*')
		{
			if (l->c=='+'||l->c=='-')
			{
				putchar('(');
				l->print();
				putchar(')');
			}
			else
				l->print();
			putchar('*');
			if (r->c=='+'||r->c=='-')
			{
				putchar('(');
				r->print();
				putchar(')');
			}
			else
				r->print();
		}
		else /* / */
		{
			if (l->c=='+'||l->c=='-')
			{
				putchar('(');
				l->print();
				putchar(')');
			}
			else
				l->print();
			putchar('/');
			if (r->c<'a')
			{
				putchar('(');
				r->print();
				putchar(')');
			}
			else
				r->print();
		}
	}
};
node pool[10000000];
int ptr=0;
node* stk1[300];
char stk2[300];
int sp1;
int sp2;
int prec[256]={0};
/*node* build(char* s,int len)
{
	if (len==1)
	{
		node* res=pool+ptr++;
		res->c=*s;
		return res;
	}
	int best=len-1;
	int i=len-1,l=0;
	do
	{
		if (s[i]==')') l++;
		else if (s[i]=='(') l--;
		if (l==0&&prec[s[i]]>prec[s[best]])
			best=i;
		i--;
	}
	while (i>=0);
	if (!prec[s[best]]) //brackets surrounding
		return build(s+1,len-2);
	else //operator found!
	{
		node* res=pool+ptr++;
		res->c=s[best];
		res->l=build(s,best);
		res->r=build(s+best+1,len-best-1);
		return res;
	}
}*/
int main()
{
	prec['+']=1;
	prec['-']=1;
	prec['*']=2;
	prec['/']=2;
	int T,i,j;
	char s[300];
	scanf("%d\n",&T);
	node* n;
	for (i=0; i<T; i++)
	{
		sp1=0;
		gets(s);
		//Dijkstra's shunting yard algorithm
		for (j=0; s[j]; j++)
		{
			if (s[j]>='a') //number
				(stk1[sp1++]=pool+ptr++)->c=s[j];
			else if (s[j]=='(')
				stk2[sp2++]='(';
			else if (s[j]==')')
				while (stk2[--sp2]!='(')
				{
					n=pool+ptr++;
					n->c=stk2[sp2];
					n->l=stk1[sp1-2];
					n->r=stk1[sp1-1];
					stk1[(--sp1)-1]=n;
				}
			else //operator!
			{
				while (sp2&&prec[stk2[sp2-1]]>=prec[s[j]])
				{
					n=pool+ptr++;
					n->c=stk2[--sp2];
					n->l=stk1[sp1-2];
					n->r=stk1[sp1-1];
					stk1[(--sp1)-1]=n;
				}
				stk2[sp2++]=s[j];
			}
		}
		while (sp2)
		{
			n=pool+ptr++;
			n->c=stk2[--sp2];
			n->l=stk1[sp1-2];
			n->r=stk1[sp1-1];
			stk1[(--sp1)-1]=n;
		}
		(*stk1)->print();
		puts("");
	}
	return 0;
}
