#include <Windows.h>
#include <stdio.h>
#include <time.h>

bool dataCompare(const BYTE *pData, const BYTE *pMask, const char *sMask)
{
	for (; *sMask; ++sMask, ++pData, ++pMask)
	{
		if (*sMask == 'x' && *pData != *pMask)
		{
			return false;
		}
	}

	return (*sMask == NULL);
}

struct SIGADDRESS_DLL
{
	/*
	Signaturspezifisch
	*/
	DWORD dwBaseAddress;
	DWORD dwLength;
	BYTE pMask[64];
	char sMask[64];
	DWORD dwOffset;

	bool bGetOpcode;

	/*
	Endgültige Addresse
	*/
	DWORD dwAddress;

	/*
	Name der Addresse
	*/
	char szName[32];
};

extern "C" __declspec(dllexport) void findPattern(SIGADDRESS_DLL pSig)
{
	pSig.dwAddress = 0;

	for (DWORD i = 0; i < pSig.dwLength; i++)
	{
		if (dataCompare((BYTE*)(pSig.dwBaseAddress + i), pSig.pMask, pSig.sMask))
		{
			pSig.dwAddress = ((DWORD)(pSig.dwBaseAddress + i)) + pSig.dwOffset;
			break;
		}
	}

	if (pSig.dwAddress == NULL && pSig.bGetOpcode)
	{
		pSig.dwAddress = *(PDWORD)pSig.dwAddress;
	}

	DWORD nBr = 0;
	HANDLE hPipe = CreateFile("\\\\.\\pipe\\sigscanpipe", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(hPipe, &pSig, sizeof(pSig), &nBr, NULL);

	CloseHandle(hPipe);
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

