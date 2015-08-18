#ifndef _INC_SMEM
#define _INC_SMEM

#include <windows.h>
#include <string>

using namespace std;

/******************************************
 *-----���L���������g�p����ׂ̃N���X-----*
 ******************************************/
template <typename T > 
class SharedMemory{
private:
	HANDLE hShMem;	//�t�@�C���}�b�s���O�̃n���h���ۑ��p
	const std::string FILEMAPNAME;	//���L����t�@�C����(���L����v���Z�X�œ��ꂷ��)
	T *datap;	//���L�������ւ̃|�C���^

	//���L�������ւ̃|�C���^���擾����
	void getShMem();
	//�t�@�C���}�b�v�r���[�ƃn���h����j������
	void releaseShMem();

public:
	//�R���X�g���N�^
	//�����Ƀt�@�C�������K�v
	SharedMemory(const std::string FMNAME);
	//�f�X�g���N�^
	~SharedMemory();

	//���L�������Ƀf�[�^����������
	void setShMemData( T setData , int offset = 0 );
	//�f�[�^���擾
	T getShMemData( int offset = 0 );

};

//�������� �ȉ��������� ��������
template < typename T >
SharedMemory<T>::SharedMemory(const std::string FMNAME) :FILEMAPNAME(FMNAME)
{
	SharedMemory::getShMem();
}

template < typename T >
SharedMemory<T>::~SharedMemory()
{
	SharedMemory::releaseShMem();
}

template < typename T >
void SharedMemory<T>::getShMem()
{
	/*
	* CreateFileMapping�͎w�肳�ꂽ�t�@�C���ɑ΂���A
	* ���O�t���܂��͖��O�Ȃ��̃t�@�C���}�b�s���O�I�u�W�F�N�g���쐬�܂��͊J���܂��B
	*
	* HANDLE CreateFileMapping(
	* HANDLE hFile,                       // �t�@�C���̃n���h��
	* LPSECURITY_ATTRIBUTES lpAttributes, // �Z�L�����e�B
	* DWORD flProtect,                    // �ی�
	* DWORD dwMaximumSizeHigh,            // �T�C�Y��\����� DWORD
	* DWORD dwMaximumSizeLow,             // �T�C�Y��\������ DWORD
	* LPCTSTR lpName                      // �I�u�W�F�N�g��
	* );
	*
	* ref=>https://msdn.microsoft.com/ja-jp/library/Cc430039.aspx
	*/
	hShMem = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		1024,
		FILEMAPNAME.c_str());

	if (hShMem != NULL)
	{
		//bool isCreated = GetLastError() != ERROR_ALREADY_EXISTS;

		/*
		* MapViewOfFile�͌Ăяo�����v���Z�X�̃A�h���X��ԂɁA�t�@�C���̃r���[���}�b�v���܂��B
		*
		* LPVOID MapViewOfFile(
		* HANDLE hFileMappingObject,   // �t�@�C���}�b�s���O�I�u�W�F�N�g�̃n���h��
		* DWORD dwDesiredAccess,       // �A�N�Z�X���[�h
		* DWORD dwFileOffsetHigh,      // �I�t�Z�b�g�̏�� DWORD
		* DWORD dwFileOffsetLow,       // �I�t�Z�b�g�̉��� DWORD
		* SIZE_T dwNumberOfBytesToMap  // �}�b�v�Ώۂ̃o�C�g��
		* );
		*
		* ref=>https://msdn.microsoft.com/ja-jp/library/Cc430198.aspx
		*/
		datap = (T *)MapViewOfFile(
			hShMem,
			FILE_MAP_WRITE,
			0,
			0,
			0);
	}
}

template < typename T >
void SharedMemory<T>::releaseShMem()
{
	if (hShMem != NULL)
	{
		if (datap != NULL)
			UnmapViewOfFile(datap);
		CloseHandle(hShMem);
		hShMem = NULL;
	}
}

template < typename T >
void SharedMemory<T>::setShMemData(T setData, int offset)
{
	*(datap + offset) = setData;
}

template < typename T >
T SharedMemory<T>::getShMemData(int offset)
{
	return *(datap + offset);
}

#endif