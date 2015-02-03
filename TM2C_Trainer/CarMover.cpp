#include "CarMover.h"
#include "TM2Hack.h"


CCarMover::CCarMover(CTM2Hack *pHack)
{
	this->m_bEnabled = false;
	this->m_pHack = pHack;
}


CCarMover::~CCarMover()
{
}

void CCarMover::SwitchStatus(void)
{
	static CNop PosChangeFixes[4];
	PosChangeFixes[0].Initialize(m_pHack, 0x5334EA, 28);
	PosChangeFixes[1].Initialize(m_pHack, 0x5337BE, 30);
	PosChangeFixes[2].Initialize(m_pHack, 0x5335C8, 30);
	PosChangeFixes[3].Initialize(m_pHack, 0x53358F, 30);

	if (!m_bEnabled)
	{
		for (int i = 0; i < 4; i++)
		{
			PosChangeFixes[i].Enable();
		}
		m_bEnabled = true;
		m_pHack->SoundUpdate(SOUND_ON);
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			PosChangeFixes[i].Disable();
		}
		m_bEnabled = false;
		m_pHack->SoundUpdate(SOUND_OFF);
	}
}

void CCarMover::MoveAxis(AXES axis, bool bDirection)
{
	if (!m_bEnabled)
	{
		return;
	}

	DWORD dwPosAddress = m_pHack->GetPositionAddress();
	switch (axis)
	{
	case AXES::X_AXIS:
		dwPosAddress += 0x0;
		break;
	case AXES::Z_AXIS:
		dwPosAddress += 0x4;
		break;
	case AXES::Y_AXIS:
		dwPosAddress += 0x8;
		break;
	}

	float fPos = 0;
	m_pHack->ReadAddress(dwPosAddress, &fPos, sizeof(fPos));
	fPos += bDirection ? MOVE_DISTANCE : -MOVE_DISTANCE;
	m_pHack->WriteAddress(dwPosAddress, &fPos, sizeof(fPos));
}
