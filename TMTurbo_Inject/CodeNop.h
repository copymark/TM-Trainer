#pragma once
#include "MemTools.h"

class CCodeNop :
	public CMemTool
{
public:
	CCodeNop(DWORD dwAddress, size_t size);
	~CCodeNop();
};

