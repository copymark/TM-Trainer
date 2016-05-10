#include "Nop.h"


CNop::CNop(void)
{
}


CNop::~CNop(void)
{
}

void CNop::Initialize(cHack *pHack, UINT_PTR uiAddress, SIZE_T nSize)
{
	if (this->IsInitialized())
	{
		return;
	}

	BYTE *pNops = (BYTE*)malloc(nSize * sizeof(BYTE));
	for (unsigned int i = 0; i < nSize; i++)
	{
		pNops[i] = 0x90;
	}

	this->SetHack(pHack);
	this->m_CodeChange.Initialize(pHack, uiAddress, pNops, nSize);

	free(pNops);

	this->setIntializedStatus(true);
}

void CNop::EnableFeature(void)
{
	this->m_CodeChange.Enable();
}

void CNop::DisableFeature(void)
{
	this->m_CodeChange.Disable();
}

bool CNop::IsEnabled(void)
{
	return this->m_CodeChange.IsEnabled();
}