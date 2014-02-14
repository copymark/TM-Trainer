#include "Registration.h"
//#include "mysql.h"
#include "XorStr.h"

char *g_pszPath;

bool g_bRegistered = false;

void Registration::SetSavePath(char *pszPath)
{
	g_pszPath = pszPath;
}

bool Registration::IsRegistered(void)
{
	return g_bRegistered;
}

LPCTSTR Registration::GetDialogResource(void)
{
	return MAKEINTRESOURCE(IDD_DIALOG3);
}

void Registration::GenerateKey(char szName[16], SIZE_T length, char szOutput[20])
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
		szSerial[i] *= (char)i;
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

INT_PTR CALLBACK Registration::RegistrationProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
			{
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
						EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON1), FALSE);
						char szName[16];
						char szKeyIn[20];
						unsigned int iLength = GetDlgItemText(hwndDlg, IDC_EDIT1, szName, sizeof(szName));
						unsigned int iLengthKey = GetDlgItemText(hwndDlg, IDC_EDIT2, szKeyIn, sizeof(szKeyIn));
						char szCorrectKey[20];
						GenerateKey(szName, iLength, szCorrectKey);

						if (iLength < 8)
						{
							MessageBox(hwndDlg, "Your name is too short!\nIt has to be at least 8 characters long.", "Warning", 0);
							break;
						}

						if ((iLengthKey == 19) && (!strcmp(szCorrectKey, szKeyIn)))
						{
							// Correct
							SetRegistryKey<char*>(HKEY_CURRENT_USER, g_pszPath, "hack_username", REG_SZ, szName);
							SetRegistryKey<char*>(HKEY_CURRENT_USER, g_pszPath, "hack_key", REG_SZ, szKeyIn);
							CheckRegistration();
							EndDialog(hwndDlg, 0);
						}
						else
						{
							MessageBox(hwndDlg, "Your Key is incorrect!\nTry Again", "Error", 0);
						}
						
						EnableWindow(GetDlgItem(hwndDlg, IDC_BUTTON1), TRUE);
						break;
					}
				}
				return TRUE;
			}
		case WM_CLOSE:
			{
				EndDialog(hwndDlg, 0);
			}
	}
	return FALSE;
}

//bool CheckOnlineHwID(bool &bResult, char *szName, char *szKey, int iHWIDLocal)
//{
//	/*
//	HWID Abfrage auf dem SQL-Server
//	*/
//	MYSQL *pMySql;
//	pMySql = mysql_init(NULL);
//	if (!pMySql)
//	{
//		add_log("Fehler mysql_init(): %u (%s)\n", mysql_errno(pMySql), mysql_error(pMySql));
//		return false;
//	}
//
//	if (mysql_real_connect(pMySql, /*web-berlin.dyndns.org*/XorStr<0x05,22,0xF8E73B41>("\x72\x63\x65\x25\x6B\x6F\x79\x60\x64\x60\x21\x74\x68\x7C\x77\x7A\x66\x38\x78\x6A\x7E"+0xF8E73B41).s, /*copymark*/XorStr<0xB8,9,0x9A80576E>("\xDB\xD6\xCA\xC2\xD1\xDC\xCC\xD4"+0x9A80576E).s, /*nerio123*/XorStr<0x37,9,0xF147D553>("\x59\x5D\x4B\x53\x54\x0D\x0F\x0D"+0xF147D553).s, /*copymark_db1*/XorStr<0xD1,13,0x921E8084>("\xB2\xBD\xA3\xAD\xB8\xB7\xA5\xB3\x86\xBE\xB9\xED"+0x921E8084).s, 0, 0, 0) == NULL)
//	{
//		add_log("Fehler mysql_real_connect(): %u (%s)\n", mysql_errno(pMySql), mysql_error(pMySql));
//		return false;
//	}
//
//	char sql[MAX_PATH];
//	sprintf_s(sql, sizeof(sql), "SELECT hwid FROM tmhackhwids WHERE name = \"%s\" AND serial = \"%s\"", szName, szKey);
//	mysql_real_query(pMySql, sql, strlen(sql));
//
//	if (mysql_errno(pMySql) != 0)
//	{
//		add_log("Fehler: %s\n", mysql_error(pMySql));
//		return false;
//	}
//
//	MYSQL_RES *res = mysql_store_result(pMySql);
//	if (mysql_errno(pMySql) != 0)
//	{
//		add_log("Fehler: %s\n", mysql_error(pMySql));
//		return false;
//	}
//
//	MYSQL_ROW row;
//
//	while (row = mysql_fetch_row(res))
//	{
//		int iOnlineHWID = atoi(row[0]);
//		if (iOnlineHWID == iHWIDLocal)
//		{
//			bResult = true;
//			break;
//		}
//	}
//
//	mysql_free_result(res);
//
//	mysql_close(pMySql);
//	return true;
//}

