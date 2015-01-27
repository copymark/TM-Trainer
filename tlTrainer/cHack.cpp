#include "cHack.h"

LPTHREAD_START_ROUTINE FindGame(LPVOID lpParam);

cHack::cHack(char *pszGameWindowName)
{
	try
	{
		m_hGameHandle = 0;
		m_pszGameWindowName = pszGameWindowName;
		m_bLogEnabled = false;
		m_bLogWithMessageBox = false;
		m_pConsole = nullptr;

		SendStatusExtern = 0;
		ProgressUpdateExtern = 0;

		char *pcCommandLine = GetCommandLine();
		if (strstr(pcCommandLine, "log"))
		{
			m_bLogEnabled = true;
		}
		if (strstr(pcCommandLine, "logmsgbox"))
		{
			m_bLogWithMessageBox = true;
		}

	}
	catch (CError &e)
	{
		e.Show();
	}
}


cHack::~cHack(void)
{
}

void cHack::SetConsolePointer(CConsole *pConsole) {
	m_pConsole = pConsole;
}

void cHack::SoundUpdate(SndNtfy Message)
{
	try
	{
		BOOL bSuccess = TRUE;
		switch (Message)
		{
		case SOUND_ON:
			bSuccess = PlaySound(MAKEINTRESOURCE(IDR_WAVE3), g_hDllModule, SND_RESOURCE | SND_ASYNC);
			break;
		case SOUND_OFF:
			bSuccess = PlaySound(MAKEINTRESOURCE(IDR_WAVE2), g_hDllModule, SND_RESOURCE | SND_ASYNC);
			break;
		case SOUND_LOADED:
			bSuccess = PlaySound(MAKEINTRESOURCE(IDR_WAVE1), g_hDllModule, SND_RESOURCE | SND_ASYNC);
			break;
		}

		if (bSuccess == FALSE)
			throw (CError("Failed to play soundfile", ERRORPOS));
	}
	catch (CError &e)
	{
		e.Show();
	}
}
void cHack::Start(void)
{
	DefineAddresses();

	HANDLE hSuccess = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)FindGame, this, NULL, NULL);
	if (!hSuccess)
		throw (CError("Creating Thread Failed", ERRORPOS));
}

void cHack::OnGameFound(void)
{
}

void cHack::WriteAddress(DWORD dwAddress, LPVOID lpValue, SIZE_T nSize)
{
	try
	{
		if (m_hGameHandle == 0)
			throw (CError("WriteAddress - Invalid GameHandle", ERRORPOS));

		BOOL bSuccess = WriteProcessMemory(m_hGameHandle, (LPVOID)dwAddress, lpValue, nSize, NULL);
		if (!bSuccess)
			throw (CError("WriteAddress - Writing -> Area is Inaccessible", ERRORPOS));
	}
	catch (CError &e)
	{
		e.Show();
	}
}

void cHack::ReadAddress(DWORD dwAddress, LPVOID lpValue, SIZE_T nSize)
{
	try
	{
		if (m_hGameHandle == 0)
			throw (CError("ReadAddress - Invalid GameHandle", ERRORPOS));

		BOOL bSuccess = ReadProcessMemory(m_hGameHandle, (LPVOID)dwAddress, lpValue, nSize, NULL);
		if (!bSuccess)
			throw (CError("ReadAddress - Reading -> Area is Inaccessible", ERRORPOS));
	}
	catch (CError &e)
	{
		e.Show();
	}
}

void cHack::WritePointer(DWORD dwAddress, DWORD dwOffset, LPVOID lpValue, SIZE_T nSize)
{
	try
	{
		if (m_hGameHandle == 0)
			throw (CError("WritePointer - Invalid GameHandle", ERRORPOS));

		DWORD dwBaseAddressValue = 0;

		BOOL bSuccess = ReadProcessMemory(m_hGameHandle, (LPVOID)dwAddress, &dwBaseAddressValue, sizeof(DWORD), NULL);
		if (!bSuccess)
			throw (CError("WritePointer - Reading -> Area is Inaccessible", ERRORPOS));

		DWORD dwRealAddress = dwBaseAddressValue + dwOffset;

		bSuccess = WriteProcessMemory(m_hGameHandle, (LPVOID)dwRealAddress, &lpValue, nSize, NULL);
		if (!bSuccess)
			throw (CError("WritePointer - Writing -> Area is Inaccessible", ERRORPOS));
	}
	catch (CError &e)
	{
		e.Show();
	}
}

