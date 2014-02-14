#include "Nop.h"


CNop::CNop(void)
{
}


CNop::~CNop(void)
{
}

void CNop::Initialize(cHack *pHack, UINT_PTR uiAddress, SIZE_T nSize)
{
	BYTE *pNops = new BYTE[nSize];
	for (unsigned int i = 0; i < nSize; i++)
	{
		pNops[i] = 0x90;
	}

	this->m_CodeChange.Initialize(pHack, uiAddress, pNops, nSize);

	delete pNops;
}

void CNop::Enable(void)
{
	this->m_CodeChange.Enable();
}

void CNop::Disable(void)
{
	this->m_CodeChange.Disable();
}

bool CNop::IsEnabled(void)
{
	return this->m_CodeChange.IsEnabled();
}