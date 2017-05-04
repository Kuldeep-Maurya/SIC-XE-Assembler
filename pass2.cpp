#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<cstdio>
#include<map>
#include<climits>

using namespace std;

#include "pass1.cpp"

ofstream obj,lst,mod;
ifstream intermd,obj_r,sym_r;
bool imm,ind;


void modify_obj_file()		//To write modification records
{
   ifstream fin;
   fin.open("modification.txt");
   string s;
   while(true)
   {
       getline(fin,s);
       if(s=="") break;
       obj<<s<<endl;
   }
   
}

void input(string a[])		//Function to get next word for intermediate
{
    int i;
    for(i=0;i<6;++i)
        getline(intermd,a[i]);
}

void assemble(string[]);
string gen_code(string[]);
string text_s="",text_e="";
int text_length=0,base=INT_MAX;
		

void assemble(string a[])		//This will write object code for each instruction to the listing file
{
    string obj_code;
    string loc_ctr;
    int format;
    if(a[2]=="RESB"||a[2]=="RESW")
    {
        lst<<a[0]<<"\t"<<putzero(4,a[4])<<"  "<<"\t\t"<<a[1]<<"\t"<<a[2]<<"\t\t"<<a[3]<<endl;
        if(text_length>0) obj<<text_s<<"^"<<putzero(2,toHex(text_length/2))<<text_e<<endl;
        text_s="";
        text_e="";
        text_length=0;
        return;
    }
    imm=ind=false;
    obj_code=gen_code(a);
    if(a[2]=="BYTE"||a[2]=="WORD")
        lst<<a[0]<<"\t"<<putzero(4,a[4])<<"  "<<"\t\t"<<a[1]<<"\t"<<a[2]<<"\t\t"<<a[3]<<endl;
    else
    {
        if(imm)      
			lst<<a[0]<<"\t"<<putzero(4,a[4])<<"  "<<"\t\t"<<a[1]<<"\t\t"<<a[2]<<"\t\t#"<<a[3]<<"\t\t"<<obj_code<<endl;
        else if(ind) 
			lst<<a[0]<<"\t"<<putzero(4,a[4])<<"  "<<"\t\t"<<a[1]<<"\t\t"<<a[2]<<"\t\t@"<<a[3]<<"\t\t"<<obj_code<<endl;
        else         
			lst<<a[0]<<"\t"<<putzero(4,a[4])<<"  "<<"\t\t"<<a[1]<<"\t\t"<<a[2]<<"\t\t"<<a[3]<<"\t\t"<<obj_code<<endl;
    }
    if(text_s=="")
    {
        loc_ctr=a[4];
        text_s="T^"+putzero(6,loc_ctr);
        text_e="^"+obj_code;
        text_length=obj_code.length();
    }
    else if(text_length+obj_code.length()>60)
    {
        obj<<text_s<<"^"<<putzero(2,toHex(text_length/2))<<text_e<<endl;
        loc_ctr=a[4];
        text_s="T^"+putzero(6,loc_ctr);
        text_e="^"+obj_code;
        text_length=obj_code.length();
    }
    else    {
        text_e+="^"+obj_code;
        text_length+=obj_code.length();
    }
    if(a[2]=="LDB")
    {
        base=toDec(SYMTAB[a[3]].address);
    }
}

