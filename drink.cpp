// 2008-04-21
#include <iostream>
using namespace std;
double round(double x)
{
	if (x<0) return (-round(-x));
	double x10=x*10.0;
	if (x10-int(x10)+1e-8>0.5)
		return (int(x10)+1.0)/10.0;
	else
		return int(x10)/10.0;
}
int main()
{
	double Mw,Mi,Tw,Ti,w,i,T;
	for(;;)
	{
		cin >> Mw >> Mi >> Tw >> Ti;
		if (Mw==0 && Mi==0)
			return 0;
		double E=Mw*(Tw*4.19+335.0+273.0*2.09)+Mi*((Ti+273.0)*2.09);
		//E is total thermal energy available
		//enough to melt all of the ice?
		double WaterAt273K=(Mw+Mi)*(335.0+273.0*2.09);
		double IceAt273K=(Mw+Mi)*(273.0*2.09);
		if (E>=WaterAt273K)
		{
			w=Mw+Mi;
			i=0;
			T=(E/(Mw+Mi)-335.0-273.0*2.09)/4.19;
		}
		else if (E<=IceAt273K)
		{
			w=0;
			i=Mw+Mi;
			T=E/(Mw+Mi)/2.09-273;
		}
		else
		{
			//some water, some ice
			w=(E-(Mw+Mi)*(273*2.09))/335;
			i=Mw+Mi-w;
			T=0;
		}
		printf("%.1lf g of ice and %.1lf g of water at %.1lf C\n",round(i),round(w),round(T));
	}
}
