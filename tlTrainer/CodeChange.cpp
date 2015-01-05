#include "CodeChange.h"


CCodeChange::CCodeChange(void)
{
	this->m_nSize = 0;
	this->m_pbOriginal = 0;
	this->m_pbNewCode = 0;
	this->m_uiAddress = 0;
}


CCodeChange::~CCodeChange(void)
{
	delete[] this->m_pbOriginal;
	delete[] this->m_pbNewCode;
}

void CCodeChange::EnableFeature(void)
{
	this->GetHack()->WriteAddress(this->m_uiAddress, this->m_pbNewCode, this->m_nSize);
}

void CCodeChange::DisableFeature(void)
{
	this->GetHack()->WriteAddress(this->m_uiAddress, this->m_pbOriginal, this->m_nSize);
}

void CCodeChange::Initialize(cHack *pHack, UINT_PTR uiAddress, BYTE *pbNewCode, SIZE_T nSize)
{
	if (this->IsEnabled())
		return;

	this->SetHack(pHack);
	this->m_uiAddress = uiAddress;
	this->m_nSize = nSize;

	delete[] this->m_pbNewCode;
	this->m_pbNewCode = new BYTE[this->m_nSize];
	memcpy(this->m_pbNewCode, pbNewCode, this->m_nSize);

	delete[] this->m_pbOriginal;
	this->m_pbOriginal = new BYTE[this->m_nSize];
	this->GetHack()->ReadAddress(this->m_uiAddress, this->m_pbOriginal, this->m_nSize);

	this->setIntializedStatus(true);
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
	delete[] this->m_pbNewCode;
	this->m_pbNewCode = new BYTE[this->m_nSize];
	memcpy(this->m_pbNewCode, pbNewCode, this->m_nSize);
}