string gen_code(string a[])		//Function to generate objectcode for BYTE,WORD,RSUB and other formats
{
    string ob1,ob2,ob3;
    string operand_addr,prgm_ctr;
    int format;
    if(a[2]=="BYTE")
    {
        int i;
        ob1="";
        if(a[3][0]=='X')
            for(i=2;i<a[3].length()-1;++i) ob1+=a[3][i];
        else 
            for(i=2;i<a[3].length()-1;++i)
                ob1+=toHex((int)a[3][i]);
        return ob1;
    }
    if(a[2]=="WORD")
    {
        ob1=toHex(atoi(a[3].c_str()));
        return ob1;
    }
    if(a[2]=="RSUB")
    {
        ob1="4F0000";
        return ob1;
    }
    if(a[2]=="+RSUB")
    {
        ob1="4F000000";
        return ob1;
    }
    if(a[2][0]=='+')
    {
        format=4;
        a[2]=a[2].substr(1);
    }
    else
        format=OPTAB[a[2]].format;
    if(format==1)
    {
        ob1=OPTAB[a[2]].opcode;
        return ob1;
    }
    if(format==2)
    {
        ob1=OPTAB[a[2]].opcode;
        if(a[3].length()==3)
        {
            ob2=toHex(reg_num(a[3][0]));
            if(isdigit(a[3][2]))  ob2=ob2+toHexaVal(a[3][2]-1);
            else
            {
                ob2=ob2+toHexaVal(reg_num(a[3][2]));
            }
        }
        else  
	     {
            if(isdigit(a[3][0]))
            {
                ob2=toHex(atoi(a[3].c_str()))+"0";
            }
            else
            {
                ob2=toHex(reg_num(a[3][0]))+"0";
            }
        }
        return (ob1+putzero(2,ob2));
    }
    if(format==3)
    {
        ob1=OPTAB[a[2]].opcode;
        if(a[3][0]=='#')
        {
            imm=true;
            ob1=toHex(toDec(ob1)+1);
            a[3]=a[3].substr(1);
            if(isdigit(a[3][0]))
            {
                ob2="0";
                ob3=toHex(atoi(a[3].c_str()));
                return putzero(2,ob1)+ob2+putzero(3,ob3);
            }    
        }
        else if(a[3][0]=='@')
        {
            ind=true;
            ob1=toHex(toDec(ob1)+2);
            a[3]=a[3].substr(1);
        }
        else
            ob1=toHex(toDec(ob1)+3);
        ob2="0";
        bool x=false;
        if(a[3][a[3].length()-2]==',')
        {
            x=true;
            ob2=toHex(toDec(ob2)+8);
            a[3]=a[3].substr(0,a[3].length()-2);
        }
       
	    prgm_ctr=a[5];
        operand_addr=SYMTAB[a[3]].address;
        if(x) a[3]+=",X";
        int disp=toDec(operand_addr)-toDec(prgm_ctr);
        if(disp>=-2048 && disp<2048)
        {
            ob2=toHex(toDec(ob2)+2);
            if(disp<0) 
				disp+=4096;
            ob3=toHex(disp);
            return putzero(2,ob1)+putzero(1,ob2)+putzero(3,ob3);
        }
        disp=toDec(operand_addr)-base;
        if(disp>=-2048 && disp<2048)
        {
            ob2=toHex(toDec(ob2)+4);
            if(disp<0) 
				disp+=4096;
            ob3=toHex(disp);
            return putzero(2,ob1)+putzero(1,ob2)+putzero(3,ob3);
        }
    }
    if(format==4)
    {
        ob1=OPTAB[a[2]].opcode;
        if(a[3][0]=='#')
        {
            imm=true;
            ob1=toHex(toDec(ob1)+1);
            a[3]=a[3].substr(1);
            if(isdigit(a[3][0]))
            {
                ob2="0";
                ob3=toHex(atoi(a[3].c_str()));
                a[2]="+"+a[2];
                return ob1+ob2+putzero(5,ob3);
            }
        }
        else if(a[3][0]=='@')
        {
            ind=true;
            ob1=toHex(toDec(ob1)+2);
            a[3]=a[3].substr(1);
        }
        else
            ob1=toHex(toDec(ob1)+3);
        bool x=false;
        ob2="1";
        if(a[3][a[3].length()-2]==',')
        {
            x=true;
            ob2=toHex(toDec(ob2)+8);
            a[3]=a[3].substr(0,a[3].length()-2);
        }
        operand_addr=SYMTAB[a[3]].address;
        if(x) 
		a[3]+=",X";
        ob3=operand_addr;
        a[2]="+"+a[2];
        string loc_ctr=toHex(toDec(a[4])+1);
        mod<<"M^"<<putzero(6,loc_ctr)<<"^05"<<endl;
        return putzero(2,ob1)+putzero(1,ob2)+putzero(5,ob3);
    }
}


int main()
{
    run();
    string a[6];
    char ch;
    string start;
    if(error_flag)
    {
        cout<<"ERRORS found"<<endl<<"Check above program to know more!!"<<endl;
        exit(1);
    }
    
    sym_r.open("symtab.txt");
    cout<<"\n\n\t\tSYMTAB is :\n\n";
        while(sym_r)
        {
        	sym_r.get(ch);
        	cout<<ch;
		}
    intermd.open("intermediate.txt");
    obj.open("objectcode.txt");
    lst.open("list.txt");
    mod.open("modification.txt");
    lst<<"----------------------------------------------------------------"<<endl<<endl;
    input(a);

    while(a[1]=="//")
    {
        lst<<a[0]<<"\t\t "<<a[2]<<endl;
        input(a);
    }
    if(a[2]=="START")
    {
        lst<<a[0]<<"\t"<<putzero(4,a[4])<<"  "<<"\t\t"<<a[1]<<"\t\t"<<a[2]<<"\t\t"<<a[3]<<endl;
        obj<<"H^";
        int i;
        for(i=0;i<a[1].length();++i)
            obj<<a[1][i];
        for(;i<6;++i)
            obj<<" ";
        obj<<"^"<<putzero(6,a[3])<<"^"<<putzero(6,len)<<endl;
        start=a[3];
    }
    else
    {
        error_flag=1;
        error<<"Intermediate File has NO START!"<<endl;
        cout<<"ERROR!!!Intermediate File has NO START!"<<endl;
    }
    while(true)
    {
        input(a);
        if(a[1]=="//")
        {
            lst<<a[0]<<"\t\t "<<a[2]<<endl;
            continue;
        }
        if(a[2]=="END")
        {
            lst<<a[0]<<"\t\t\t\t\t\t"<<a[2]<<"\t\t"<<a[3]<<endl;
            if(text_length>0)
            {
                obj<<text_s<<"^"<<putzero(2,toHex(text_length/2))<<text_e<<endl;
                cout<<"!!"<<endl;
            }
            text_length=0;
            text_s="";
            text_e="";
            modify_obj_file();
            obj<<"E^"<<putzero(6,start)<<endl;
            break;
        }
       
        assemble(a);
       
    }
    if(error_flag)
    {
        cout<<"ERRORS found!!Object code not made"<<endl<<"Read the ERROR fileto know more!"<<endl;
    }
    else
    {
    	obj_r.open("objectcode.txt");
        cout<<"\n\n\t\t\t\tOBJECT CODE is :\n\n";
        while(obj_r)
        {
        	obj_r.get(ch);
        	cout<<ch;
		}
    }
}

