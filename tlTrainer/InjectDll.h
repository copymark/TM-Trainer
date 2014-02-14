#pragma once

#include "DllGlobals.h"
#include "cHack.h"

struct SIGADDRESS;

namespace InjectDLL
{
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

	bool GetDllPath(char *pszBuffer, char *pszDllName);
	void SigAddressToDllVersion(SIGADDRESS_DLL *pSigDll, SIGADDRESS *pSigSrc);
	__declspec(dllexport) bool RemoteLoadLib(HANDLE hProcessHandle, char *pszDllName);
	bool RemoteLoadLoader(HANDLE hProcessHandle, char *pszDllName, char *pszFunctionName, SIGADDRESS *Address, HANDLE *hPipe);
}