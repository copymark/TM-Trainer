#pragma once

#include <Windows.h>
#include "MemTools.h"
#include "Address.h"

struct sSignature
{
	BYTE *pSig;
	char *szMask;
};

class CAddress
{
private:
	DWORD m_dwAddress;
	char* m_szName;

	struct sSignature* m_Signature;

	bool m_hasSignature;
	bool m_isInitialized;

public:
	CAddress(char *name, DWORD dwAddress);
	CAddress(char *name, BYTE* pSig, char* szMask, DWORD stdAddress = 0);
	~CAddress();

	void initialize();

	DWORD getAddress();
	char *getName();
};

