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

enum GAME_MODE
{
	MODE_STADIUM,
	MODE_ISLAND,
	MODE_DESERT,
	MODE_RALLY,
	MODE_BAY,
	MODE_COAST,
	MODE_SNOW
};

class cTrackManiaHack :
	public cHack
{
public:
	cTrackManiaHack(void);
	~cTrackManiaHack(void);

	virtual void DefineAddresses(void);
	virtual void OnGameFound(void);
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
	void DisableTriesCounter(void);
	void SetStuntPoints(int iPoints);
	void NoRestrictions(void);
	void EnableNadeoGhosts(void);
	void UnlockAllTracks(void);
	void NoGravity(void);

	/*
	Get and Set of special Values
	*/
	DWORD GetPositionAddress(void);
	float GetBoostMulti(void);
	void SetBoostMulti(float fNewMulti);
	int GetBoostDuration(void);
	void SetBoostDuration(int iNewDuration);
	void SetBasePointerGroundNum(int iIndex);

	/*
	Settings
	*/
	bool m_bBoostEnabled;
	bool m_bCurrentlyBoosting;
	WORD m_wBoostHotkey;
	float m_fBoostMulti;
	float m_fBoostMultiHack;
	bool m_bUseBoostMultiHack;
	GAME_MODE m_GameMode;
	DWORD m_dwModeOffset;

	/*
	TELEPORT 
	*/
	DIRECTION m_SavedDirections[4];
	POSITION m_SavedPos[4];
	std::vector<POSITION> m_vecAutoPositions;
	unsigned short m_wNoGravityHotkey;
	boolean m_bNoGravityEnabled;
	void SwitchNoGravity(void);
};

extern cTrackManiaHack *g_pTMHack;