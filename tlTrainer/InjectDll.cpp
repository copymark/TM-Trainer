#include "InjectDll.h"

/*
Gibt den absoluten Pfad der Dll zurück
Buffer sollte mit MAX_PATH initialisiert sein
*/
bool InjectDLL::GetDllPath(char *pszBuffer, char *pszDllName)
{
	char szDirectory[MAX_PATH];
	_getcwd(szDirectory, MAX_PATH);

	if (!szDirectory)
	{
		return false;
	}
	sprintf_s(pszBuffer, MAX_PATH, "%s\\%s\0", szDirectory, pszDllName);

	return true;
}

typedef void (*fpFunction)(InjectDLL::SIGADDRESS_DLL);

struct REMOTELOADERPAR
{
	HINSTANCE (__stdcall *LoadLibrary)(char*);
	LPVOID (__stdcall *GetProcAddress)(HINSTANCE, char*);
	char szPath[MAX_PATH];
	char szFunction[16];
	InjectDLL::SIGADDRESS_DLL Address;
};

DWORD WINAPI dwRemoteLoadLoaderThread(LPVOID lpParam)
{
	REMOTELOADERPAR *rlp = (REMOTELOADERPAR*)lpParam;
	fpFunction pFunction = (fpFunction)rlp->GetProcAddress(rlp->LoadLibrary(rlp->szPath), rlp->szFunction);
	pFunction(rlp->Address);
	return 0;
}

/*
Bei der Übergabe mit CreateRemoteThread macht es keinen Sinn Zeiger zu übergeben, da sie im anderen Prozess nicht funktionieren würden
*/
void InjectDLL::SigAddressToDllVersion(SIGADDRESS_DLL *pSigDll, SIGADDRESS *pSigSrc)
{
	pSigDll->dwBaseAddress = pSigSrc->dwBaseAddress;
	pSigDll->bGetOpcode = pSigSrc->bGetOpcode;
	pSigDll->dwLength = pSigSrc->dwLength;
	pSigDll->dwOffset = pSigSrc->dwOffset;
	strcpy_s(pSigDll->sMask, 64, pSigSrc->sMask);
	strcpy_s(pSigDll->szName, 32, pSigSrc->szName);
	for (unsigned int i = 0; i < strlen(pSigSrc->sMask); i++)
	{
		pSigDll->pMask[i] = pSigSrc->pMask[i];
	}
}

