// durbin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
#include<conio.h>
#include<fstream>
using namespace std;
int size=0;
long double arr[321];  //initialization of raw data array of frequencies

void durbin(long double r[],long double a[],int p)  //durbin module for calculating Ai's
{

	long double E[13],K[13],ar[13][13],midsum;
	E[0]=r[0];
	for(int i=1;i<=p;i++){
							midsum=0;
							for(int j=1;j<=i-1;j++)
											midsum=midsum+(ar[j][i-1]*r[i-j]);
							if(i==1) midsum=0;
							K[i]=(r[i]-midsum)/E[i-1];
							ar[i][i]=K[i];
							for(int j=1;j<=i-1;j++){
											ar[j][i]=ar[j][i-1]-(K[i]*ar[i-j][i-1]);
												   }
							E[i]=(1-(K[i]*K[i]))*E[i-1];
						}


	for(int i=1;i<=p;i++){
							a[i]=ar[i][p];

						}
}
void calc_cepstral(long double a[],long double c[],int p,long double frst) //module to calculate Ci's
{
	c[0]= logl(frst); //initial cepstral coefficent
	for(int m=1;m<=p;m++)
	{
		long double sum=0;
		for(int j=1;j<=m-1;j++) //calculate the sum from older cepstral coefficents to compute new cepstral coefficients
			sum+=(j*c[j]*a[m-j])/m;
		c[m]=a[m]+sum; //new cepstral coefficients
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	int p=12;
	int i=0;
long double r[13],a[13],c[13];
for(i=0;i<13;i++)
{
r[i]=0;
}
i=0;
long double n;
ifstream raw;
	raw.open("test.txt");
	while(!raw.eof())
	{
		raw >> n;             //reads in the double value
		arr[size++]= (long double)n; //put the sample values to array
		i++;
	}
		raw.close();
		ofstream dump;
		dump.open("ri.txt");
   for(i=0;i<13;i++)
   {
	   for(int j=0;j<320;j++)
	   {
		   if((j+i)<320)
		   r[i] += arr[j]*arr[j+i];
	   }
	  dump<<r[i]<<endl;
   }
   dump.close();
   dump.open("Ai.txt");
durbin(r,a,p); //call to durbin module
for(int i=1;i<=p;i++){
dump<<a[i]<<endl;        //Putting values of Ai's into Ai.txt file
}
dump.close();
calc_cepstral(a,c,p,r[0]); // call to calculate cepstral coefficients
dump.open("ci.txt");
for(int i=1;i<=p;i++){
dump<<c[i]<<endl;          //Putting values of Ci's into ci.txt file

}
dump.close();
cout<<"CHECK Files \n1.'ri.txt' for values of Ri's\n2.'Ai.txt' for values of Ai's\n2.'ci.txt' for values of Ci's\n ";
	getch();
	return 0;
}

