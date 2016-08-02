#pragma once
#include "Feature.h"
class CDisableEngineOff :
	public CFeature
{
public:
	CDisableEngineOff();
	~CDisableEngineOff();
protected:
	virtual void setupAddresses();
	virtual void initialize();
};

