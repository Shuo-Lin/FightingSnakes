#ifndef PRINTASCIIGRAPH
#define PRINTASCIIGRAPH

#include<Windows.h>
#include<fstream>
#include<iostream>
#include<string>



//参数为文件名和颜色代码
void printAG(char * fname,int colar)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colar);
	std::ifstream fin(fname,_IOS_Nocreate);
	if (fin.fail())
	{
		std::cout << "failed to open the file\n";
		exit(1);
	}
	std::string l;
	while (1)
	{
		std::getline(fin, l);
		std::cout << l << std::endl;
		if (fin.eof())
			break;
	}
	fin.close();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}


#endif