DWORD cHack::ReadPointer(DWORD dwAddress, DWORD dwOffset)
{
	try
	{
		if (m_hGameHandle == 0)
			throw (CError("ReadPointer - Invalid GameHandle", ERRORPOS));

		DWORD dwBaseAddressValue = 0;
		BOOL bSuccess = ReadProcessMemory(m_hGameHandle, (LPVOID)dwAddress, &dwBaseAddressValue, sizeof(dwBaseAddressValue), NULL);
		if (!bSuccess)
			throw (CError("ReadPointer - Reading -> Area is Inaccessible", ERRORPOS));
		return dwBaseAddressValue + dwOffset;
	}
	catch (CError &e)
	{
		e.Show();
		return 0;
	}
}

DWORD cHack::ReadPointer(DWORD dwAddress, DWORD dwOffsets[], SIZE_T nSize)
{
	try
	{
		if (m_hGameHandle == 0)
			throw (CError("ReadPointerArray - Invalid GameHandle", ERRORPOS));

		for (unsigned int i = 0; i < nSize/sizeof(DWORD); i++, dwOffsets++)
		{
			BOOL bSuccess = ReadProcessMemory(m_hGameHandle, (LPVOID)dwAddress, &dwAddress, sizeof(dwAddress), NULL);
			if (!bSuccess)
				throw (CError("ReadPointerArray - Reading -> Area is Inaccessible", ERRORPOS));
			dwAddress += *dwOffsets;		
		}
		return dwAddress;
	}
	catch (CError &e)
	{
		e.Show();
		return 0;
	}
}

LPTHREAD_START_ROUTINE FindGame(LPVOID lpParam)
{
	try
	{
		cHack *Hack = (cHack*)lpParam;
	
		while (true)
		{
			if (Hack->m_hGameHandle && FindWindow(Hack->GetGameWindowName(), NULL))
			{
				Hack->SendStatus("Everything loaded successful");
				Sleep(2000);
				continue;
			}
			Hack->ProgressUpdate(PBM_SETPOS);

			char szStatusWaiting[64];
			sprintf_s(szStatusWaiting, sizeof(szStatusWaiting), "Waiting for %s", Hack->GetGameWindowName());
			Hack->SendStatus(szStatusWaiting);
			HWND hTMWindow = 0;
			while (!hTMWindow)
			{
				hTMWindow = FindWindow(Hack->GetGameWindowName(), NULL);
				Sleep(1000);
			}
			Hack->SendStatus("Game Window found");

			DWORD dwProcessID = (DWORD)-1;
			GetWindowThreadProcessId(hTMWindow, &dwProcessID);

			if (dwProcessID == -1)
				throw (CError("Invalid Process ID", ERRORPOS));

			Hack->m_hGameHandle = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, false, dwProcessID);

			if (!Hack->m_hGameHandle)
				throw (CError("Invalid Game Handle", ERRORPOS));
			 
			Hack->OnGameFound();

			Hack->SendStatus("Loading Addresses ... please wait!");

			Hack->LoadAddressesDLL();

			Hack->SendStatus("Addresses found");
			g_Config.Enable();
			Hack->SoundUpdate(SOUND_LOADED);
		}
	}
	catch (CError &e)
	{
		e.Show();
		return 0;
	}
}

void cHack::ProgressUpdate(int iParam)
{
	if (ProgressUpdateExtern != NULL)
	{
		ProgressUpdateExtern(iParam);
	}
}

void cHack::SendStatus(char *pszText)
{
	if (SendStatusExtern != NULL)
	{
		SendStatusExtern(pszText);
	}
}

