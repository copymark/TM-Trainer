#include "CodeChange.h"


CCodeChange::CCodeChange(void)
{
	this->m_bActivated = false;
	this->m_bInitialized = false;
	this->m_nSize = 0;
	this->m_pbOriginal = 0;
	this->m_pbNewCode = 0;
	this->m_pHack = 0;
	this->m_uiAddress = 0;
}


CCodeChange::~CCodeChange(void)
{
	delete this->m_pbOriginal;
	delete this->m_pbNewCode;
}

void CCodeChange::Enable(void)
{
	if (!this->m_bInitialized)
		return;

	this->m_pHack->WriteAddress(this->m_uiAddress, this->m_pbNewCode, this->m_nSize);

	this->m_bActivated = true;
}

void CCodeChange::Disable(void)
{
	if (!this->m_bInitialized)
		return;

	this->m_pHack->WriteAddress(this->m_uiAddress, this->m_pbOriginal, this->m_nSize);

	this->m_bActivated = false;
}

void CCodeChange::Initialize(cHack *pHack, UINT_PTR uiAddress, BYTE *pbNewCode, SIZE_T nSize)
{
	if (this->m_bActivated)
		return;

	this->m_pHack = pHack;
	this->m_uiAddress = uiAddress;
	this->m_nSize = nSize;

	delete this->m_pbNewCode;
	this->m_pbNewCode = new BYTE[this->m_nSize];
	memcpy(this->m_pbNewCode, pbNewCode, this->m_nSize);

	delete this->m_pbOriginal;
	this->m_pbOriginal = new BYTE[this->m_nSize];
	this->m_pHack->ReadAddress(this->m_uiAddress, this->m_pbOriginal, this->m_nSize);

	this->m_bInitialized = true;
}

bool CCodeChange::IsEnabled(void)
{
	return this->m_bActivated;
}

void CCodeChange::ConvertEndians(LPVOID pInput, BYTE *pOutput, SIZE_T nSize)
{
	BYTE *pSource = (BYTE*)pInput;
	for (unsigned int i = 0; i < nSize; i++)
	{
		pOutput[i] = (pSource[(nSize-i-1)]);
	}
}

void CCodeChange::SetNewCode(BYTE *pbNewCode)
{
	delete this->m_pbNewCode;
	this->m_pbNewCode = new BYTE[this->m_nSize];
	memcpy(this->m_pbNewCode, pbNewCode, this->m_nSize);
}