bool Registration::CheckRegistration()
{
	HKEY hKey;
	long lReturn = RegOpenKeyEx(HKEY_CURRENT_USER, g_pszPath, 0, KEY_ALL_ACCESS, &hKey);
	if (lReturn == ERROR_SUCCESS)
	{
		char szName[16];
		char szKey[20];

		SIZE_T Length = sizeof(szName);
		SIZE_T LengthKey = sizeof(szKey);
		RegQueryValueEx(hKey, "hack_username", NULL, NULL, (LPBYTE)szName, &Length);
		RegQueryValueEx(hKey, "hack_key", NULL, NULL, (LPBYTE)szKey, &LengthKey);
		RegCloseKey(hKey);

		char szCorrectKey[20];
		GenerateKey(szName, strlen(szName), szCorrectKey);

		bool bChecks[2] = {false, false};
		if (!strcmp(szCorrectKey, szKey))
		{
			bChecks[0] = true;
		}

		int iHWIDLocal = GenerateHWID(szName);

#ifndef NO_ONLINE_HWID_CHECK
		if (!CheckOnlineHwID(bChecks[1], szName, szKey, iHWIDLocal))
		{
			MessageBox(NULL, "Connection to Webserver failed. Please disable your firewall.", "Error", MB_OK | MB_ICONSTOP);
			return false;
		}
#endif


		char szInfo[128];
		sprintf_s(szInfo, sizeof(szInfo), "Your Data:\nName:\t\t%s\nSerial:\t\t%s\nHwID:\t\t%d\n--------------------\n", szName, szKey, iHWIDLocal);
		char szFullText[256];
		char *email = "mark@engerisser.de";

		if (bChecks[0] == true && bChecks[1] == false)
		{
#ifndef NO_ONLINE_HWID_CHECK
			sprintf_s(szFullText, sizeof(szFullText), "%sYour serial is correct but the Online-HwID Check failed!\n\nSupport: %s", szInfo, email);
#else
			sprintf_s(szFullText, sizeof(szFullText), "%sYour serial is correct and Online-HwID Check is disabled!\n\nSupport: %s", szInfo, email);
			MessageBox(NULL, szFullText, "Check-Registration: Ok", MB_OK | MB_ICONASTERISK);
			g_bRegistered = true;
			return true;
#endif
		}
		else if (bChecks[0] == true && bChecks[1] == false)
		{
			sprintf_s(szFullText, sizeof(szFullText), "%sYour serial is incorrect, but the Online-HwID Check succeded ?!?!?!\n\n Please contact support: %s", szInfo, email);
		}
		else if (bChecks[0] == false && bChecks[1] == false)
		{
			sprintf_s(szFullText, sizeof(szFullText), "%sYour serial is incorrect and the Online-HwID Check failed!\n\nSupport: %s", szInfo, email);
		}
		else if (bChecks[0] == true && bChecks[1] == true)
		{
			sprintf_s(szFullText, sizeof(szFullText), "%sYour serial is correct and Online-HwID Check succeded ;-)", szInfo);
			MessageBox(NULL, szFullText, "Check-Registration: Ok", MB_OK | MB_ICONASTERISK);
			g_bRegistered = true;
			return true;
		}
		MessageBox(NULL, szFullText, "Registration Error", MB_OK | MB_ICONSTOP);
	}
	return false;
}

template <class T> bool Registration::SetRegistryKey(HKEY rootkey, const char *keypath, const char *key, DWORD type, T value)
{
	HKEY hKey;
	long lReturn = RegOpenKeyEx(rootkey,keypath,0,KEY_ALL_ACCESS,&hKey);

	// Falls der Schlüssel nicht existiert
	if (lReturn != ERROR_SUCCESS)
	{
		DWORD dw;

		lReturn = RegCreateKeyEx(rootkey,keypath,0,0,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,&hKey,&dw);

		if (lReturn != ERROR_SUCCESS)
		{
			//cout << "Failed to create key: [" << RootKeytoString(rootkey) << "] " << keypath << endl;
			return false;
		}
	}

	// Entscheiden ob DWORD wert oder String
	//if (!((int)(typeid(value)) == (int)(typeid(DWORD))))
	//{
		lReturn = RegSetValueEx(hKey,key,0,type,(BYTE*)value, strlen((const char*)value)+1);
	//}
	//else
	//{
	//	lReturn = RegSetValueEx(hKey,key,0,type,(BYTE*)&value, sizeof(value));
	//}

	// Alles ok!
	if (lReturn == ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return true;
	}
	
	//cout << "Failed to write Value: [" << RootKeytoString(rootkey) << "] " << keypath << " | " << key << endl;
	RegCloseKey(hKey);
	return false;
}

char *Registration::RootKeytoString(HKEY key)
{
	if (key == HKEY_CURRENT_USER)
		return "HKEY_CURRENT_USER";
	else if (key == HKEY_CLASSES_ROOT)
		return "HKEY_CLASSES_ROOT";
	else if (key == HKEY_LOCAL_MACHINE)
		return "HKEY_LOCAL_MACHINE";
	else if (key == HKEY_USERS)
		return "HKEY_USERS";
	else if (key == HKEY_CURRENT_CONFIG)
		return "HKEY_CURRENT_CONFIG";
	else
		return "Invalid Key";
}

Registration::cCRC::cCRC()
{
	crctable = 0;
}

Registration::cCRC::~cCRC()
{
	delete crctable;
}

void Registration::cCRC::generate_crc32_table()
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

DWORD Registration::cCRC::crc32(const char* s)
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

int Registration::GenerateHWID(char *szUsername)
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