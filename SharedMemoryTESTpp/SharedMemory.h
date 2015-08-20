#ifndef _INC_SMEM
#define _INC_SMEM

#include <windows.h>
#include <string>

using namespace std;

/******************************************
 *-----共有メモリを使用する為のクラス-----*
 ******************************************/
template <typename T > 
class SharedMemory{
private:
	HANDLE hShMem;	//ファイルマッピングのハンドル保存用
	const std::string FILEMAPNAME;	//共有するファイル名(共有するプロセスで統一する)
	T *datap;	//共有メモリへのポインタ
	bool mIsCreated;	//オブジェクトを新規作成したかどうか

	HANDLE hMutex;	//ミューテックスオブジェクトのハンドル保存用
	const std::string MUTEXNAME;	//ミューテックスオブジェクト名(排他制御対象で統一する)

	//共有メモリへのポインタを取得する
	void getShMem();
	//ファイルマップビューとハンドルを破棄する
	void releaseShMem();

	//排他処理関連
	//ミューテックスオブジェクト取得
	void getMtx();
	//所有権をtime[millisec]待つ
	//基本的にはタイムアウト無し
	void waitMtx(int time = INFINITE);
	//処理が終わったら必ず所有権を返す
	void releaseMtx();

public:
	//コンストラクタ
	//引数にファイルマッピングオブジェクト名が必要
	SharedMemory(const std::string FMNAME);
	//引数にファイルマッピングオブジェクト名とミューテックスオブジェクト名が必要
	SharedMemory(const std::string FMNAME, const std::string MTX);
	//デストラクタ
	~SharedMemory();

	//共有メモリにデータを書き込む
	void setShMemData( T setData , int offset = 0 );
	//データを取得
	T getShMemData( int offset = 0 );

	////オブジェクトを新規作成したかどうか取得
	bool isCreated();

};

//↓↓↓↓ 以下実装部分 ↓↓↓↓
//コンストラクタ
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
//デストラクタ
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
	* CreateFileMappingは指定されたファイルに対する、
	* 名前付きまたは名前なしのファイルマッピングオブジェクトを作成または開きます。
	*
	* HANDLE CreateFileMapping(
	* HANDLE hFile,                       // ファイルのハンドル
	* LPSECURITY_ATTRIBUTES lpAttributes, // セキュリティ
	* DWORD flProtect,                    // 保護
	* DWORD dwMaximumSizeHigh,            // サイズを表す上位 DWORD
	* DWORD dwMaximumSizeLow,             // サイズを表す下位 DWORD
	* LPCTSTR lpName                      // オブジェクト名
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
		//既にオブジェクトが作成されていたかどうか取得
		mIsCreated = GetLastError() == ERROR_ALREADY_EXISTS;

		/*
		* MapViewOfFileは呼び出し側プロセスのアドレス空間に、ファイルのビューをマップします。
		*
		* LPVOID MapViewOfFile(
		* HANDLE hFileMappingObject,   // ファイルマッピングオブジェクトのハンドル
		* DWORD dwDesiredAccess,       // アクセスモード
		* DWORD dwFileOffsetHigh,      // オフセットの上位 DWORD
		* DWORD dwFileOffsetLow,       // オフセットの下位 DWORD
		* SIZE_T dwNumberOfBytesToMap  // マップ対象のバイト数
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
	 * ミューテックスオブジェクトのハンドルを取得する
	 * 既に存在している場合はそのハンドルを返す
	 *
	 * HANDLE CreateMutex(
	 * LPSECURITY_ATTRIBUTES lpMutexAttributes, // セキュリティ記述子
	 * BOOL bInitialOwner, // 最初の所有者
	 * LPCTSTR lpName // オブジェクトの名前
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
		 * ミューテックスオブジェクトの所有権を待つ
		 * 
		 * DWORD WaitForSingleObject(
		 * HANDLE hHandle, // オブジェクトのハンドル
		 * DWORD dwMilliseconds // タイムアウト時間
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
		 * 所有権を返す
		 *
		 * BOOL ReleaseMutex( HANDLE hMutex // ミューテックスオブジェクトのハンドル );
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