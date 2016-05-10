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

void CFeature::initialize()
{
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
}

void CFeature::enable()
{
	m_bEnabled = true;
}

void CFeature::disable()
{
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
