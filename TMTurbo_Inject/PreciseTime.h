#pragma once
#include "Feature.h"


class CPreciseTime :
	public CFeature
{
private:
	BYTE* m_SavedData;
protected:
	virtual void setupAddresses();
	virtual void initialize();
public:
	CPreciseTime();
	~CPreciseTime();

	virtual void enable();
	virtual void disable();
};

