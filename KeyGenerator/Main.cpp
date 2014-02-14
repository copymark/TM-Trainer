#include <Windows.h>
#include <stdio.h>
#include "resource.h"


INT_PTR CALLBACK RegistrationProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

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

cCRC::cCRC()
{
	crctable = 0;
}

cCRC::~cCRC()
{
	delete crctable;
}

void cCRC::generate_crc32_table()
{
	DWORD dwPolynomial = 0xEDB88320;
	crctable = new DWORD[256];

	DWORD x;
	for(int i = 0; i < 256; i++)
	{
		x = i;
		for(int j = 8; j > 0; j--)
		{
			if(x & 1) { x = (x >> 1) ^ dwPolynomial; }
			else      { x >>= 1; }
		}
		crctable[i] = x;
	}
}

DWORD cCRC::crc32(const char* s)
{
	if (!crctable) 
	{
		generate_crc32_table(); 
	}

	register DWORD dwCrc32 = 0xFFFFFFFF;
	while(*s) 
	{ 
		dwCrc32 = ((dwCrc32) >> 8) ^ crctable[ *s ^ ((dwCrc32) & 0xFF)]; s++; 
	}
	return ~dwCrc32;
}

int GenerateHWID(char *szUsername)
{
	char cUsername[255] = "";
	char cComputername[255] = "";
	char sHDSerial[255] = "";

	DWORD nSize = sizeof(cUsername);
	DWORD nSize2 = sizeof(cComputername);

	GetUserName(cUsername, &nSize);

	GetComputerName(cComputername, &nSize2);

	DWORD lVolSerialNbr = 0;
	GetVolumeInformation("C:\\", 0, 0, &lVolSerialNbr, 0, 0, 0, 0);
    _ultoa_s(lVolSerialNbr, sHDSerial, sizeof(sHDSerial), 10);

	cCRC *pCRC = new cCRC;
	DWORD crcComputerName = pCRC->crc32(cComputername);
	DWORD crcUserName = pCRC->crc32(cUsername);
	DWORD crcHDSer = pCRC->crc32(sHDSerial);
	DWORD crcUsername = pCRC->crc32(szUsername);

    DWORD HardWareID = (crcComputerName+crcUserName+crcHDSer+crcUsername)*(DWORD)(18031994);

	return HardWareID;
}
/*
MAIN - EINSTIEGSPUNKT
*/
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, RegistrationProc);

	return FALSE;
}

void GenerateKey(char szName[16], SIZE_T length, char szOutput[20])
{
	char szSerial[128];
	int iFillCount = 0;
	if (length < 16)
	{
		iFillCount = 16-length;
	}
	strcpy_s(szSerial, sizeof(szSerial), szName);
	bool bSwitch = false;
	for (int i = 0; i < iFillCount; i++)
	{
		char cNext;
		!bSwitch ? cNext = szName[i] : cNext = szName[length-i];
		szSerial[length+i] = cNext;
		bSwitch = !bSwitch;
	}
	szSerial[length+iFillCount] = 0;

	/*
	Wir haben ejtzt einen 16-Stelligen String + 0
	*/
	strcpy_s(szSerial, sizeof(szSerial), _strrev(szSerial));

	cCRC *pCrc = new cCRC;
	DWORD dwCrc32 = pCrc->crc32(szName);

	for (unsigned int i = 0; i < strlen(szSerial); i++)
	{
		// Irgendwie verhunzen
		szSerial[i] >>= 1;
		szSerial[i] *= i;
		szSerial[i] ^= dwCrc32;
		// erst positive zahl dann nur zahlen und große buchstaben
		char cMod = (szSerial[i] & 0x7F) % 42;
		szSerial[i] = cMod + 48;

		// paar sonderzeichen dazwischen, einfach addieren damit wir in den Buchstabenbereich kommen
		if (szSerial[i] >= 0x3A && szSerial[i] <= 0x40)
		{
			szSerial[i] += 7;
		}
	}



	/*
	Bindestriche einfügen
	*/
	char szFinalSerial[20];
	char *c = szSerial;
	int i;
	for (i = 0; *c; ++i) 
		szFinalSerial[i] = i%5==4 ? '-' : *c++; 
	szFinalSerial[i] = '\0';

	// in Final OUtput schreiben
	strcpy_s(szOutput, 20, szFinalSerial);
}

void GetCurrentLicence(HWND *pHwnd)
{
	SetDlgItemText(*pHwnd, IDC_EDIT3, "Not Registered");
	SetDlgItemText(*pHwnd, IDC_EDIT4, "Not Registered");

	HKEY hKey;
	long lReturn = RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Nadeo\\TMUltraTrainer", 0, KEY_ALL_ACCESS, &hKey);
	if (lReturn == ERROR_SUCCESS)
	{
		char szName[16];
		char szKey[20];

		SIZE_T Length = sizeof(szName);
		SIZE_T LengthKey = sizeof(szKey);
		RegQueryValueEx(hKey, "hack_username", NULL, NULL, (LPBYTE)szName, &Length);
		RegQueryValueEx(hKey, "hack_key", NULL, NULL, (LPBYTE)szKey, &LengthKey);

		SetDlgItemText(*pHwnd, IDC_EDIT3, szName);
		SetDlgItemText(*pHwnd, IDC_EDIT4, szKey);
	}

	RegCloseKey(hKey);
}

void DeleteCurrentLicence(void)
{
	HKEY hKey;
	long lReturn = RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Nadeo", 0, KEY_ALL_ACCESS, &hKey);

	if (lReturn == ERROR_SUCCESS)
	{
		long lSuccess = RegDeleteKey(hKey, "TMUltraTrainer");
		if (lSuccess == ERROR_SUCCESS)
		{
			MessageBox(0, "Licence deleted...", "Success", 0);
		}
	}

	RegCloseKey(hKey);
}

INT_PTR CALLBACK RegistrationProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
			{
				GetCurrentLicence(&hwndDlg);
				return TRUE;
			}
		case WM_COMMAND:
			{
				int wmId    = LOWORD(wParam);
				int wmEvent = HIWORD(wParam);

				switch (wmId)
				{
				case IDC_BUTTON1:
					{
						char szName[16];
						SIZE_T length = GetDlgItemText(hwndDlg, IDC_EDIT1, szName, sizeof(szName));
						if (length >= 8)
						{
							char szOutput[20];
							GenerateKey(szName, length, szOutput);
							SetDlgItemText(hwndDlg, IDC_EDIT2, szOutput);

							char szHwid[20];
							int iHwid = GenerateHWID(szName);
							sprintf_s(szHwid, sizeof(szHwid), "%d", iHwid);
							SetDlgItemText(hwndDlg, IDC_EDIT5, szHwid);
						}
						else
						{
							MessageBox(hwndDlg, "Name must be at least 8 characters long!", "ERROR", 0);
						}

						break;
					}
				case IDC_BUTTON2:
					{
						DeleteCurrentLicence();
						break;
					}
				}
				return TRUE;
			}
		case WM_CLOSE:
			{
				EndDialog(hwndDlg, 0);
				return TRUE;
			}
	}
	return FALSE;
}