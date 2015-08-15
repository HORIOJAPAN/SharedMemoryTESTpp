#include <windows.h>
#include <stdio.h>

#include <iostream>
#include <string>

using namespace std;

#define FILEMAP_NAME    "testmap"

static HANDLE ghShMem = NULL;

void *getShMem();
void releaseShMem(void *pAddr);

int main()
{
	int num;

	int *datap = (int *)getShMem();
	
	datap++;

	while (true)
	{
		printf("Input Number =>");

		cin >> num;

		printf("Input Number=%d\n", num);
		*datap = num;
		//if (num == -99)
			break;
	}
	releaseShMem(datap);

	return 0;
}

void *getShMem()
{
	void *pAddr = NULL;

	ghShMem = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		1024,
		FILEMAP_NAME);
	if (ghShMem != NULL)
	{
		//bool isCreated = GetLastError() != ERROR_ALREADY_EXISTS;

		pAddr = (void *)MapViewOfFile(
			ghShMem,
			FILE_MAP_WRITE,
			0,
			0,
			0);
	}
	return pAddr;
}

void releaseShMem(void *pAddr)
{
	if (ghShMem != NULL)
	{
		if (pAddr != NULL)
			UnmapViewOfFile(pAddr);
		CloseHandle(ghShMem);
		ghShMem = NULL;
	}
}