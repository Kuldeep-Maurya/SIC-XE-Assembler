#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<cstdio>
#include<map>

using namespace std;

#define filename "max.txt"    //Mention the filename over here

#include "Op_Sym_Tab.cpp"
#include "Convert_Hexa.cpp"

string putzero(int dig,string a)	//This will put 0's wherever neccesary in the object code
{
    string temp="";
    for(int i=0;i<dig-a.length();++i)
        temp+='0';
    return temp+a;
}

bool isWhiteSpace(char a)   //To check if its white space or not
{
    if(a==' ')  
	return true;
	
    if(a=='\t') 
	return true;
	
    return false;
}

void extract(string a,string word[],int& count)    //Extract the next word
{
    int i;
    for(i=0;i<5;++i) word[i]="";
    count=0;
    i=0;
    while(isWhiteSpace(a[i])&&i<a.length()) 
	{
		++i;
		continue;
	}
	
    if(i==a.length()||a[i]=='.')   
	return;
	
    for(;i<a.length();)
    {
        while(isWhiteSpace(a[i])&&i<a.length()) 
		{
			++i;
			continue;
		}
        
		if(i==a.length()) 
		break;
        
		for(;!isWhiteSpace(a[i])&&i<a.length();++i) word[count]+=a[i];
        ++count;
    }
}

void execute(string[],int);

int line;
string pc,strt,len,end;
bool error_flag=0;
ifstream fin1;
ofstream fout1,error,fsym;

void run()               		//Acts as the main() function for Pass 1
{
    create();
    char ch;
    string s,word[5];
    int count=0;
    fsym.open("symtab.txt");
    fin1.open(filename);
    fout1.open("intermediate.txt");
    error.open("error.txt");
    line=5;
    getline(fin1,s);
    cout<<"\n\t\t\t\tTHE PROGRAM IS:\n\n";
    cout<<line<<"\t"<<s<<endl;
    extract(s,word,count);
    while(count==0)
    {
        fout1<<line<<endl;
        fout1<<"//";         
        fout1<<s<<endl;
        fout1<<""<<endl;
        fout1<<""<<endl;
        line+=5;
        cout<<line<<"\t"<<s<<endl; 
    }
    pc="0";
    line=5;
    if(word[0]=="START")
    {
        pc=word[1];
        strt=pc;
        fout1<<line<<endl;
        fout1<<""<<endl;
        fout1<<"START"<<endl;
        fout1<<pc<<endl;
        fout1<<pc<<endl;
        fout1<<""<<endl;
    }
    else if(word[1]=="START")
    {
        pc=word[2];
        strt=pc;
        fout1<<line<<endl;
        fout1<<word[0]<<endl;
        fout1<<"START"<<endl;
        fout1<<pc<<endl;
        fout1<<pc<<endl;
        fout1<<""<<endl;
    }
    else
        execute(word,count); 
		       
    while(true)
    {
        getline(fin1,s);
        extract(s,word,count);
        line+=5;
        cout<<line<<"\t"<<s<<endl;
        fout1<<line<<endl;
        if(count==0)
        {
            fout1<<"//"<<endl;
            fout1<<s<<endl;
            fout1<<""<<endl;
            fout1<<""<<endl;
            fout1<<""<<endl;
            continue;
        }
        if(word[0]=="END")
        {
            end=pc;
            len=toHex(toDec(end)-toDec(strt));
            fout1<<""<<endl;
            fout1<<word[0]<<endl;
            fout1<<word[1]<<endl;
            fout1<<pc<<endl;
            fout1<<""<<endl;
            break;
        }
        execute(word,count); 
     }
}

void execute(string word[],int count)  //Function to put locctr value
{
    if(word[0][0]=='+')
    {
        fout1<<""<<endl;
        fout1<<word[0]<<endl;
        fout1<<word[1]<<endl;
        fout1<<pc<<endl;
        pc=toHex(toDec(pc)+4);
        fout1<<pc<<endl;
        return;
    }
    if(OPTAB[word[0]].exist=='y')
    {
        fout1<<""<<endl;
        fout1<<word[0]<<endl;
        fout1<<word[1]<<endl;
        fout1<<pc<<endl;
        pc=toHex(toDec(pc)+OPTAB[word[0]].format);
        fout1<<pc<<endl;
        return;
    }
    if(OPTAB[word[0]].exist=='n')
    {
        if(SYMTAB[word[0]].exist=='y')
        {
            error<<"Line "<<line<<": Duplicate Symbol"<<endl;
            cout<<"Error encounter at Line no. "<<line<<": Duplicate Symbol"<<endl;
            error_flag=1;
        }
        else
        {
            SYMTAB[word[0]].address=pc;
            SYMTAB[word[0]].exist='y';
            fout1<<word[0]<<endl;
            fout1<<word[1]<<endl;
            fout1<<word[2]<<endl;
            fsym<<word[0]<<"\t"<<putzero(4,SYMTAB[word[0]].address)<<endl;  //SYMTAB genertor
            fout1<<pc<<endl;
            if(word[1][0]=='+')
                pc=toHex(toDec(pc)+4);
            else if(OPTAB[word[1]].exist=='y')
                pc=toHex(toDec(pc)+OPTAB[word[1]].format);
            else if(word[1]=="WORD")      pc=toHex(toDec(pc)+3);
            else if(word[1]=="RESW")      pc=toHex(toDec(pc)+(atoi(word[2].c_str())*3));
            else if(word[1]=="RESB")      pc=toHex(toDec(pc)+atoi(word[2].c_str()));
            else if(word[1]=="BYTE")
            {
                 int len=word[2].length()-3;
                 if(word[2][0]=='X') len/=2;
                 pc=toHex(toDec(pc)+len);
            }
            else
            {
                error<<"Line "<<line<<": Opcode not found"<<endl;
                cout<<"Error encounter at Line no. "<<line<<": Opcode not found"<<endl;
                error_flag=1;
            }
            fout1<<pc<<endl;
        }
    }
}