void cHack::AddAddress(char *szName, DWORD dwBaseAddress, DWORD dwLength, BYTE *pMask, char *sMask, DWORD dwOffset, bool bGetOpcode)
{
	SIGADDRESS Address;
	Address.bGetOpcode = bGetOpcode;
	Address.dwBaseAddress = dwBaseAddress;
	Address.dwLength = dwLength;
	Address.dwOffset = dwOffset;
	Address.pMask = pMask;
	Address.sMask = sMask;
	Address.szName = szName;
	Address.dwAddress = 0;

	m_vecAddresses.push_back(Address);
}

void cHack::AddAddress(char *szName, DWORD dwAddress)
{
	AddAddress(szName, 0, 0, 0, 0, 0, false);
	SetAddress(szName, dwAddress);
}

DWORD cHack::GetAddress(char *szName)
{
	try
	{
		for (unsigned int i = 0; i < m_vecAddresses.size(); i++)
		{
			if (!strcmp(szName, m_vecAddresses[i].szName))
			{
				return m_vecAddresses[i].dwAddress;
			}
		}
		throw (CError("GetAddress - Address couldn't be found", ERRORPOS));
	}
	catch (CError &e)
	{
		e.Show();
		return 0;
	}
}

void cHack::SetAddress(char *szName, DWORD dwAddress)
{
	for (unsigned int i = 0; i < m_vecAddresses.size(); i++)
	{
		if (!strcmp(szName, m_vecAddresses[i].szName))
		{
			m_vecAddresses[i].dwAddress = dwAddress;
			return;
		}
	}
}

void cHack::LoadAddressesDLL(void)
{
	// Pipe erstellen, welche die Datenübertragung von pscan.dll zum Hack übernimmt
	HANDLE hPipe = CreateNamedPipe("\\\\.\\pipe\\sigscanpipe", PIPE_ACCESS_INBOUND, PIPE_TYPE_MESSAGE, 1, 0, 0, 0, 0);
	if (hPipe == NULL || hPipe == INVALID_HANDLE_VALUE)
	{
		return;
	}

	if (m_pConsole != nullptr)
	{
		m_pConsole->SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		m_pConsole->print("\n****DEBUG-INFO***************************************************************\n* NAME                 *** Address     *** INFO                             *\n*****************************************************************************\n");
	}

	// Für jeden Address-Eintrag
	for (unsigned int i = 0; i < m_vecAddresses.size(); i++)
	{
		// Wenn der Eintrag gar keine Signatur besitzt, dann einfach überspringen und als gefunden ansehen
		if (!this->m_vecAddresses[i].sMask)
		{
			ProgressUpdate(PBM_STEPIT);
			continue;
		}

		// Loader in Process laden, der dann die Funktion findPattern in der DLL pscan.dll ausführt mit dem parameter des Eintrags
		InjectDLL::RLLStatus status = InjectDLL::RemoteLoadLoader(this->m_hGameHandle, "pscan.dll", "findPattern", &this->m_vecAddresses[i], &hPipe);

		if (status == InjectDLL::RLL_SUCCESS)
			ProgressUpdate(PBM_STEPIT);

		char *infotexts[] = {"Ok, address found", "No address found, using standard", "Unkown Error"};

		char buffer[128];
		sprintf_s(buffer, sizeof(buffer), "* %-22s:  %#-.8X   :  %-32s *\n", m_vecAddresses[i].szName, m_vecAddresses[i].dwAddress, infotexts[status]);
		if (m_pConsole != nullptr)
		{
			m_pConsole->print(buffer);
		}

		if (m_bLogEnabled)
		{

			if (m_bLogWithMessageBox)
			{
				MessageBox(NULL, buffer, "address", NULL);
			}
			else
			{
				add_log(buffer);
			}
		}
		
	}

	if (m_pConsole != nullptr)
	{
		m_pConsole->print("*****************************************************************************\n\n");
	}
	
	// Pipe wieder schließen
	CloseHandle(hPipe);
}