#include "Config.h"

CConfig g_Config;

_declspec(dllexport) CConfig *GetDllGlobalConfig(void)
{
	return &g_Config;
}

char g_szConfigPath[MAX_PATH];

CConfig::CConfig(void)
{
}


CConfig::~CConfig(void)
{
}

void CConfig::Initialize(HWND *hWindow, char *pszFileName)
{
	this->m_hWindow = hWindow;

	char szDirectory[MAX_PATH];
	_getcwd(szDirectory, MAX_PATH);
	sprintf_s(g_szConfigPath, MAX_PATH, "%s\\%s", szDirectory, pszFileName);
}

// Lädt alle Optionen
void CConfig::Load(void)
{
	for (unsigned int i = 0; i < this->m_vecOptions.size(); i++)
	{
		this->m_vecOptions[i]->Load(CLASSNAME);
	}
}


// Aktiviert die entsprechenden Optionen
void CConfig::Enable(void)
{
	for (unsigned int i = 0; i < this->m_vecOptions.size(); i++)
	{
		this->m_vecOptions[i]->Enable();
	}
}


// speichert alle Optionen
void CConfig::Save(void)
{
	for (unsigned int i = 0; i < this->m_vecOptions.size(); i++)
	{
		this->m_vecOptions[i]->Save(CLASSNAME);
	}
}


void CConfig::AddOption(int iIDDlg, char* pszName)
{
	this->m_vecOptions.push_back(new COption(pszName, iIDDlg, m_hWindow));
}
