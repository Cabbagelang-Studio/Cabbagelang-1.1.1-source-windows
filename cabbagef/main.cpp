/*It is a compiler of Cabbage language.*/
//Header files.
#include <iostream>
#include <string>
#include "path.h"
#include "compile.h"
using namespace std;
//Main function.
int main(int argc,char** argv){
	if (param(argc,argv)==0){
		return 0;
	}string path=param(argc,argv);
	compile(path); 
	return 0;
}
