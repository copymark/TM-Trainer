#pragma once
#include "Feature.h"
class CNoWallFriction :
	public CFeature
{
public:
	CNoWallFriction();
	~CNoWallFriction();
protected:
	virtual void setupAddresses();
	virtual void initialize();
};

