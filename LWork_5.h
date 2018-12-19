#include "iostream"
#include "fstream"
#include "string"
#include <windows.h>

void CreateConnect(HANDLE &hMapFile, char* &dataPtr,int fsize)
{

	hMapFile = CreateFileMapping(nullptr, NULL, PAGE_READWRITE, 0, fsize , L"OpenMemory");
	dataPtr = (char*)MapViewOfFile(hMapFile,FILE_MAP_ALL_ACCESS, 0, 0, fsize);	
	dataPtr[0]++;
}
void CloseConnect(HANDLE &hMapFile, char* &dataPtr)
{
	if (!(dataPtr[0]-1))
	{
		UnmapViewOfFile(dataPtr);
		CloseHandle(hMapFile);
		MessageBox(NULL, L"", L"Delete memory", 0);
	}
	else
	{
		dataPtr[0] -= 1;
	}
}