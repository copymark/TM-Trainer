#pragma once

#include "Globals.h"

class CSetting
{
public:
	CSetting(char *pszName, int iIDDlg, int iPos, HWND phHwndDlg);
	~CSetting(void);

	HANDLE m_hName;
	HANDLE m_hState;

	char *m_pszName;

	int m_iIDDlg;

	HWND m_phHwndDlg;

	bool CheckVarStatus(void);

	void DoAction(void);
};

