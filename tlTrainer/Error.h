#pragma once

#include "DllGlobals.h"

#define ERRORPOS __FILE__,__LINE__
class __declspec(dllexport) CError
{
private:
	char *m_pszText;
	char *m_pszFile;
	int m_iLine;

public:
	CError(char *pszText, char *pszFile, int iLine);
	~CError(void);
	// Gibt Messagebox mit Fehler aus
	void Show(void);
};

