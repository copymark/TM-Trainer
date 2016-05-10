#pragma once

#include "MemTools.h"
#include "Address.h"
#include <vector>

using namespace std;

class CFeature
{
private:
	bool m_bEnabled = false;
	bool m_bInitialized = false;

	int m_iButtonId = -1;

	vector<CAddress*> m_vecAddresses;

protected:
	void addAddress(CAddress* adr);
	DWORD getAddress(char* szName);

	virtual void initialize();
	virtual void setupAddresses() = 0;

public:
	CFeature();
	~CFeature();

	virtual void enable();
	virtual void disable();

	void switchFeatureState();

	void checkButtonState();

	bool isEnabled();

	void setButtonId(int bId);
};

