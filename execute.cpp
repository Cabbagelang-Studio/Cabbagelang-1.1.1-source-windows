#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <windows.h>
#include <unistd.h> 
#include <stdlib.h>
#include <time.h>
using namespace std;
void python_run(string path){//Run Python API. 
	string cabbage_home;
	TCHAR file_name[MAX_PATH];
	GetModuleFileName(NULL,file_name,MAX_PATH);
	cabbage_home=file_name;
	cabbage_home=cabbage_home.substr(0,cabbage_home.length()-11);
	string cmd=cabbage_home+"python.exe "+path;
	system(cmd.data()); 
}
inline bool is_char(string expr){//Is it character?
	return (expr.substr(0,1)=="\"" and expr.substr(expr.length()-1,expr.length())=="\"");
}bool is_number(string expr){//Is it number?
	stringstream turn;
	turn<<expr;
	int o;
	turn>>o;
	if (o!=0){
		return true;
	}else if (expr=="0"){
		return true;
	}else{
		return false;
	}
}inline bool is_bool(string expr){//Is it bool?
	return (expr=="true" or expr=="false");
}inline bool is_stmt(string expr){//Is it statement?
	return (expr=="kin");
}bool is_value(string expr){//Is it a value?
	ifstream file;
	expr=expr+".val";
	file.open(expr.data());
	bool result=file.is_open();
	file.close();
	return result;
}void redef(string name,string value){//Redefine value.
	name=name+".val";
	ofstream of;
	of.open(name.data());
	of<<value;
	of.close();
}
int execute(string path){//Execute function.
	path=path+"f";
	ifstream file;
	file.open(path.data());
	string line;
	int line_number=0,to_while=0;
	bool if_enter=false,is_if=false,is_while=false,while_enter=false,trying=false;
	while (getline(file,line)){
		line_number++;
		try{
			if (!is_if or if_enter){
				if (line.length() > 7 and line.substr(0,7)=="output "){
					if (is_char(line.substr(7))){
						string expr=line.substr(8,line.substr(7).length()-2);
						for (int i=0;i<expr.length();i++){
							if (expr.substr(i,2)=="\\n"){
								cout<<endl;
								i++;
							}else if (expr.substr(i,2)=="\\t"){
								cout<<"\t";
								i++;
							}else{
								if (expr.substr(i,1)=="\\"){
									i++;
								}
								cout<<expr.substr(i,1);
							}
						}
					}else if (is_stmt(line.substr(7))){
						if (line.substr(7)=="kin"){
							string kin;
							getline(cin,kin);
							cout<<kin;
						}
					}else if(is_value(line.substr(7))){
						ifstream in;
						string name=line.substr(7)+".val";
						in.open(name.data());
						string expr;
						getline(in,expr);
						in.close();
						if (is_char(expr)){
							string exp=expr.substr(1,expr.length()-2);
							for (int i=0;i<exp.length();i++){
								if (exp.substr(i,2)=="\\n"){
									cout<<endl;
									i++;
								}else if (exp.substr(i,2)=="\\t"){
									cout<<"\t";
									i++;
								}else{
									cout<<exp.substr(i,1);
								}
							}
						}else if (is_stmt(expr)){
							if (expr=="kin"){
								string kin;
								getline(cin,kin);
								cout<<kin;
							}
						}else if (is_number(expr) or is_bool(expr)){
							cout<<expr;
						}else{
							cout<<"nil";
						}
					}else if (is_number(line.substr(7))){
						cout<<line.substr(7);
					}else{
						cout<<"nil";
					}
				}if (line.length() > 6 and line.substr(0,6)=="redef "){
					string expr=line.substr(6);
					string name="",value="";
					bool given=false;
					for (int i=0;i<expr.length();i++){
						if (expr.substr(i,1)=="="){
							given=true;
						}else if (given){
							value=value+expr.substr(i,1);
						}else{
							name=name+expr.substr(i,1);
						}
					}
					if (is_stmt(value)){
						if (value=="kin"){
							getline(cin,value);
							value="\""+value+"\"";
						}
					}else if (is_value(value)){
						value=value+".val";
						ifstream in(value.data());
						getline(in,value);
						in.close();
					}else if (is_number(value) or is_bool(value) or is_char(value)){
					}else{
						value="nil";
					}
					redef(name,value);
				}if (line.length() > 7 and line.substr(0,7)=="number "){
					line=line+".val";
					ifstream v;
					v.open(line.substr(7).data());
					string content;
					v>>content;
					v.close();
					content=content.substr(1,content.length()-2);
					stringstream turn;
					turn<<content;
					double o;
					turn>>o;
					ofstream of;
					of.open(line.substr(7).data());
					of<<o;
					of.close();
				}if (line.length() > 5 and line.substr(0,5)=="char "){
					line=line+".val";
					ifstream v;
					v.open(line.substr(5).data());
					string content;
					v>>content;
					v.close();
					content="\""+content+"\"";
					ofstream of;
					of.open(line.substr(5).data());
					of<<content;
					of.close();
				}if (line.length() > 5 and line.substr(0,5)=="bool "){
					line=line+".val";
					ifstream v;
					v.open(line.substr(5).data());
					string content;
					v>>content;
					v.close();
					if (content=="0" or content=="nil"){
						content="false";
					}else{
						content="true";
					}
					ofstream of;
					of.open(line.substr(5).data());
					of<<content;
					of.close();
				}if (line.length() > 7 and line.substr(0,7)=="return "){
					stringstream turn;
					turn<<line.substr(7);
					int returnResult;
					turn>>returnResult;
					file.close();
					return returnResult;
				}if (line.length() > 7 and line.substr(0,7)=="system "){
					if (is_value(line.substr(7))){
						string p=line.substr(7)+".val";
						ifstream in(p.data());
						string content;
						getline(in,content);
						in.close();
						content=content.substr(1,content.length()-2);
						if(content.length()>3 and content.substr(0,3)=="cd "){
							chdir(content.substr(3).data());
						}else
							system(content.data());
					}else{
						string content=line.substr(7);
						content=content.substr(1,content.length()-2);
						if(content.length()>3 and content.substr(0,3)=="cd "){
							chdir(content.substr(3).data());
						}else
							system(content.data());
					}
				}else if (line.length() > 8 and line.substr(0,8)=="connect "){
					string first="",now="",result="";
					string expr=line.substr(8);
					for (int i=0;i<expr.length();i++){
						if (expr.substr(i,1)=="+" and expr.substr(i-1,1)!="\\"){
							if (first==""){
								first=now;
							}if (is_value(now)){
								now=now+".val";
								ifstream in(now.data());
								getline(in,now);
								in.close();
								now=now.substr(1,now.length()-2); 
							}else{
								now=now.substr(1,now.length()-2);
							} 
							result=result+now;
							now="";
						}else{
							now=now+expr.substr(i,1);
						}
					}if (first==""){
						first=now;
					}if (is_value(now)){
						now=now+".val";
						ifstream in(now.data());
						getline(in,now);
						in.close();
						now=now.substr(1,now.length()-2); 
					}else{
						now=now.substr(1,now.length()-2);
					}result=result+now;
					result="\""+result+"\"";
					redef(first,result);
				}else if (line.length() > 6 and line.substr(0,6)=="count "){
					string first="",now="",sign="";
					double item,result;
					stringstream turn;
					string expr=line.substr(6);
					expr=expr+"#";
					for (int i=0;i<expr.length();i++){
						if (expr.substr(i,1)=="+"){
							if (sign==""){
								first=now;
								sign="+";
								now=now+".val";
								ifstream in(now.data());
								in>>now;
								turn<<now;
								turn>>result;
							}else{
								if (is_value(now)){
									now=now+".val";
									ifstream in(now.data());
									in>>now;
									turn<<now;
									turn>>item;
									in.close();
								}else{
									turn<<now;
									turn>>item;
								}if (sign=="+"){
									result=result+item;
								}if (sign=="-"){
									result=result-item;
								}if (sign=="*"){
									result=result*item;
								}if (sign=="/"){
									result=result/item;
								}if (sign=="^"){
									result=pow(result,item);
								}if (sign=="%"){
									int a=(int)result;
									int b=(int)item;
									result=a%b;
								}
								sign="+";
							}now="";
						}else if (expr.substr(i,1)=="-"){
							if (sign==""){
								first=now;
								sign="-";
								if (is_value(now)){
									now=now+".val";
									ifstream in(now.data());
									in>>now;
									turn<<now;
									turn>>result;
									in.close();
								}else{
									turn<<now;
									turn>>result;
								}
							}else{
								if (is_value(now)){
									now=now+".val";
									ifstream in(now.data());
									in>>now;
									turn<<now;
									turn>>item;
									in.close();
								}else{
									turn<<now;
									turn>>item;
								}if (sign=="+"){
									result=result+item;
								}if (sign=="-"){
									result=result-item;
								}if (sign=="*"){
									result=result*item;
								}if (sign=="/"){
									result=result/item;
								}if (sign=="^"){
									result=pow(result,item);
								}if (sign=="%"){
									int a=(int)result;
									int b=(int)item;
									result=a%b;
								}
								sign="-";
							}now="";
						}else if (expr.substr(i,1)=="*"){
							if (sign==""){
								first=now;
								sign="*";
								if (is_value(now)){
									now=now+".val";
									ifstream in(now.data());
									in>>now;
									turn<<now;
									turn>>result;
									in.close();
								}else{
									turn<<now;
									turn>>result;
								}
							}else{
								if (is_value(now)){
									now=now+".val";
									ifstream in(now.data());
									in>>now;
									turn<<now;
									turn>>item;
									in.close();
								}else{
									turn<<now;
									turn>>item;
								}if (sign=="+"){
									result=result+item;
								}if (sign=="-"){
									result=result-item;
								}if (sign=="*"){
									result=result*item;
								}if (sign=="/"){
									result=result/item;
								}if (sign=="^"){
									result=pow(result,item);
								}if (sign=="%"){
									int a=(int)result;
									int b=(int)item;
									result=a%b;
								}
								sign="*";
							}now="";
						}else if (expr.substr(i,1)=="/"){
							if (sign==""){
								first=now;
								sign="/";
								if (is_value(now)){
									now=now+".val";
									ifstream in(now.data());
									in>>now;
									turn<<now;
									turn>>result;
									in.close();
								}else{
									turn<<now;
									turn>>result;
								}
							}else{
								if (is_value(now)){
									now=now+".val";
									ifstream in(now.data());
									in>>now;
									turn<<now;
									turn>>item;
									in.close();
								}else{
									turn<<now;
									turn>>item;
								}if (sign=="+"){
									result=result+item;
								}if (sign=="-"){
									result=result-item;
								}if (sign=="*"){
									result=result*item;
								}if (sign=="/"){
									result=result/item;
								}if (sign=="^"){
									result=pow(result,item);
								}if (sign=="%"){
									int a=(int)result;
									int b=(int)item;
									result=a%b;
								}
								sign="/";
							}now="";
						}else if (expr.substr(i,1)=="^"){
							if (sign==""){
								first=now;
								sign="^";
								if (is_value(now)){
									now=now+".val";
									ifstream in(now.data());
									in>>now;
									turn<<now;
									turn>>result;
									in.close();
								}else{
									turn<<now;
									turn>>result;
								}
							}else{
								if (is_value(now)){
									now=now+".val";
									ifstream in(now.data());
									in>>now;
									turn<<now;
									turn>>item;
									in.close();
								}else{
									turn<<now;
									turn>>item;
								}if (sign=="+"){
									result=result+item;
								}if (sign=="-"){
									result=result-item;
								}if (sign=="*"){
									result=result*item;
								}if (sign=="/"){
									result=result/item;
								}if (sign=="^"){
									result=pow(result,item);
								}if (sign=="%"){
									int a=(int)result;
									int b=(int)item;
									result=a%b;
								}
								sign="^";
							}now="";
						}else if (expr.substr(i,1)=="%"){
							if (sign==""){
								first=now;
								sign="%";
								if (is_value(now)){
									now=now+".val";
									ifstream in(now.data());
									in>>now;
									turn<<now;
									turn>>result;
									in.close();
								}else{
									turn<<now;
									turn>>result;
								}
							}else{
								if (is_value(now)){
									now=now+".val";
									ifstream in(now.data());
									in>>now;
									turn<<now;
									turn>>item;
									in.close();
								}else{
									turn<<now;
									turn>>item;
								}if (sign=="+"){
									result=result+item;
								}if (sign=="-"){
									result=result-item;
								}if (sign=="*"){
									result=result*item;
								}if (sign=="/"){
									result=result/item;
								}if (sign=="^"){
									result=pow(result,item);
								}if (sign=="%"){
									int a=(int)result;
									int b=(int)item;
									result=a%b;
								}
								sign="%";
							}now="";
						}else if (expr.substr(i,1)=="#"){
							if (is_value(now)){
								now=now+".val";
								ifstream in(now.data());
								in>>now;
								turn<<now;
								turn>>item;
								in.close();
							}else{
								turn<<now;
								turn>>item;
							}
							if (sign=="+"){
								result=result+item;
							}if (sign=="-"){
								result=result-item;
							}if (sign=="*"){
								result=result*item;
							}if (sign=="/"){
								result=result/item;
							}if (sign=="^"){
								result=pow(result,item);
							}if (sign=="%"){
									int a=(int)result;
									int b=(int)item;
									result=a%b;
								}
							sign="#";
						}else{
							now=now+expr.substr(i,1);
						}turn.clear();
					}string new_result;
					turn<<result;
					turn>>new_result;
					redef(first,new_result);
				}
			}if (line.length() > 3 and line.substr(0,3)=="if "){
				is_if=true;
				if (is_value(line.substr(3))){
					string val=line.substr(3)+".val";
					ifstream in(val.data());
					string expr;
					in>>expr;
					in.close();
					if (expr=="false" or expr=="nil" or expr=="0"){
						if_enter=false;
					}else{
						if_enter=true;
					}
				}else{
					string expr=line.substr(3);
					string first="",second="";
					bool same=false,bigger=false,smaller=false;
					for (int i=0;i<expr.length();i++){
						if (expr.substr(i,1)=="="){
							same=true;
						}else if (expr.substr(i,1)==">"){
							bigger=true;
						}else if (expr.substr(i,1)=="<"){
							smaller=true;
						}else{
							if (!same and !bigger and !smaller){
								first=first+expr.substr(i,1);
							}else{
								second=second+expr.substr(i,1);
							}
						}
					}
					if (same){
						ifstream in;
						first=first+".val";
						second=second+".val";
						in.open(first.data());
						getline(in,first);
						in.close();
						in.open(second.data());
						getline(in,second);
						in.close();
						if (first==second){
							if_enter=true;
						}else{
							if_enter=false;
						}
					}if (bigger){
						ifstream in;
						stringstream turn;
						first=first+".val";
						second=second+".val";
						in.open(first.data());
						getline(in,first);
						in.close();
						in.open(second.data());
						getline(in,second);
						in.close();
						double st,nd;
						turn<<first;
						turn>>st;
						turn.clear();
						turn<<second;
						turn>>nd;
						if (st>nd){
							if_enter=true;
						}else{
							if_enter=false;
						}
					}if (smaller){
						ifstream in;
						stringstream turn;
						first=first+".val";
						second=second+".val";
						in.open(first.data());
						getline(in,first);
						in.close();
						in.open(second.data());
						getline(in,second);
						in.close();
						double st,nd;
						turn<<first;
						turn>>st;
						turn.clear();
						turn<<second;
						turn>>nd;
						if (st<nd){
							if_enter=true;
						}else{
							if_enter=false;
						}
					}
				}
			}if (line=="endif"){
				is_if=false;
			}if (line.length() > 5 and line.substr(0,5)=="else "){
				is_if=true;
				if (is_value(line.substr(5))){
					string val=line.substr(5)+".val";
					ifstream in(val.data());
					string expr;
					in>>expr;
					in.close();
					if (expr=="false" or expr=="nil" or expr=="0"){
						if_enter=false;
					}else{
						if_enter=true;
					}
				}else{
					string expr=line.substr(5);
					string first="",second="";
					bool same=false,bigger=false,smaller=false;
					for (int i=0;i<expr.length();i++){
						if (expr.substr(i,1)=="="){
							same=true;
						}else if (expr.substr(i,1)==">"){
							bigger=true;
						}else if (expr.substr(i,1)=="<"){
							smaller=true;
						}else{
							if (!same and !bigger and !smaller){
								first=first+expr.substr(i,1);
							}else{
								second=second+expr.substr(i,1);
							}
						}
					}
					if (same){
						ifstream in;
						first=first+".val";
						second=second+".val";
						in.open(first.data());
						getline(in,first);
						in.close();
						in.open(second.data());
						getline(in,second);
						in.close();
						if (first==second){
							if_enter=false;
						}else{
							if_enter=true;
						}
					}if (bigger){
						ifstream in;
						stringstream turn;
						first=first+".val";
						second=second+".val";
						in.open(first.data());
						getline(in,first);
						in.close();
						in.open(second.data());
						getline(in,second);
						in.close();
						double st,nd;
						turn<<first;
						turn>>st;
						turn.clear();
						turn<<second;
						turn>>nd;
						if (st>nd){
							if_enter=false;
						}else{
							if_enter=true;
						}
					}if (smaller){
						ifstream in;
						stringstream turn;
						first=first+".val";
						second=second+".val";
						in.open(first.data());
						getline(in,first);
						in.close();
						in.open(second.data());
						getline(in,second);
						in.close();
						double st,nd;
						turn<<first;
						turn>>st;
						turn.clear();
						turn<<second;
						turn>>nd;
						if (st<nd){
							if_enter=false;
						}else{
							if_enter=true;
						}
					}
				}
			}if (line.length() > 4 and line.substr(0,4)=="use "){
				string funcName=line.substr(4,line.length())+".cbg";
				execute(funcName);
			}if (line.length() > 6 and line.substr(0,6)=="title "){
				if (is_value(line.substr(6))){
					string name=line.substr(6)+".val";
					ifstream in(name.data());
					string content;
					getline(in,content);
					in.close();
					SetConsoleTitle(content.substr(1,content.length()-2).data());
				}if (is_char(line.substr(6))){
					string content=line.substr(6);
					SetConsoleTitle(content.substr(1,content.length()-2).data());
				}
			}if (line.length() > 6 and line.substr(0,6)=="sleep "){
				if (is_value(line.substr(6))){
					string name=line.substr(6)+".val";
					ifstream in(name.data());
					string content;
					getline(in,content);
					in.close();
					int sleepTime;
					stringstream turn;
					turn<<content;
					turn>>sleepTime;
					Sleep(sleepTime);
				}if (is_number(line.substr(6))){
					string content=line.substr(6);
					int sleepTime;
					stringstream turn;
					turn<<content;
					turn>>sleepTime;
					Sleep(sleepTime);
				}
			}if (line.length() > 6 and line.substr(0,6)=="while "){
				is_while=true;
				to_while=line_number;
				if (is_value(line.substr(6))){
					string val=line.substr(6)+".val";
					ifstream in(val.data());
					string expr;
					in>>expr;
					in.close();
					if (expr=="false" or expr=="nil" or expr=="0"){
						while_enter=false;
					}else{
						while_enter=true;
					}
				}else{
					string expr=line.substr(6);
					string first="",second="";
					bool same=false,bigger=false,smaller=false;
					for (int i=0;i<expr.length();i++){
						if (expr.substr(i,1)=="="){
							same=true;
						}else if (expr.substr(i,1)==">"){
							bigger=true;
						}else if (expr.substr(i,1)=="<"){
							smaller=true;
						}else{
							if (!same and !bigger and !smaller){
								first=first+expr.substr(i,1);
							}else{
								second=second+expr.substr(i,1);
							}
						}
					}
					if (same){
						ifstream in;
						first=first+".val";
						second=second+".val";
						in.open(first.data());
						getline(in,first);
						in.close();
						in.open(second.data());
						getline(in,second);
						in.close();
						if (first==second){
							while_enter=true;
						}else{
							while_enter=false;
						}
					}if (bigger){
						ifstream in;
						stringstream turn;
						first=first+".val";
						second=second+".val";
						in.open(first.data());
						getline(in,first);
						in.close();
						in.open(second.data());
						getline(in,second);
						in.close();
						double st,nd;
						turn<<first;
						turn>>st;
						turn.clear();
						turn<<second;
						turn>>nd;
						if (st>nd){
							while_enter=true;
						}else{
							while_enter=false;
						}
					}if (smaller){
						ifstream in;
						stringstream turn;
						first=first+".val";
						second=second+".val";
						in.open(first.data());
						getline(in,first);
						in.close();
						in.open(second.data());
						getline(in,second);
						in.close();
						double st,nd;
						turn<<first;
						turn>>st;
						turn.clear(); 
						turn<<second;
						turn>>nd;
						if (st<nd){
							while_enter=true;
						}else{
							while_enter=false;
						}
					}
				}if (!while_enter){
					while (line!="endwhile"){
						getline(file,line);	
					}
				}
				while (while_enter){
					ifstream t_file(path.data());
					string t_line,execute_code="";
					bool start=false,end=false;
					int t_line_number=0;
					while (getline(t_file,t_line)){
						t_line_number++;
						if (t_line_number==to_while){
							start=true;
						}else if (start and t_line=="endwhile"){
							end=true;
						}else if (start and !end){
							execute_code=execute_code+t_line+"\n";
						}
					}if (is_value(line.substr(6))){
						string val=line.substr(6)+".val";
						ifstream in(val.data());
						string expr;
						in>>expr;
						in.close();
						if (expr=="false" or expr=="nil" or expr=="0"){
							while_enter=false;
						}else{
							while_enter=true;
						}
					}else{
						string expr=line.substr(6);
						string first="",second="";
						bool same=false,bigger=false,smaller=false;
						for (int i=0;i<expr.length();i++){
							if (expr.substr(i,1)=="="){
								same=true;
							}else if (expr.substr(i,1)==">"){
								bigger=true;
							}else if (expr.substr(i,1)=="<"){
								smaller=true;
							}else{
								if (!same and !bigger and !smaller){
									first=first+expr.substr(i,1);
								}else{
									second=second+expr.substr(i,1);
								}
							}
						}
						if (same){
							ifstream in;
							first=first+".val";
							second=second+".val";
							in.open(first.data());
							getline(in,first);
							in.close();
							in.open(second.data());
							getline(in,second);
							in.close();
							if (first==second){
								while_enter=true;
							}else{
								while_enter=false;
							}
						}if (bigger){
							ifstream in;
							stringstream turn;
							first=first+".val";
							second=second+".val";
							in.open(first.data());
							getline(in,first);
							in.close();
							in.open(second.data());
							getline(in,second);
							in.close();
							double st,nd;
							turn<<first;
							turn>>st;
							turn.clear();
							turn<<second;
							turn>>nd;
							if (st>nd){
								while_enter=true;
							}else{
								while_enter=false;
							}
						}if (smaller){
							ifstream in;
							stringstream turn;
							first=first+".val";
							second=second+".val";
							in.open(first.data());
							getline(in,first);
							in.close();
							in.open(second.data());
							getline(in,second);
							in.close();
							double st,nd;
							turn<<first;
							turn>>st;
							turn.clear(); 
							turn<<second;
							turn>>nd;
							if (st<nd){
								while_enter=true;
							}else{
								while_enter=false;
							}
						}
					}
					if (while_enter){
						ofstream of("while_code.cbgf");
						of<<execute_code;
						of.close();
						t_file.close();
						execute((string)"while_code.cbg");
					}
				}is_while=false;
				while (line!="endwhile"){
					getline(file,line);	
				}
			}if (line=="throw"){
				throw 0;
			}if (line.length() > 5 and line.substr(0,5)=="read "){
				string name="",val="",expr=line.substr(5);
				bool next=false;
				for (int i=0;i<expr.length();i++){
					if (expr.substr(i,1)==","){
						next=true;
					}else{
						if (!next){
							name=name+expr.substr(i,1);
						}else{
							val=val+expr.substr(i,1);
						}
					}
				}name=name+".val";
				val=val+".val";
				ifstream pipe(name.data());
				getline(pipe,name);
				pipe.close();
				name=name.substr(1,name.length()-2); 
				ifstream in(name.data());
				string content,t_line;
				while (getline(in,t_line)){
					content=content+t_line+"\\n";
				}content="\""+content.substr(0,content.length()-2)+"\"";
				in.close();
				ofstream of(val.data());
				of<<content;
				of.close();
			}if (line.length() > 6 and line.substr(0,6)=="write "){
				string name="",content="",expr=line.substr(6);
				bool next=false;
				for (int i=0;i<expr.length();i++){
					if (expr.substr(i,1)==","){
						next=true;
					}else{
						if (!next){
							name=name+expr.substr(i,1);
						}else{
							content=content+expr.substr(i,1);
						}
					}
				}name=name+".val";
				content=content+".val";
				ifstream pipe(name.data());
				getline(pipe,name);
				pipe.close();
				pipe.open(content.data());
				getline(pipe,content);
				pipe.close();
				name=name.substr(1,name.length()-2); 
				content=content.substr(1,content.length()-2);
				for (int i=0;i<content.length();i++){
					if (content.substr(i,2)=="\\n"){
						content=content.substr(0,i)+"\n"+content.substr(i+2);
					}if (content.substr(i,3)=="\\\\n"){
						content=content.substr(0,i)+"\\n"+content.substr(i+3);
					} 
				}
				ofstream of(name.data());
				of<<content;
				of.close();
			}if (line.length() > 7 and line.substr(0,7)=="append "){
				string name="",content="",expr=line.substr(7);
				bool next=false;
				for (int i=0;i<expr.length();i++){
					if (expr.substr(i,1)==","){
						next=true;
					}else{
						if (!next){
							name=name+expr.substr(i,1);
						}else{
							content=content+expr.substr(i,1);
						}
					}
				}name=name+".val";
				content=content+".val";
				ifstream pipe(name.data());
				getline(pipe,name);
				pipe.close();
				pipe.open(content.data());
				getline(pipe,content);
				pipe.close(); 
				name=name.substr(1,name.length()-2); 
				content=content.substr(1,content.length()-2);
				for (int i=0;i<content.length();i++){
					if (content.substr(i,2)=="\\n"){
						content=content.substr(0,i)+"\n"+content.substr(i+2);
						content=content.substr(0,content.length()-1);
					}if (content.substr(i,3)=="\\\\n"){
						i+=3;
					} 
				}
				ofstream of(name.data(),ios::out|ios::app);
				of<<content;
				of.close();
			}else if (line.length() > 8 and line.substr(0,8)=="cutchar "){
				string expr=line.substr(8),val="",start="",end="";
				int index=0; 
				for (int i=0;i<expr.length();i++){
					if (expr.substr(i,1)==","){
						index++;
					}else{
						if (index==0){
							val=val+expr.substr(i,1);
						}if (index==1){
							start=start+expr.substr(i,1);
						}if (index==2){
							end=end+expr.substr(i,1);
						}
					}
				}stringstream turn;
				int s,e;
				turn<<start;
				turn>>s;
				turn.clear();
				turn<<end;
				turn>>e;
				val=val+".val";
				string save=val;
				ifstream in(val.data());
				getline(in,val);
				val=val.substr(1,val.length()-2);
				in.close();
				try{
					val=val.substr(s,e);
				}catch(...){
					val="0";
				}ofstream of(save.data());
				of<<"\""<<val<<"\"";
				of.close();
			}else if (line.length() > 7 and line.substr(0,7)=="python "){
				string val=line.substr(7)+".val";
				string content;
				ifstream in(val.data());
				getline(in,content);
				python_run(content);
			}else if (line.length() > 5 and line.substr(0,5)=="time "){
				stringstream turn;
				string t_value;
				turn<<time(NULL);
				turn>>t_value;
				redef(line.substr(5),t_value);
			}else if (line.length() > 5 and line.substr(0,5)=="rand "){
				srand(time(NULL));
				stringstream turn;
				string t_value;
				turn<<rand();
				turn>>t_value;
				redef(line.substr(5),t_value);
			}
		}catch(...){
			if (trying){
				execute("try_code.cbg");
			}else{
				throw 0;
			}
		}
		if (line=="try"){
			trying=true;
		}if (line=="catch"){
			ifstream t_file(path.data());
			string t_line,execute_code="";
			bool start=false,end=false;
			int t_line_number=0;
			while (getline(t_file,t_line)){
				t_line_number++;
				if (t_line_number==to_while){
					start=true;
				}else if (start and t_line=="endtry"){
					end=true;
				}else if (start and !end){
					execute_code=execute_code+t_line+"\n";
				}
			}ofstream of("try_code.cbgf");
			of<<execute_code;
			of.close();
			t_file.close();
		}
	}
	file.close();
	return 0;
}



