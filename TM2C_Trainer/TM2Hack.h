#pragma once

#include "Globals.h"
#include "cHack.h"

// Tools
#include "CodeInjection.h"
#include "CodeChange.h"
#include "Nop.h"

// Features
#include "CarMover.h"
#include "BoostHack.h"


struct POSITION
{
	float fPosX;
	float fPosZ;
	float fPosY;
};

enum GROUND_ID
{
	UNDEFINED = 0,
	GRAS = 2,
	GRID = 4,
	DIRT = 6,
	BOOSTER = 7,
	STREET_BORDER = 9,
	STREET = 10,
	TURBO1 = 26,
	FREEDRIVE = 29,
	TURBO2 = 30,
};

class CTM2Hack : public cHack
{
public:
	CTM2Hack(CConsole *pConsole);
	~CTM2Hack(void);

	virtual void DefineAddresses(void);

	void MeterHack(void);
	void CheckpointHack(void);
	void NoGrasDrive(void);
	void NoPlatformCounterIncrease(void);
	void NoFreeDrive(void);
	void NoGravity(bool playSound);
	void SpeedHandlingHack(float newValue);
	void NoWallFriction(void);
	void UnlockNadeos(void);
	void UnlockTracks(void);
	void TimeFreeze(int iTimeSec, int iTimeCentiSec);

	void SwitchNoGravityStatus(void);
	void CheckVersion(void);

	/*
	Settings
	*/
	bool m_bNoGravityEnabled;
	WORD m_wNoGravityHotkey;

	/*
	TELEPORT
	*/
	DWORD GetPositionAddress(void);
	void TeleportToPos(int iIndex);
	void SaveCurPos(int iIndex);
	void SaveCurPosToSeries(void);
	void LoadTeleportArray(void);
	void DoCarJump(int iHeight);
	POSITION m_SavedPos[4];
	std::vector<POSITION> m_vecAutoPositions;

	CCarMover m_CarMover;
	CBoostHack m_BoostHack;
};

extern CTM2Hack *g_pTM2Hack;

