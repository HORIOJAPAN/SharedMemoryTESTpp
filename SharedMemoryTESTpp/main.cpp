#include <iostream>
#include <cstdlib>

#include "SharedMemory.h"

using namespace std;
const string FMNAME = "testmap";	//���L�t�@�C����

void main()
{
	//�^�ƃt�@�C�������w�肵�ăC���X�^���X����
	SharedMemory<int> shMem(FMNAME);
	//��ɋN������c#�̃v���Z�X�����L�������̐擪�ɏ������񂾒l���擾
	cout << "Process# says => " << shMem.getShMemData() <<endl;
	
	//�W�����͂���int�^�̐����l���擾
	string str;
	cout << "Input number => " ;
	getline(cin,str);

	//int�^�̐����l�����L��������2�Ԗڂ̗̈�ɏ�������
	shMem.setShMemData(atoi(str.c_str()), 1);

	getline(cin, str);

}