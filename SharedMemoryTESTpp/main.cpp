#include <iostream>
#include <cstdlib>

#include "SharedMemory.h"

using namespace std;
const string FMNAME = "testmap";	//共有ファイル名

void main()
{
	//型とファイル名を指定してインスタンス生成
	SharedMemory<int> shMem(FMNAME);
	//先に起動したc#のプロセスが共有メモリの先頭に書き込んだ値を取得
	cout << "Process# says => " << shMem.getShMemData() <<endl;
	
	//標準入力からint型の整数値を取得
	string str;
	cout << "Input number => " ;
	getline(cin,str);

	//int型の整数値を共有メモリの2番目の領域に書き込む
	shMem.setShMemData(atoi(str.c_str()), 1);

	getline(cin, str);

}