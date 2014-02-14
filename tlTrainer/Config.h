#pragma once

#include "DllGlobals.h"
#include "Option.h"

#define CLASSNAME "SETTINGS"

class __declspec(dllexport) CConfig
{
public:
	CConfig(void);
	~CConfig(void);

	void Initialize(HWND *hWindow, char *pszFileName);

	std::vector<COption*> m_vecOptions;

	HWND *m_hWindow;
	// Lädt alle Optionen
	void Load(void);
	// Aktiviert die entsprechenden Optionen
	void Enable(void);
	// speichert alle Optionen
	void Save(void);
	void AddOption(int iIDDlg, char* pszName);
};

extern CConfig g_Config;

_declspec(dllexport) extern CConfig *GetDllGlobalConfig(void);