bool InjectDLL::RemoteLoadLoader(HANDLE hProcessHandle, char *pszDllName, char *pszFunctionName, SIGADDRESS *Address, HANDLE *hPipe)
{
	/*
	Absoluten Pfad der DLL holen
	*/
	char szDllPath[MAX_PATH];
	if (!GetDllPath(szDllPath, pszDllName))
	{
		return false;
	}

	/*
	Privilegien setzen
	*/
	HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
        tkp.PrivilegeCount = 1;
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(hToken, 0, &tkp, sizeof(tkp), NULL, NULL);
    }

	/*
	Struct füllen, das unserem Thread als Parameter übergeben wird
	---> DIESER TEIL MACHT DIESE FUNKTION "HARDCODED" - FIX nötig
	*/
	REMOTELOADERPAR lParam;
	SigAddressToDllVersion(&lParam.Address, Address);
	strcpy_s(lParam.szPath, sizeof(lParam.szPath), szDllPath);
	strcpy_s(lParam.szFunction, sizeof(lParam.szFunction), pszFunctionName);
	HINSTANCE hDll = LoadLibrary("KERNEL32");
	lParam.LoadLibrary = (HINSTANCE (__stdcall *)(char*))GetProcAddress(hDll, "LoadLibraryA");
	lParam.GetProcAddress = (LPVOID (__stdcall *)(HINSTANCE, char*))GetProcAddress(hDll, "GetProcAddress");

	/*
	Wie groß ist unsere Threadfunktion??
	*/
	DWORD dwFuncSize = 0x42;

	/*
	Platz allozieren für unseren Thread und die struktur die im übergeben wird
	*/
	LPVOID lpParamStart = VirtualAllocEx(hProcessHandle, NULL, dwFuncSize + sizeof(lParam), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	/*
	An die erste Stelle in unserem neuen Speicherbereich schreiben wir die Struktur hin -> Rückgabe von VirtualAlloc = Start Addresse von Struktur
	*/
	WriteProcessMemory(hProcessHandle, lpParamStart, &lParam, sizeof(lParam), NULL);

	/*
	Ende unserer Struktur herausfinden, damit wir direkt ans ende der struktur die Thread funktion schreiben können
	*/
	LPVOID lpThreadStart = (LPVOID)((UINT_PTR)lpParamStart + sizeof(lParam));
	WriteProcessMemory(hProcessHandle, lpThreadStart, &dwRemoteLoadLoaderThread, dwFuncSize, NULL);

	/*
	Thread-Funktion starten und ihr die Addresse von der Struktur übergeben
	*/
	HANDLE hThread = CreateRemoteThread(hProcessHandle, NULL, NULL, (LPTHREAD_START_ROUTINE)lpThreadStart, lpParamStart, NULL, NULL);

	ConnectNamedPipe(*hPipe, NULL);

	SIGADDRESS_DLL buffer;
	DWORD numBytesRead = 0;
	if (ReadFile(*hPipe, &buffer, sizeof(SIGADDRESS_DLL), &numBytesRead, NULL))
	{
		//add_log("Name: %s | dwAddressBuffer: %X | dwAddressOld: %X", buffer.szName, buffer.dwAddress, Address->dwAddress);
		if (buffer.dwAddress != NULL)
			Address->dwAddress = buffer.dwAddress;
	}

	DisconnectNamedPipe(*hPipe);

	/*
	Warten bis Thread fertig
	*/
	WaitForSingleObject(hThread, INFINITE);

	VirtualFreeEx(hProcessHandle, lpParamStart, NULL, MEM_RELEASE);

	return true;
}

bool InjectDLL::RemoteLoadLib(HANDLE hProcessHandle, char *pszDllName)
{
	/*
	Absoluten Pfad der DLL holen
	*/
	char szDllPath[MAX_PATH];
	if (!GetDllPath(szDllPath, pszDllName))
	{
		return false;
	}

	HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
        tkp.PrivilegeCount = 1;
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(hToken, 0, &tkp, sizeof(tkp), NULL, NULL);
    }

	/*
	Platz für den Dll Pfad reservieren
	*/
	UINT_PTR uDllPathAddress = (UINT_PTR)VirtualAllocEx(hProcessHandle, NULL, strlen(szDllPath) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if (!uDllPathAddress)
		return false;

	/*
	An die reservierte Stelle den Pfad schreiben
	*/
	if (!WriteProcessMemory(hProcessHandle, (LPVOID)uDllPathAddress, szDllPath, strlen(szDllPath)+1, NULL))
		return false;

	/*
	Addresse von LoadLibraryA holen
	*/
	PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryA");

	if (!pfnThreadRtn)
		return false;

	/*
	Den Thread (die Funktion LoadLibraryA) starten mit dem Pfad unserer DLL als Parameter
	*/
	HANDLE hThread = CreateRemoteThread(hProcessHandle, NULL, NULL, pfnThreadRtn, (LPVOID)uDllPathAddress, NULL, NULL);

	if (!hThread)
	{
		return false;
	}

	/*
	Warten bis der Thread fertig ist
	*/
	WaitForSingleObject(hThread, INFINITE);

	/*
	Aufräumen
	*/
	if (uDllPathAddress != NULL)
		VirtualFreeEx(hProcessHandle, (LPVOID)uDllPathAddress, strlen(szDllPath) + 1, MEM_RELEASE);

	if (hThread  != NULL) 
		CloseHandle(hThread);

	return true;
}