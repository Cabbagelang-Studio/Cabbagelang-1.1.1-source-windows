#include <iostream>
#include <fstream>
#include <sstream>
#include <io.h>
using namespace std;
const char* param(int argc,char** argv){
	if (argc==1){
		cout<<"Fatal Error:No path."<<endl;
		return 0;
	}else{
		ifstream request;
		request.open(argv[1]);
		if (not request.is_open()){
			cout<<"Fatal Error:File is not exist."<<endl;
			return 0;
		}request.close();
		string path=argv[1];
		string compile=path+"f";
		request.open(compile.data());
		if (not request.is_open()){
			cout<<"Fatal Error:Can not open compile file."<<endl;
			return 0;
		}request.close(); 
		if (path.substr(path.length()-4)!=".cbg"){
			cout<<"Fatal Error:File format must be \".cbg\"."<<endl;
			return 0;
		}if (!access("args",F_OK)==-1){
			system("md args");
		}
		int c=0; 
		for (int i=0;i<argc-1;i++){
			ofstream of;
			stringstream turn;
			string path;
			turn<<i;
			turn>>path;
			path="args/"+path+".val";
			of.open(path.data());
			of<<argv[i+1];
			of.close();
			c++;
		}ofstream of("argc.val");
		of<<c;
		of.close();
		return path.data();
	}
} 
