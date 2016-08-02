#include "CodeModify.h"



CCodeModify::CCodeModify(DWORD dwAddress, BYTE* newCode, size_t size) :
	CMemTool::CMemTool(dwAddress, size)
{
	m_pNewData = new BYTE[m_Size];
	memcpy(m_pNewData, newCode, m_Size);
}


CCodeModify::~CCodeModify()
{
	delete[] m_pNewData;
}