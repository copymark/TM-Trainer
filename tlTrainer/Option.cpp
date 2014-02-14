#include "Option.h"

COption::COption(char *pszName, int iIDDlg, HWND *hWindow) : m_bAutostart(false)
{
	this->m_pszName = pszName;
	this->m_hWindow = hWindow;
	this->m_iIDDlg = iIDDlg;
}


COption::~COption(void)
{
	delete this->m_pszName;
}


// Setzt den Checked Status des Objekts
void COption::SetState(bool bState)
{
	//SendDlgItemMessage(g_hWindow, this->m_iIDDlg, BM_SETCHECK, bState ? BST_CHECKED : BST_UNCHECKED, 0);
}


// Lädt die Daten aus der Datei
void COption::Load(char* pszGroup)
{
	int iValue = GetPrivateProfileInt(pszGroup, this->m_pszName, 0, g_szConfigPath);
	this->m_bAutostart = (iValue) ? true : false;
}


// Aktiviert die Option
void COption::Enable(void)
{
	if (this->m_bAutostart)
	{
		SendDlgItemMessage(*m_hWindow, this->m_iIDDlg, BM_CLICK, 0, 0);
	}
}


// speichert die Daten
void COption::Save(char* pszGroup)
{
	char cValue[2];
	sprintf_s(cValue, sizeof(cValue), "%d", this->m_bAutostart ? 1 : 0);

	WritePrivateProfileString(pszGroup, this->m_pszName, cValue, g_szConfigPath);
}
