#pragma once

#include "DllGlobals.h"

extern char g_szConfigPath[MAX_PATH];

class COption
{
public:
	COption(char *pszName, int iIDDlg, HWND *hWindow);
	~COption(void);

	char *m_pszName;

	int m_iIDDlg;

	bool m_bAutostart;

	HWND *m_hWindow;


	// Setzt den Checked Status des Objekts
	void SetState(bool bState);
	// Lädt die Daten aus der Datei
	void Load(char* pszGroup);
	// Aktiviert die Option
	void Enable(void);
	// speichert die Daten
	void Save(char* pszGroup);
};

