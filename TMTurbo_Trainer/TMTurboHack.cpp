#include "TMTurboHack.h"

CTMTurboHack *g_pTMTHack;

CTMTurboHack::CTMTurboHack(CConsole *pConsole) : cHack(GAMENAME)
{

	this->SetConsolePointer(pConsole);
	this->Start();
}


CTMTurboHack::~CTMTurboHack()
{
}

void CTMTurboHack::NoWallFriction(void)
{
	static CNop Fix;
	Fix.Initialize(this, GetAddress("nowallfriction"), 10);

	if (!Fix.IsEnabled())
	{
		Fix.Enable();
		SoundUpdate(SOUND_ON);
	}
	else
	{
		Fix.Disable();
		SoundUpdate(SOUND_OFF);
	}
}

void CTMTurboHack::NoEngineOff(void)
{
	static CNop Fix;
	Fix.Initialize(this, GetAddress("noengineoff"), 6);

	if (!Fix.IsEnabled())
	{
		Fix.Enable();
		SoundUpdate(SOUND_ON);
	}
	else
	{
		Fix.Disable();
		SoundUpdate(SOUND_OFF);
	}
}

void CTMTurboHack::DefineAddresses()
{
	m_vecAddresses.clear();

	AddAddress("nowallfriction", 0x006BDFEC);
	AddAddress("noengineoff", 0x006BFAA1);
}
