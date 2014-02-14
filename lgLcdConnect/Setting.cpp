#include "Setting.h"


CSetting::CSetting(char *pszName, int iIDDlg, int iPos, HWND phHwndDlg)
{
	this->m_pszName = new char[strlen(pszName)+1];
	strcpy(this->m_pszName, pszName);

	this->m_iIDDlg = iIDDlg;
	this->m_phHwndDlg = phHwndDlg;

	this->m_hName = g_pLCD->AddText(LG_STATIC_TEXT, LG_MEDIUM, DT_LEFT, 120, 1, 0);
	g_pLCD->SetOrigin(this->m_hName, 10, iPos*g_iLineHeight);
	g_pLCD->SetText(this->m_hName, this->m_pszName);

	this->m_hState = g_pLCD->AddText(LG_STATIC_TEXT, LG_MEDIUM, DT_RIGHT, 40, 1, 0);
	g_pLCD->SetOrigin(this->m_hState, 120, iPos*g_iLineHeight);
	g_pLCD->SetText(this->m_hState, this->CheckVarStatus() ? "TRUE" : "FALSE");
}


CSetting::~CSetting(void)
{
	delete this->m_pszName;
}

bool CSetting::CheckVarStatus(void)
{
	int iCheck = SendDlgItemMessage(this->m_phHwndDlg, this->m_iIDDlg, BM_GETCHECK, 0, 0);
	if (iCheck == BST_CHECKED)
		return true;
	else
		return false;
}

void CSetting::DoAction(void)
{
	SendDlgItemMessage(this->m_phHwndDlg, this->m_iIDDlg, BM_CLICK, 0, 0);
	if (this->CheckVarStatus())
	{
		g_pLCD->SetText(this->m_hState, "TRUE");
	}
	else
	{
		g_pLCD->SetText(this->m_hState, "FALSE");
	}
}
