#pragma once

#include "Globals.h"
#include "cHack.h"
#include "Console.h"

// Tools
#include "Nop.h"


class CTMTurboHack :
	public cHack
{
public:
	CTMTurboHack(CConsole *pConsole);
	~CTMTurboHack();

	void NoWallFriction(void);
	void NoEngineOff(void);

protected:
	void DefineAddresses();
};

extern CTMTurboHack *g_pTMTHack;

