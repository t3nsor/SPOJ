// 2008-06-13
#include <cstdio>
#ifdef _MSC_VER
#define GC getchar
#define PC putchar
#else
#define GC getchar_unlocked
#define PC putchar_unlocked
#endif
char a[30000];
int input()
{
	int x=0;
	char c;
	for(;;)
	{
		c=GC();
		if (c==' '||c=='\n')
			return x;
		x=10*x+c-'0';
	}
}
struct BracketTreeNode
{
	int l;
	int u;
	BracketTreeNode* L;
	BracketTreeNode* R;
	int _l;
	int _r;
	BracketTreeNode(int ll,int uu):l(ll),u(uu),L(0),R(0)
	{
		if (uu>ll)
		{
			L=new BracketTreeNode(ll,(ll+uu)/2);
			R=new BracketTreeNode((ll+uu)/2+1,uu);
		}
	}
};
struct BracketTree
{
	BracketTreeNode* root;
	int length;
	bool valid()
	{
		if (root->_l||root->_r)
			return false;
		else
			return true;
	}
	BracketTree(int l):length(l)
	{
		root=new BracketTreeNode(0,l-1);
	}
	void ins(BracketTreeNode* p,int x,int _l,int _r)
	{
		if (p->l==p->u)
		{
			p->_l=_l;
			p->_r=_r;
		}
		else
		{
			if (x<=(p->l+p->u)/2)
				ins(p->L,x,_l,_r);
			else
				ins(p->R,x,_l,_r);
			if (p->L->_r>p->R->_l)
			{
				p->_l=p->L->_l;
				p->_r=p->R->_r+p->L->_r-p->R->_l;
			}
			else
			{
				p->_r=p->R->_r;
				p->_l=p->L->_l+p->R->_l-p->L->_r;
			}
		}
	}
	void init()
	{
		int i;
		for (i=0; i<length; i++)
			if (GC()=='(')
			{
				ins(root,i,0,1);
				a[i]='(';
			}
			else
			{
				ins(root,i,1,0);
				a[i]=')';
			}
		GC();
	}
	bool OK()
	{
		if (root->_l||root->_r)
			return false;
		else
			return true;
	}
};
int main()
{
	int i,n,m,j,k;
	for (i=0; i<10; i++)
	{
		printf("Test %d:\n",i+1);
		n=input();
		BracketTree BT(n);
		BT.init();
		m=input();
		for (j=0; j<m; j++)
		{
			k=input();
			if (k==0)
				if (BT.OK())
				{
					PC('Y');PC('E');PC('S');PC('\n');
				}
				else
				{
					PC('N');PC('O');PC('\n');
				}
			else
				if (a[k-1]=='(')
				{
					BT.ins(BT.root,k-1,1,0);
					a[k-1]=')';
				}
				else
				{
					BT.ins(BT.root,k-1,0,1);
					a[k-1]='(';
				}
		}
	}
	return 0;
}
