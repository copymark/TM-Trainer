#include "BoostHack.h"
#include "TM2Hack.h"


CBoostHack::CBoostHack(CTM2Hack *pHack)
{
	m_bBoostEnabled = false;
	m_wBoostHotkey = 0x58; // X
	m_fBoostMulti = 5;
	m_fBoostMultiHack = 5;
	m_bUseBoostMultiHack = false;
	this->m_pHack = pHack;
}


CBoostHack::~CBoostHack()
{
}

void CBoostHack::Boost(bool bKeyDown)
{
	if (!m_bBoostEnabled)
		return;

	if (bKeyDown)
	{
		if (m_bUseBoostMultiHack)
		{
			SetBoostMulti(m_fBoostMultiHack);
		}
		DWORD dwOffsets[] = { 0x4C, 0xC8, 0x7C, 0x38, 0x198 };
		DWORD dwAddressGroundNum = m_pHack->ReadPointer(m_pHack->GetAddress("BaseP1"), dwOffsets, sizeof(dwOffsets));
		BYTE BoostValue = GROUND_ID::BOOSTER;
		m_pHack->WriteAddress(dwAddressGroundNum, &BoostValue, sizeof(BoostValue));
	}
	else
	{
		if (m_bUseBoostMultiHack)
		{
			SetBoostMulti(m_fBoostMulti);
		}
	}
}

void CBoostHack::BoostHack(void)
{
	static CCodeInjection Fix1;

	BYTE NewCode[] = { 0x81, 0x64, 0x24, 0x1C, 0xFF, 0xFF, 0xFF, 0x7F,		// and [esp+1C],7FFFFFFF
		0xF3, 0x0F, 0x59, 0x44, 0x24, 0x1C,					// mulss xmm0, [esp+1C]
		0xE9, 0x00, 0x00, 0x00, 0x00						// jmp #back#
	};

	Fix1.Initialize(m_pHack, m_pHack->GetAddress("BoostHack"), NewCode, sizeof(NewCode), 6);

	if (!m_bBoostEnabled)
	{
		Fix1.Enable();
		m_bBoostEnabled = true;
		m_pHack->SoundUpdate(SOUND_ON);
	}
	else
	{
		Fix1.Disable();
		m_bBoostEnabled = false;
		m_pHack->SoundUpdate(SOUND_OFF);
	}
}

float CBoostHack::GetBoostMulti(void)
{
	DWORD dwOffsetsSpeed[] = { 0x4C, 0xC8, 0x80, 0x28, 0xC8 };
	float fBoostMulti = 5;
	DWORD dwBoostOptionAddress = m_pHack->ReadPointer(m_pHack->GetAddress("BaseP1"), dwOffsetsSpeed, sizeof(dwOffsetsSpeed));
	m_pHack->ReadAddress(dwBoostOptionAddress, &fBoostMulti, sizeof(fBoostMulti));
	return fBoostMulti;
}
void CBoostHack::SetBoostMulti(float fNewMulti)
{
	DWORD dwOffsetsSpeed[] = { 0x4C, 0xC8, 0x80, 0x28, 0xC8 };
	DWORD dwBoostOptionAddress = m_pHack->ReadPointer(m_pHack->GetAddress("BaseP1"), dwOffsetsSpeed, sizeof(dwOffsetsSpeed));
	m_pHack->WriteAddress(dwBoostOptionAddress, &fNewMulti, sizeof(fNewMulti));
}
int CBoostHack::GetBoostDuration(void)
{
	DWORD dwOffsetsSpeed[] = { 0x4C, 0xC8, 0x80, 0x28, 0xCC };
	int iBoostDuration = 250;
	DWORD dwBoostOptionAddress = m_pHack->ReadPointer(m_pHack->GetAddress("BaseP1"), dwOffsetsSpeed, sizeof(dwOffsetsSpeed));
	m_pHack->ReadAddress(dwBoostOptionAddress, &iBoostDuration, sizeof(iBoostDuration));
	return iBoostDuration;
}
void CBoostHack::SetBoostDuration(int iNewDuration)
{
	DWORD dwOffsetsSpeed[] = { 0x4C, 0xC8, 0x80, 0x28, 0xCC };
	DWORD dwBoostOptionAddress = m_pHack->ReadPointer(m_pHack->GetAddress("BaseP1"), dwOffsetsSpeed, sizeof(dwOffsetsSpeed));
	m_pHack->WriteAddress(dwBoostOptionAddress, &iNewDuration, sizeof(iNewDuration));
}