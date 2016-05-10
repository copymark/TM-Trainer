#pragma once

#include <Windows.h>

class CMemTools
{
public:
	CMemTools();
	~CMemTools();

	static DWORD dwFindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask);
	static void fillWithNops(DWORD dwAddress, size_t size);

	static void changeCode(DWORD dwAddress, BYTE* pData, size_t nSize);

};

