#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<cstdio>
#include<map>

using namespace std;

char toHexaVal(int);
int toDecVal(char);

string toHex(int a)
{
    string res="";
    if(a==0)
    {
    	res=res+'0';
    }
    while(a>0)
    {
        res=toHexaVal(a%16)+res;
        a=a/16;
    }
    return res;
}


int toDec(string val)
{
    int l=val.length()-1,curr=1,res=0;
    while(l>=0)
    {
        res+=(toDecVal(val[l])*curr);
        curr=curr*16;
        --l;
    }
    return res;
}

int toDecVal(char a)
{
    if(a=='0')
	 return 0;
	 
    if(a=='1') 
	return 1;
	
    if(a=='2') 
	return 2;
	
    if(a=='3') 
	return 3;
	
    if(a=='4') 
	return 4;
	
    if(a=='5') 
	return 5;
	
    if(a=='6')
	return 6;
	
    if(a=='7')
	return 7;
	
    if(a=='8') 
	return 8;
	
    if(a=='9') 
	return 9;
	
    if(a=='A')
	return 10;
	
    if(a=='B')
	return 11;
	
    if(a=='C') 
	return 12;
	
    if(a=='D') 
	return 13;
    
	if(a=='E') 
	return 14;
	
    if(a=='F') 
	return 15;
}


char toHexaVal(int a)
{
    if(a==0) 
	return '0';
	
    if(a==1) 
	return '1';
	
    if(a==2) 
	return '2';
	
    if(a==3) 
	return '3';
	
    if(a==4) 
	return '4';
    
	if(a==5) 
	return '5';
    
	if(a==6) 
	return '6';
	
    if(a==7) 
	return '7';
	
    if(a==8) 
	return '8';
	
    if(a==9) 
	return '9';
	
    if(a==10) 
	return 'A';
	
    if(a==11) 
	return 'B';
	
    if(a==12) 
	return 'C';
	
    if(a==13) 
	return 'D';
	
    if(a==14) 
	return 'E';
	
    if(a==15)
	return 'F';
}


