#include "CodeNop.h"



CCodeNop::CCodeNop(DWORD dwAddress, size_t size) : 
	CMemTool::CMemTool(dwAddress, size)
{
	m_pNewData = new BYTE[m_Size];
	for (UINT i = 0; i < m_Size; i++)
	{
		   m_pNewData[i] = 0x90;
	}
}


CCodeNop::~CCodeNop()
{
	delete[] m_pNewData;
}
