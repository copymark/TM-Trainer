#pragma once
#include "MemTools.h"

class CCodeModify :
	public CMemTool
{
public:
	CCodeModify(DWORD dwAddress, BYTE* newCode, size_t size);
	~CCodeModify();
};

