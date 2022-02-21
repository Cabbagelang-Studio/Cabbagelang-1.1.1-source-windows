#include <iostream>
#include <fstream>
using namespace std;
const char* param(int argc,char** argv){
	if (argc==1){
		cout<<"Fatal Error:No path."<<endl;
	}else{
		ifstream request;
		request.open(argv[1]);
		if (not request.is_open()){
			cout<<"Fatal Error:File is not exist."<<endl;
		}string path=argv[1]; 
		if (path.substr(path.length()-4)!=".cbg"){
			cout<<"Fatal Error:File format must be \".cbg\"."<<endl;
		}return path.data();
	}return 0;
} 
