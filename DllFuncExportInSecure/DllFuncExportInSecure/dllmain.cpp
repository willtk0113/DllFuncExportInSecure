// dllmain.cpp : DLL 애플리케이션의 진입점을 정의합니다.

#include <iostream>
#include "framework.h"

#define DLL_EXPORT_API 1000

enum SAMPLE_EXPORT_API
{
	SAMPLE_EXPORT_API_ADD = 0x2000,
	SAMPLE_EXPORT_API_PRINT = 0x2001,
};

int Add(int a, int b)
{
	return a + b;
}

void Print(LPCWSTR pwString)
{
	printf("Print() func is called");
}

DWORD_PTR GetApiAddress(DWORD_PTR dwKey)
{
	DWORD_PTR ptr = NULL;
	switch (dwKey)
	{
	case SAMPLE_EXPORT_API_ADD:
		ptr = (DWORD_PTR)Add;
		break;

	case SAMPLE_EXPORT_API_PRINT:
		ptr = (DWORD_PTR)Print;
		break;

	default:
		break;
	}

	return ptr;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;

	case DLL_EXPORT_API:
		if (lpReserved == NULL)
		{
			return FALSE;
		}

		PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hModule;

		if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		{
			return FALSE;
		}

		PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)
			((LPBYTE)pDosHeader + (DWORD)pDosHeader->e_lfanew);

		if (pNTHeader->Signature != IMAGE_NT_SIGNATURE)
		{
			return FALSE;
		}

		PDWORD_PTR pdwAddr = (PDWORD_PTR)lpReserved;

		*pdwAddr = GetApiAddress(*pdwAddr);

		if (*pdwAddr == 0)
		{
			return FALSE;
		}
		break;
    }
	
    return TRUE;
}

