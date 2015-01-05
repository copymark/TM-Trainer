#include "Helper.h"


CHelper::CHelper()
{
	this->m_bActivated = false;
	this->m_bInitialized = false;
	this->m_pHack = nullptr;
}


CHelper::~CHelper()
{
}

bool CHelper::IsEnabled(void) const
{
	return this->m_bActivated;
}

void CHelper::SwitchStatus(void) {
	if (this->IsEnabled() == true) 
	{
		this->Disable();
	}
	else 
	{
		this->Enable();
	}
}

bool CHelper::IsInitialized(void) const
{
	return (this->m_bInitialized && this->m_pHack != nullptr);
}

void CHelper::Enable(void)
{
	if (!this->IsInitialized())
	{
		return;
	}

	this->EnableFeature();

	this->m_bActivated = true;
}

void CHelper::Disable(void)
{
	if (!this->IsInitialized())
	{
		return;
	}

	this->DisableFeature();

	this->m_bActivated = false;
}

cHack *CHelper::GetHack(void) const 
{
	return this->m_pHack;
}

void CHelper::setIntializedStatus(bool status)
{
	this->m_bInitialized = status;
}

void CHelper::SetHack(cHack *pHack)
{
	this->m_pHack = pHack;
}