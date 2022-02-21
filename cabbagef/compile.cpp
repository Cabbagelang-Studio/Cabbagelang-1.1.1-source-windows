#include <iostream>
#include <fstream>
#include <string>
#include <io.h>
#include <sstream>
#include <direct.h> 
#define MAX_PATH 260
bool success=true;
using namespace std;
void output(string path,string content){//Output .cbgf file.
	ofstream file;
	file.open(path.data(),ios::out|ios::app);
	file<<content<<endl;
	file.close();
}string trim(string str){//Trim the string.
	if (!str.empty()){
		str.erase(0,str.find_first_not_of(" "));
		str.erase(str.find_last_not_of(" ")+1);
		str.erase(0,str.find_first_not_of("\t"));
		str.erase(str.find_last_not_of("\t")+1);
	}return str;
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
	string exp=expr+".val";
	file.open(exp.data());
	bool result=file.is_open();
	file.close();
	if (expr.substr(0,5)=="args/" and is_number(expr.substr(5))){
		result=true;
	}
	return result;
}string replace_all(string str,const string old_value,const string new_value){//Replace string function.     
	while(true)   {     
		string::size_type   pos(0);     
		if(   (pos=str.find(old_value))!=string::npos   )     
		str.replace(pos,old_value.length(),new_value);     
		else   break;     
	}     
	return   str;     
} 
inline bool is_expr(string expr){ //Is it a expression?
	return (is_char(expr) or is_number(expr) or is_bool(expr) or is_stmt(expr) or is_value(expr));
}class Exception{//Exception class.
	public:
		void raise(const char* content,string line,int line_number){
			if (success){
				cout<<"Excpetion list:"<<endl;
			}
			string sign1="|    [";
			string sign2="]-->";
			string sign3=":";
			string number;
			stringstream turn;
			turn<<line_number;
			turn>>number;
			string log=sign1+number+sign2+line+sign3+content;
			cout<<"|    ["<<number<<"]-->"<<line<<":"<<content<<endl;
			out(log.data());
			success=false;
		}
	private:
		void out(const char* content){
			ofstream file;
			file.open("compile_log.log",ios::out|ios::app);
			file<<content<<endl;
			file.close();
		}
};
void compile(string path){//Compile function.
	ofstream __CONST__("__FILE__.val");
	char buffer[MAX_PATH];
	getcwd(buffer,MAX_PATH);
	__CONST__<<(buffer+path);
	__CONST__.close();
	string outP=path+"f";
	ofstream logCleaner("compile_log.log");
	logCleaner<<"";
	logCleaner.close();
	ofstream fileCleaner;
	fileCleaner.open(outP.data());
	fileCleaner<<"";
	fileCleaner.close();
	ifstream file; 
	file.open(path.data());
	string line;
	int line_number=0;
	Exception error;
	bool is_if=false,is_while=false,trying=false;
	while (getline(file,line)){
		line_number++;
		line=trim(line);
		if (line.length() > 7 and line.substr(0,7)=="output "){
			output(outP,line);
		}else if (line.length() > 4 and line.substr(0,4)=="val "){
			if (is_expr(line.substr(4)) and !is_value(line.substr(4))){
				error.raise("It is a expression.",line,line_number);
			}else{
				bool NoInvalidSign=true;
				string expr=line.substr(4);
				string value="",name="";
				for (int i=0;i<expr.length();i++){
					if (expr.substr(i,1)=="=" or expr.substr(i,1)=="+"  or expr.substr(i,1)=="-"  or expr.substr(i,1)=="*"  or expr.substr(i,1)=="/"  or expr.substr(i,1)=="^"  or expr.substr(i,1)==" " or expr.substr(i,1)=="," or expr.substr(i,1)=="#"){
						if (expr.substr(i,1)=="="){
							name=expr.substr(0,i);
							value=expr.substr(i+1);
						}else{
							NoInvalidSign=false;	
						}
					}
				}if (NoInvalidSign){
					ofstream of;
					string vP=name+".val";
					of.open(vP.data());
					of<<"0";
					of.close();
					output(outP,"redef "+name+"="+value);
				}else{
					error.raise("Invalid syntax.",line,line_number);
				}	
			}
		}else if (line.length() > 6 and line.substr(0,6)=="redef "){
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
			}if (name=="" or value==""){
				error.raise("Invalid syntax.",line,line_number);
			}else{
				if (is_value(name)){
					output(outP,line);
				}else{
					error.raise("It is not a value.",line,line_number);	
				}
			}
		}else if (line.length() > 7 and line.substr(0,7)=="number "){
			if (is_value(line.substr(7))){
				output(outP,line); 
			}else{
				error.raise("It is not a value.",line,line_number);
			}
		}else if (line.length() > 5 and line.substr(0,5)=="char "){
			if (is_value(line.substr(5))){
				output(outP,line);
			}else{
				error.raise("It is not a value.",line,line_number);
			}
		}else if (line.length() > 5 and line.substr(0,5)=="bool "){
			if (is_value(line.substr(5))){
				output(outP,line);
			}else{
				error.raise("It is not a value.",line,line_number);
			}
		}else if (line.length() > 5 and line.substr(0,5)=="list "){
			if (!is_stmt(line.substr(5))){
				if ((access(line.substr(5).data(),F_OK)==-1)){
					string cmd="md "+line.substr(5);
					system(cmd.data());
				}ofstream of;
				string len=line.substr(5)+"/len.val";
				of.open(len.data());
				of<<"0";
				of.close();	
			}else{
				error.raise("It is a expression.",line,line_number);
			}
		}else if (line.length() > 7 and line.substr(0,7)=="return "){
			if (is_number(line.substr(7))){
				output(outP,line);
			}else{
				error.raise("It is not a number.",line,line_number);
			}
		}else if (line.length() > 7 and line.substr(0,7)=="system "){
			if (is_char(line.substr(7)) or is_value(line.substr(7))){
				output(outP,line);
			}else{
				error.raise("It is not a value or characters.",line,line_number);
			}
		}else if (line.length() > 8 and line.substr(0,8)=="connect "){
			string now="";
			bool next=true,first=true;
			string expr=line.substr(8);
			for (int i=0;i<expr.length();i++){
				if (expr.substr(i,1)=="+"){
					if (is_value(now) or is_char(now)){
						if (first){
							first=false;
							if (!is_value(now)){
								error.raise("The first item is not a value.",line,line_number);
								next=false;
							}
						}now="";
					}else{
						error.raise("It is not a value or characters.",line,line_number);
						next=false;
						break;
					}
				}else{
					now=now+expr.substr(i,1);
				}
			}if (next){
				if (now==""){
					error.raise("Invalid syntax.",line,line_number);
				}else{
					if (is_value(now) or is_char(now)){
						output(outP,line);
					}else{
						error.raise("It is not a value or characters.",line,line_number);
					}
				}	
			}
		}else if (line.length() > 6 and line.substr(0,6)=="count "){
			string now="";
			bool next=true,first=true;
			string expr=line.substr(6);
			for (int i=0;i<expr.length();i++){
				if (expr.substr(i,1)=="+" or expr.substr(i,1)=="-" or expr.substr(i,1)=="*" or expr.substr(i,1)=="/" or expr.substr(i,1) == "^" or expr.substr(i,1) == "%"){
					if (is_value(now) or is_number(now)){
						if (first){
							first=false;
							if (!is_value(now)){
								error.raise("The first item is not a value.",line,line_number);
								next=false;
							}
						}now="";
					}else{
						error.raise("It is not a value or a number.",line,line_number);
						next=false;
						break;
					}
				}else{
					now=now+expr.substr(i,1);
				}
			}if (next){
				if (now==""){
					error.raise("Invalid syntax.",line,line_number);
				}else{
					if (is_value(now) or is_number(now)){
						output(outP,line);
					}else{
						error.raise("It is not a value or a number.",line,line_number);
					}
				}	
			}
		}else if (line.length() > 3 and line.substr(0,3)=="if " and !is_if){
			if (is_value(line.substr(3))){
				is_if=true;	
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
				}if (is_expr(first) and is_expr(second)){
					output(outP,line);
				}else{
					goto raiseError;
				}is_if=true;
			}
		}else if (line=="endif" and is_if){
			is_if=false;
			output(outP,line);
		}else if (line.length() > 5 and line.substr(0,5)=="else " and !is_if){
			if (is_value(line.substr(5))){
				is_if=true;	
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
				}if (is_expr(first) and is_expr(second)){
					output(outP,line);
				}else{
					goto raiseError;
				}is_if=true;
			}
		}else if (line.length() > 5 and line.substr(0,5)=="func "){
			string funcName=line.substr(5)+".cbgf";
			outP=funcName;
			ofstream of(funcName.data());
			of<<"";
			of.close();
		}else if (line=="endfunc"){
			outP=path+"f";
		}else if (line.length() > 4 and line.substr(0,4)=="use "){
			string func=line.substr(4)+".cbgf";
			ifstream in(func.data());
			bool gotoRaise=not(in.is_open());
			in.close();
			if (gotoRaise){
				goto raiseError;
			}else{
				output(outP,line);
			}
		}else if (line.length() > 6 and line.substr(0,6)=="title "){
			if(is_value(line.substr(6)) or is_char(line.substr(6))){
				output(outP,line);
			}else{
				goto raiseError;
			}
		}else if (line.length() > 6 and line.substr(0,6)=="sleep "){
			if(is_value(line.substr(6)) or is_number(line.substr(6))){
				output(outP,line);
			}else{
				goto raiseError;
			}
		}else if (line.length() > 6 and line.substr(0,6)=="while " and !is_while){
			if (is_value(line.substr(6))){
				is_while=true;	
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
				if (is_expr(first) and is_expr(second)){
					output(outP,line);
				}else{
					cout<<is_value(first)<<":"<<second<<endl;
					goto raiseError;
				}is_while=true;
			}
		}else if (line=="endwhile" and is_while){
			is_while=false;
			output(outP,line);
		}else if (line=="try" and !trying){
			trying=true;
			output(outP,line);
		}else if (line=="catch" and trying){
			output(outP,line);
		}else if (line=="endtry" and trying){
			trying=false;
			output(outP,line);
		}else if (line=="throw"){
			output(outP,line);
		}else if (line.length() > 5 and line.substr(0,5)=="read "){
			string name="",content="",expr=line.substr(5);
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
			}if (is_value(name.data()) and is_value(content)){
				output(outP,line);
			}else{
				goto raiseError;
			}
		}else if (line.length() > 6 and line.substr(0,6)=="write "){
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
			}if (is_value(name.data()) and is_value(content)){
				output(outP,line);
			}else{
				goto raiseError;
			}
		}else if (line.length() > 7 and line.substr(0,7)=="append "){
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
			}if (is_value(name.data()) and is_value(content)){
				output(outP,line);
			}else{
				goto raiseError;
			}
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
			}if (is_value(val) and is_number(start) and is_number(end)){
				output(outP,line);
			}else{
				goto raiseError;
			}
		}else if (line.length() > 7 and line.substr(0,7)=="import "){
			string package=line.substr(7)+".cbg";
			if (access(package.data(),F_OK)==-1){
				compile(package);
			}else{
				error.raise(string("Package \""+package+"\"is non-existent.").data(),line,line_number);
			}
		}else if (line.length() > 6 and line.substr(0,6)=="class "){
			int to_class=line_number;
			string class_name=line.substr(6)+".classf";
			ifstream t_file(path.data());
			string t_line,execute_code="";
			bool start=false,end=false;
			int t_line_number=0;
			while (getline(t_file,t_line)){
				t_line_number++;
				if (t_line_number==to_class){
					start=true;
				}else if (start and t_line=="endclass"){
					end=true;
				}else if (start and !end){
					execute_code=execute_code+t_line+"\n";
				}
			}ofstream of(class_name.data());
			of<<execute_code;
			of.close();
			while (line!="endclass"){
				getline(file,line);	
			}
		}else if (line.length() > 4 and line.substr(0,4)=="new "){
			string expr=line.substr(4),val="",class_name="";
			bool giving=false;
			bool param=false; 
			string param_str="";
			for (int i=0;i<expr.length();i++){
				if (expr.substr(i,1)=="="){
					giving=true;
				}else if (expr.substr(i,1)==":"){
					giving=false;
					param=true;
				}else{
					if (!giving and !param){
						val=val+expr.substr(i,1);
					}else if (giving){
						class_name=class_name+expr.substr(i,1);
					}else if (param){
						param_str=param_str+expr.substr(i,1);
					}
				}
			}
			if (access(val.data(),F_OK)==-1){
				system(("md "+val).data());
			}if (access((val+"/params").data(),F_OK)==-1){
				system(("md "+val+"\\params").data());
			}int index=0,last=0;
			for (int i=0;i<expr.length();i++){
				if (expr.substr(i,1)==","){
					stringstream turn;
					turn<<index;
					string id_index;
					turn>>id_index;
					ofstream of((val+"/params/"+id_index+".val").data());
					of<<expr.substr(last+1,(i-last-1));
					of.close();
					index++;
					last=i;
				}
			}
			class_name=class_name+".classf"; 
			ifstream in(class_name.data());
			string content="",temp;
			while (getline(in,temp)){
				content=content+temp+"\n";
			}
			content=replace_all(content,"self",val);
			content=replace_all(content,("\\\\"+val),"self");
			in.close();
			ofstream of((val+"/"+class_name.substr(0,class_name.length()-7)+".cbg").data());
			of<<content;
			of.close();
			of.open((val+"/__init__.cbgf").data());
			of<<"";
			of.close();
			output(outP,("use "+val+"/__init__")); 
			compile((val+"\\"+class_name.substr(0,class_name.length()-7)+".cbg"));
		}else if (line.length() > 7 and line.substr(0,7)=="python "){
			if (is_value(line.substr(7))){
				output(outP,line);
			}else{
				goto raiseError;
			}
		}else if (line.length() > 5 and line.substr(0,5)=="time "){
			if (is_value(line.substr(5))){
				output(outP,line);
			}else{
				goto raiseError;
			}
		}else if (line.length() > 5 and line.substr(0,5)=="rand "){
			if (is_value(line.substr(5))){
				output(outP,line);
			}else{
				goto raiseError;
			}
		}
		
		else if (line.length() > 2 and line.substr(0,2)=="@$"){
			system((line+" "+path).substr(2).data());
		}else if (line.length() > 1 and line.substr(0,1)=="$"){
			system(line.substr(1).data());
		}else if (line.length() > 1 and line.substr(0,1)=="@"){
			cout<<("Note:"+line.substr(1))<<endl;
		}
		
		else if (line.length() > 2 and line.substr(0,2)=="//"){
			
		}else if (line.empty()){
			
		} 
		else{
			error.raise("Invalid script.",line,line_number);
			raiseError:
				error.raise("Invalid syntax.",line,line_number);
		}
	}if (is_if or is_while or trying){
		error.raise("Invalid syntax",line,line_number);
	}if (!success){
		cout<<"_________________"<<endl;
		string cmd="del "+outP;
		system(cmd.data());
	}
	file.close();
}

