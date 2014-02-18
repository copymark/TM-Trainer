#pragma once
#include "Globals.h"
#include "cHack.h"
#include "CodeInjection.h"
#include "Nop.h"
#include "CodeChange.h"

struct DIRECTION
{
	float fDirection;
	float fRollX;
	float fRollXMulti;
	float fRollZ;
	float fRollZMulti;
	float fRollY;
	float fRollMulti;
};

struct POSITION
{
	float fPosX;
	float fPosZ;
	float fPosY;
};

enum AXES
{
	X_AXIS,
	Z_AXIS,
	Y_AXIS
};

class cTrackManiaHack :
	public cHack
{
public:
	cTrackManiaHack(void);
	~cTrackManiaHack(void);

	virtual void DefineAddresses(void);

	void CheckVersion(void);

	/*
	Hack stuff
	*/
	void CheckpointHack(void);
	void MeterHack(void);
	void DoCarJump(int iHeight);
	void TimeFreeze(int iTimeSec, int iTimeCentiSec);
	void TeleportToPos(int iIndex);
	void SaveCurPos(int iIndex);
	void SaveCurPosToSeries(void);
	void LoadTeleportArray(void);
	void DisableWallFriction(void);
	void DisableFreeDrive(void);
	void Boost(bool bKeyDown);
	void BoostHack(void);
	void NoGrasDrive(void);
	void NoRestrictions(void);
	void EnableNadeoGhosts(void);
	void UnlockAllTracks(void);

	/*
	Car Mover
	*/
	void CarMover(void);
	void MoveAxis(AXES axis, bool bDirection);

	/*
	Get and Set of special Values
	*/
	DWORD GetPositionAddress(void);
	float GetBoostMulti(void);
	void SetBoostMulti(float fNewMulti);
	int GetBoostDuration(void);
	void SetBoostDuration(int iNewDuration);

	/*
	Settings
	*/
	bool m_bBoostEnabled;
	bool m_bCurrentlyBoosting;
	WORD m_wBoostHotkey;
	float m_fBoostMulti;
	float m_fBoostMultiHack;
	bool m_bUseBoostMultiHack;
	bool m_bCarMoverEnabled;

	/*
	TELEPORT 
	*/
	DIRECTION m_SavedDirections[4];
	POSITION m_SavedPos[4];
	std::vector<POSITION> m_vecAutoPositions;
};

extern cTrackManiaHack *g_pTMHack;