#include "Feature.h"



void CFeature::addAddress(CAddress* adr)
{
	m_vecAddresses.push_back(adr);
}

DWORD CFeature::getAddress(char* szName)
{
	for (auto add : m_vecAddresses) {
		if (!strcmp(szName, add->getName())) {
			return add->getAddress();
		}
	}
	return NULL;
}

void CFeature::addCodeChange(CMemTool* newCC)
{
	m_vecCodeChanges.push_back(newCC);
}

void CFeature::initialize()
{
	setupAddresses();
	for (auto add : m_vecAddresses) {
		add->initialize();
	}
}

void CFeature::checkButtonState()
{
	if (GetAsyncKeyState(m_iButtonId) & 1)
	{
		switchFeatureState();
	}
}

CFeature::CFeature()
{
}

CFeature::~CFeature()
{
	for (auto add : m_vecAddresses)
	{
		delete add;
	}
	
	for (auto cc : m_vecCodeChanges)
	{
		delete cc;
	}
}

void CFeature::enable()
{
	for (auto change : m_vecCodeChanges)
	{
		change->enable();
	}
	m_bEnabled = true;
}

void CFeature::disable()
{
	for (auto change : m_vecCodeChanges)
	{
		change->disable();
	}
	m_bEnabled = false;
}

void CFeature::switchFeatureState()
{
	if (m_bEnabled)
	{
		disable();
	}
	else
	{
		enable();
	}
}

bool CFeature::isEnabled()
{
	return m_bEnabled;
}

void CFeature::setButtonId(int bId)
{
	m_iButtonId = bId;
}
