#pragma once
#include "DllGlobals.h"
#include "InjectDll.h"
#include "Config.h"
#include "Console.h"

enum SndNtfy
{
	SOUND_ON,
	SOUND_OFF,
	SOUND_LOADED
};

struct SIGADDRESS
{
	/*
	Signaturspezifisch
	*/
	DWORD dwBaseAddress;
	DWORD dwLength;
	BYTE *pMask;
	char *sMask;
	DWORD dwOffset;

	bool bGetOpcode;

	/*
	Endgültige Addresse
	*/
	DWORD dwAddress;

	/*
	Name der Addresse
	*/
	char *szName;
};

class _declspec(dllexport) cHack
{
public:
	cHack(char *pszGameWindowName);
	~cHack(void);

	/*
	Setzt Addressen und startet Thread
	Muss von Abgeleiteter Klasse aufgerufen werden
	z.B Ende vom Konstruktor
	*/
	void Start(void);

	/*
	Wird aufgerufen, wenn das Gamehandle gefunden wurde
	*/
	virtual void OnGameFound(void);

	/*
	Diverse Memory-Access Funktionen
	*/
	void WriteAddress(DWORD dwAddress, LPVOID lpValue, SIZE_T nSize);
	void ReadAddress(DWORD dwAddress, LPVOID lpValue, SIZE_T nSize);
	void WritePointer(DWORD dwAddress, DWORD dwOffset, LPVOID dwValue, SIZE_T nSize);
	DWORD ReadPointer(DWORD dwAddress, DWORD dwOffset);
	DWORD ReadPointer(DWORD dwAddress, DWORD dwOffsets[], SIZE_T nSize);

	/*
	Das GameHandle!
	*/
	HANDLE m_hGameHandle;

	/*
	Sucht bei allen Einträgen die eine Signatur haben nach den Addressen
	INFO: Injected eine DLL in den Prozess, sodass dieser schnell darauf zugreifen kann
	*/
	void LoadAddressesDLL(void);

	/*
	Gibt den WindowNamen zurück
	*/
	inline const char *GetGameWindowName(void)
	{
		return m_pszGameWindowName;
	}

	/*
	Addressenverwaltungsmethoden
	*/
	void AddAddress(char *szName, DWORD dwBaseAddress, DWORD dwLength, BYTE *pMask, char *sMask, DWORD dwOffset = 0x0, bool bGetOpcode = false);
	void AddAddress(char *szName, DWORD dwAddress);
	DWORD GetAddress(char *szName);
	void SetAddress(char *szName, DWORD dwAddress);


	void (*SendStatusExtern)(char*);
	void SoundUpdate(SndNtfy Message);
	void (*ProgressUpdateExtern)(int);

	void ProgressUpdate(int iParam);
	void SendStatus(char *pszText);

	void SetConsolePointer(CConsole *pConsole);

	inline std::vector<SIGADDRESS> *GetAddressVector(void)
	{
		return &m_vecAddresses;
	}
protected:
	CConsole *m_pConsole;

	char *m_pszGameWindowName;
	bool m_bLogEnabled;
	bool m_bLogWithMessageBox;

	std::vector<SIGADDRESS> m_vecAddresses;

	virtual void DefineAddresses(void) = 0;
};