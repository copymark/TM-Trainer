#include "Address.h"


CAddress::CAddress(char* name, DWORD dwAddress)
{
	m_dwAddress = dwAddress;
	m_Signature = nullptr;
	m_szName = name;
	m_hasSignature = false;
	m_isInitialized = false;
}

CAddress::CAddress(char *name, BYTE* pSig, char* szMask, DWORD stdAddress /*= 0*/) : CAddress(name, stdAddress)
{
	m_Signature = new sSignature;
	m_Signature->pSig = pSig;
	m_Signature->szMask = szMask;
	m_hasSignature = true;
}

CAddress::~CAddress()
{
	delete m_Signature;
}

void CAddress::initialize()
{
	if (g_pConsole) g_pConsole->SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	if (m_hasSignature) {
		DWORD newAddress = CMemTool::dwFindPattern(TMT_MODULE_START, TMT_MODULE_SIZE, m_Signature->pSig, m_Signature->szMask);
		if (newAddress != 0) {
			m_dwAddress = newAddress;
			if (g_pConsole) g_pConsole->print("A: %s\t\t- Address found - %X", m_szName, m_dwAddress);
		}
		else
		{
			if (g_pConsole) g_pConsole->print("A: %s\t\t- Sig not found, use standard - %X", m_szName, m_dwAddress);
		}
	}
	else
	{
		if (g_pConsole) g_pConsole->print("A: %s\t\t- No Sig exists - %X", m_szName, m_dwAddress);
	}
	m_isInitialized	= true;
}

DWORD CAddress::getAddress()
{
	return m_dwAddress;
}

char* CAddress::getName()
{
	return m_szName;
}
