#include "MemTools.h"



CMemTool::CMemTool(DWORD dwAddress, size_t size)
{
	m_Size = size;
	m_dwAddress = dwAddress;
	m_pNewData = nullptr;
	m_pOriginalData = new BYTE[m_Size];
	memcpy(m_pOriginalData, (void*)dwAddress, m_Size);
}


CMemTool::~CMemTool()
{
	delete[] m_pOriginalData;
}

void CMemTool::enable()
{
	DWORD oldProtect, temp;
	VirtualProtect((void*)m_dwAddress, m_Size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy((void*)m_dwAddress, m_pNewData, m_Size);
	VirtualProtect((void*)m_dwAddress, m_Size, oldProtect, &temp);
}

void CMemTool::disable()
{
	DWORD oldProtect, temp;
	VirtualProtect((void*)m_dwAddress, m_Size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy((void*)m_dwAddress, m_pOriginalData, m_Size);
	VirtualProtect((void*)m_dwAddress, m_Size, oldProtect, &temp);
}

bool bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return false;

	return (*szMask) == NULL;
}

DWORD CMemTool::dwFindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask)
{
	for (DWORD i = 0; i < dwLen; i++)
		if (bDataCompare((BYTE*)(dwAddress + i), bMask, szMask))
			return (DWORD)(dwAddress + i);

	return 0;
}

void CMemTool::fillWithNops(DWORD dwAddress, size_t size)
{
	DWORD oldProtect;
	VirtualProtect((LPVOID)dwAddress, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	for (UINT i = 0; i < size; i++)
	{
		*(BYTE*)(dwAddress + i) = 0x90;
	}
	VirtualProtect((LPVOID)dwAddress, size, oldProtect, &oldProtect);
}