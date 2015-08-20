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
	bool mIsCreated;	//�I�u�W�F�N�g��V�K�쐬�������ǂ���

	HANDLE hMutex;	//�~���[�e�b�N�X�I�u�W�F�N�g�̃n���h���ۑ��p
	const std::string MUTEXNAME;	//�~���[�e�b�N�X�I�u�W�F�N�g��(�r������Ώۂœ��ꂷ��)

	//���L�������ւ̃|�C���^���擾����
	void getShMem();
	//�t�@�C���}�b�v�r���[�ƃn���h����j������
	void releaseShMem();

	//�r�������֘A
	//�~���[�e�b�N�X�I�u�W�F�N�g�擾
	void getMtx();
	//���L����time[millisec]�҂�
	//��{�I�ɂ̓^�C���A�E�g����
	void waitMtx(int time = INFINITE);
	//�������I�������K�����L����Ԃ�
	void releaseMtx();

public:
	//�R���X�g���N�^
	//�����Ƀt�@�C���}�b�s���O�I�u�W�F�N�g�����K�v
	SharedMemory(const std::string FMNAME);
	//�����Ƀt�@�C���}�b�s���O�I�u�W�F�N�g���ƃ~���[�e�b�N�X�I�u�W�F�N�g�����K�v
	SharedMemory(const std::string FMNAME, const std::string MTX);
	//�f�X�g���N�^
	~SharedMemory();

	//���L�������Ƀf�[�^����������
	void setShMemData( T setData , int offset = 0 );
	//�f�[�^���擾
	T getShMemData( int offset = 0 );

	////�I�u�W�F�N�g��V�K�쐬�������ǂ����擾
	bool isCreated();

};

//�������� �ȉ��������� ��������
//�R���X�g���N�^
template < typename T >
SharedMemory<T>::SharedMemory(const std::string FMNAME) :FILEMAPNAME(FMNAME)
{
	SharedMemory::getShMem();
	hMutex = NULL;
}
template < typename T >
SharedMemory<T>::SharedMemory(const std::string FMNAME, const std::string MTX) :FILEMAPNAME(FMNAME), MUTEXNAME(MTX)
{
	SharedMemory::getMtx();
	SharedMemory::getShMem();
}
//�f�X�g���N�^
template < typename T >
SharedMemory<T>::~SharedMemory()
{
	SharedMemory::releaseShMem();
	if (hMutex != NULL)	CloseHandle(hMutex);
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
		//���ɃI�u�W�F�N�g���쐬����Ă������ǂ����擾
		mIsCreated = GetLastError() == ERROR_ALREADY_EXISTS;

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
void SharedMemory<T>::getMtx()
{
	/*
	 * �~���[�e�b�N�X�I�u�W�F�N�g�̃n���h�����擾����
	 * ���ɑ��݂��Ă���ꍇ�͂��̃n���h����Ԃ�
	 *
	 * HANDLE CreateMutex(
	 * LPSECURITY_ATTRIBUTES lpMutexAttributes, // �Z�L�����e�B�L�q�q
	 * BOOL bInitialOwner, // �ŏ��̏��L��
	 * LPCTSTR lpName // �I�u�W�F�N�g�̖��O
	 * );
	 */
	hMutex = CreateMutex(NULL, FALSE, MUTEXNAME.c_str());

	if (hMutex != NULL) cout << "Create mutex" << endl;
}

template < typename T >
void SharedMemory<T>::waitMtx(int time)
{
	if (hMutex != NULL)
	{
		/*
		 * �~���[�e�b�N�X�I�u�W�F�N�g�̏��L����҂�
		 * 
		 * DWORD WaitForSingleObject(
		 * HANDLE hHandle, // �I�u�W�F�N�g�̃n���h��
		 * DWORD dwMilliseconds // �^�C���A�E�g����
		 * );
		 */
		WaitForSingleObject(hMutex, time);
	}
}

template < typename T >
void SharedMemory<T>::releaseMtx()
{
	if (hMutex != NULL)
	{
		/*
		 * ���L����Ԃ�
		 *
		 * BOOL ReleaseMutex( HANDLE hMutex // �~���[�e�b�N�X�I�u�W�F�N�g�̃n���h�� );
		 */
		ReleaseMutex(hMutex);
	}
}

template < typename T >
void SharedMemory<T>::setShMemData(T setData, int offset)
{
	waitMtx();
	*(datap + offset) = setData;
	releaseMtx();
}

template < typename T >
T SharedMemory<T>::getShMemData(int offset)
{
	T ret;

	waitMtx();
	ret = *(datap + offset);
	releaseMtx();

	return ret;
}

template < typename T >
bool SharedMemory<T>::isCreated()
{
	return mIsCreated;
}


#endif