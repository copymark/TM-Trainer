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
	if (m_hasSignature) {
		DWORD newAddress = CMemTools::dwFindPattern(0x401000, 0x1685000, m_Signature->pSig, m_Signature->szMask);
		if (newAddress != 0) {
			m_dwAddress = newAddress;
		}
	}
	m_isInitialized = true;
}

DWORD CAddress::getAddress()
{
	return m_dwAddress;
}

char * CAddress::getName()
{
	return m_szName;
}
