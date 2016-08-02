#pragma once

#include <Windows.h>

class CMemTool
{
protected:
	BYTE* m_pNewData;
	BYTE* m_pOriginalData;
	size_t m_Size;
	DWORD m_dwAddress;

public:
	CMemTool(DWORD dwAddress, size_t size);
	~CMemTool();

	virtual void enable();
	virtual void disable();


	static DWORD dwFindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask);
	static void fillWithNops(DWORD dwAddress, size_t size);

};

