#include <iostream>
#include <cstdlib>

#include "SharedMemory.h"

using namespace std;
const string FMNAME = "testmap";	//���L�t�@�C����

void unko_main()
{
	//�^�ƃt�@�C���}�b�s���O�I�u�W�F�N�g�����w�肵�ăC���X�^���X����
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

void main()
{
	int amount;
	int myNum;

	cout << "Please enter \"exit\" when you finish"  << endl;


	//�^�ƃt�@�C���}�b�s���O�I�u�W�F�N�g�����w�肵�ăC���X�^���X����
	SharedMemory<int> shMem(FMNAME);

	if (shMem.isCreated()) {
		myNum = shMem.getShMemData();
		shMem.setShMemData(++myNum);
	}
	else
	{
		myNum = 1;
		shMem.setShMemData(1);
	}

	cout << "My process Number => " << myNum << endl;

	while (true){
		amount = shMem.getShMemData();
		//�N�����̃v���Z�X�̐�
		cout << "\nThe amount of the current process => " << amount << endl;

		for (int i = 1; i <= amount; i++)
		{
			if (i != myNum)	cout << "Process" << i << " says => " << shMem.getShMemData(i) << endl;
		}

		//�W�����͂���int�^�̐����l���擾
		string str;
		cout << "\nInput number => ";
		getline(cin, str);

		//int�^�̐����l�����L��������2�Ԗڂ̗̈�ɏ�������
		shMem.setShMemData(atoi(str.c_str()), myNum);

		getline(cin, str);
		if (str == "exit") break;
		
	}

}