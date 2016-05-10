#include "MemTools.h"



CMemTools::CMemTools()
{
}


CMemTools::~CMemTools()
{
}

bool bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return false;

	return (*szMask) == NULL;
}

DWORD CMemTools::dwFindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask)
{
	for (DWORD i = 0; i < dwLen; i++)
		if (bDataCompare((BYTE*)(dwAddress + i), bMask, szMask))
			return (DWORD)(dwAddress + i);

	return 0;
}

void CMemTools::fillWithNops(DWORD dwAddress, size_t size)
{
	DWORD oldProtect;
	VirtualProtect((LPVOID)dwAddress, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	for (UINT i = 0; i < size; i++)
	{
		*(BYTE*)(dwAddress + i) = 0x90;
	}
	VirtualProtect((LPVOID)dwAddress, size, oldProtect, &oldProtect);
}

void CMemTools::changeCode(DWORD dwAddress, BYTE* pData, size_t nSize)
{
	DWORD oldProtect;
	VirtualProtect((LPVOID)dwAddress, nSize, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy((LPVOID)dwAddress, pData, nSize);
	VirtualProtect((LPVOID)dwAddress, nSize, oldProtect, &oldProtect);
}
