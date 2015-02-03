#pragma once
#include <Windows.h>

class CTM2Hack;

class CBoostHack
{
public:
	CBoostHack(CTM2Hack *pHack);
	~CBoostHack();

	void BoostHack(void);
	void Boost(bool bKeyDown);

	float GetBoostMulti(void);
	void SetBoostMulti(float fNewMulti);
	int GetBoostDuration(void);
	void SetBoostDuration(int iNewDuration);

private:
	CTM2Hack *m_pHack;

public:
	bool m_bBoostEnabled;
	WORD m_wBoostHotkey;
	float m_fBoostMulti;
	float m_fBoostMultiHack;
	bool m_bUseBoostMultiHack;
};

