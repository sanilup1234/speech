
#include "stdafx.h"
#include "stdio.h"
#include "conio.h"
#include<string>
#include "stdlib.h"
#include<iostream>
using namespace std;

long double ar[65000];
int size = 0;
int size1 = 0;
int maxi=0;
long double E[200];
int ZCR[200];

//function to display value inside raw data array
void display()
{
	for(int i=0;i<size;i++)
	{
		printf("%Lf\n",ar[i]);
	}
}

//function to normalize array with parameter passed as DC Shift value
void normalize(double d)
{
	for(int i=0;i<size;i++)
	{
		ar[i] = ((ar[i]-d)/maxi)*5000;
	}
}

//function to calculate Short term Energy 
long double energy(int beg , int end)
{
	long double temp=0;
	for(int i=beg;i<=end;i++)
		temp = temp + ar[i]*ar[i];
	temp = (temp/(end-beg+1));

	return temp;
}
//function to calculate ZCR 
int zcr(int beg,int end)
{
	int count =0;
	int i=beg;
	while(i<=end)
	{
		if((ar[i]* ar[i+1])<0)
			count++;

		i++;
	}
	return count;
}
float train(char* fname)
{
	size=0;
	int beg=0,end=65000;
int s;
	FILE *fp;
	char ch;
	int num;
	char* file = fname;   // Opening File
	fp = fopen(file,"r");
	while((ch = getc(fp)) !=EOF){    //reading values from file character by character
	        int n = 0,m,s=1;
			while(ch != '\n')
			{
				if(ch=='-')
					s =-1;
				m  = atoi(&ch);   //converting character to integer values
				n = n*10 +m;
				ch = getc(fp);
			}
			n = n*s;
			ar[size++]= (long double)n;   //assigning values of raw data into global array
	if(abs(n)>maxi)
		maxi = abs(n);  //calculating maximum value inside array
	}
	fclose(fp);
	int sum=0;
	int p=0; 
	char*file1 = "C:\\Users\\sanil\\Desktop\\yesno\\silence.txt"; //opening ambient noise file
	FILE *fp1 = fopen(file1,"r");
	while((ch = getc(fp1)) !=EOF){
	        int n = 0,m,s=1;
			
			while(ch != '\n')
			{
				if(ch=='-')
					s =-1;
				m  = atoi(&ch);
				n = n*10 +m;
				ch = getc(fp1);
			}
			n = n*s;
			sum = sum +n;
			p++;
	}
	double shift = (double)sum/p;   // calculating DC shift using ambient noise
	
	fclose(fp1);
	normalize(shift);
	int zcr1;
	long double enrgy1;
	int i=0;
	while((i+1)*100<size)    //calculating Energy and ZCR for every frame of 100 size
	{
		zcr1 = zcr(i*100,(i*100)+99);
		enrgy1 = energy(i*100,(i*100)+99);
		E[i] = enrgy1;
		ZCR[i] = zcr1;
		
	i++;
	}
	long double avg_Energy=0;
	float avg_ZCR =0;
	for(int s=0;s<i;s++)  // Calculating Average Energy and Average ZCR 
	{
		avg_Energy += E[s];
		
	}
	avg_Energy  /= i;
	
	int flags=1,flage=1;
	for(int s=0;s<i;s++)  // Calculating Average Energy and Average ZCR 
	{
		if((E[s] > 1.05*avg_Energy)&&(flags))
		{
			beg=s*100;
			flags=0;
		}
		if((E[i-s]>.1*avg_Energy)&&(flage))
		{
			end = (i-s)*100;
			flage=0;
		}
	}
	//cout<<"start== "<<beg<<"  end=="<<end<<endl;
	for(int s=beg/100;s<end/100;s++)  // Calculating Average Energy and Average ZCR 
	{
		avg_ZCR += ZCR[s];
	}

	avg_ZCR /=((end/100)-(beg/100)) ;
	return avg_ZCR;
}

int _tmain(int argc, _TCHAR* argv[])
{ 
	float zcr;
	float miny = 1000,maxy= 0;
	float minn = 1000,maxn= 0;
	int t;
	char ch;
	char fname[100];
	cout<<"************************************************"<<endl;
	cout<<"::Training Module(YES)::";
    cout<<endl;
	string name;
	long double num=1;
	while(num<11){      //training YES by 10 files
		name.append("y");
		name += to_string(num);
		name += ".txt";
		 strcpy(fname, name.c_str()); 
	zcr = train(fname); 
	if(zcr<miny)
		miny = zcr;
	if(zcr>maxy )
		maxy = zcr;
	num++;
	name.erase();
	}
	//	cout<<miny<<"  "<<maxy;
	cout<<endl;
   cout<<"************************************************"<<endl;
   
   cout<<"::Training Module(NO)::";
    num=1;
	while(num<11){      //training NO by 10 files
		name.append("n");
		name += to_string(num);
		name += ".txt";
		 strcpy(fname, name.c_str()); 
	zcr = train(fname); 
	
	if(zcr<minn)
		minn = zcr;
	if(zcr>maxn )
		maxn = zcr;
	num++;
	name.erase();
	}
	//cout<<minn<<"  "<<maxn;
	cout<<endl;
	cout<<"************************************************"<<endl;
	cout<<"Training completed";
	cout<<endl;
	cout<<"************************************************"<<endl;
   cout<<":::Testing Module:::";
   cout<<endl;
   int x;
   cout<<"how many files you want test";
   cin>>x;
   string record;
   long double i=0;
   while(i<x)
	{
		record.append("Recording_Module.exe 3 input");
		record += to_string(i);
		record.append(".wav input");
			record += to_string(i);
        record.append(".txt");
		strcpy(fname, record.c_str()); 
		system(fname);      //call to recording module 
		record.erase();
		record.append("input");
		record += to_string(i);
		
        record.append(".txt");
	strcpy(fname, record.c_str()); 
	zcr = train(fname); 
	cout<<zcr;
	cout<<endl;
	if(zcr>=miny)
		cout<<"YES\n";
	else if(zcr<= maxn)
		cout<<"NO\n";
	else
		cout<<"NON SPEECH \n";
	i++;
	record.erase();
   }
	getch();
	return 0;
}

