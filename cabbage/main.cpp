/*It is a executer of Cabbage language.*/
//Header files.
#include <iostream>
#include <string>
#include "execute.h"
#include "path.h"
using namespace std;
int main(int argc,char** argv){
	if (param(argc,argv)==0){
		return 0;
	}string path=param(argc,argv);
	return execute(path);
}
