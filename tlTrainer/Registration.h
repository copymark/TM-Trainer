#pragma once
#define NO_ONLINE_HWID_CHECK
#include "DllGlobals.h"

namespace Registration
{
	class cCRC
	{
	private:
		DWORD* crctable;
		void generate_crc32_table();
	public:
		cCRC();
		~cCRC();
		DWORD crc32(const char* s);
	};

	__declspec(dllexport) LPCTSTR GetDialogResource(void);

	__declspec(dllexport) bool IsRegistered(void);

	__declspec(dllexport) void SetSavePath(char *pszPath);

	__declspec(dllexport) INT_PTR CALLBACK RegistrationProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void GenerateKey(char szName[16], SIZE_T length, char szOutput[20]);
	int GenerateHWID(char *szName);

	__declspec(dllexport) bool CheckRegistration();

	template <class T> bool SetRegistryKey(HKEY rootkey, const char *keypath, const char *key, DWORD type, T value);

	char *RootKeytoString(HKEY key);
}