#include "cTrackManiaUnitedHack.h"

cTrackManiaHack *g_pTMHack = 0;

cTrackManiaHack::cTrackManiaHack(void) : cHack(GAMENAME)
{
	m_bBoostEnabled = false;
	m_bCurrentlyBoosting = false;
	m_wBoostHotkey = 0x58; // X
	m_fBoostMulti = 5;
	m_fBoostMultiHack = 5;
	m_bUseBoostMultiHack = false;
	m_GameMode = MODE_STADIUM;
	m_dwModeOffset = 0x74;
	m_wNoGravityHotkey = 0x46; // F
	m_bNoGravityEnabled = false;

	POSITION NullPos;
	NullPos.fPosX = 0; NullPos.fPosZ = 0; NullPos.fPosY = 0;
	m_SavedPos[0] = NullPos; m_SavedPos[1] = NullPos; m_SavedPos[2] = NullPos; m_SavedPos[3] = NullPos;

	this->Start();
}


cTrackManiaHack::~cTrackManiaHack(void)
{

}


void cTrackManiaHack::DefineAddresses(void)
{
	const DWORD TMMODULESTART = 0x00401000;
	const DWORD TMMODULESIZE = 0x00727000;

	m_vecAddresses.clear();

	AddAddress("BaseP1", TMMODULESTART, TMMODULESIZE, (PBYTE)"\x8B\x0D\x00\x00\x00\x00\x51\xFF\xD0\x83\xC4\x08\x39\x35\x00\x00\x00\x00\x76\x76\x39\x75\x08\x74\x43", "xx????xxxxxxxx????xxxxxxx", 0x2, true);	
	AddAddress("BaseP3", TMMODULESTART, TMMODULESIZE, (PBYTE)"\x8B\x0D\x00\x00\x00\x00\x57\x6A\x04\xE8", "xx????xxxx", 0x2, true);
	AddAddress("MeterHack1", TMMODULESTART, TMMODULESIZE, (PBYTE)"\x89\x86\x00\x00\x00\x00\x8B\x8F\x00\x00\x00\x00\x89\x8E\x00\x00\x00\x00\xD9\x6C\x24\x14", "xx????xx????xx????xxxx");
	AddAddress("MeterHack2", TMMODULESTART, TMMODULESIZE, (PBYTE)"\x89\xB3\x00\x00\x00\x00\x89\xB3\x00\x00\x00\x00\x89\xB3\x00\x00\x00\x00\x39\xB5\x00\x00\x00\x00", "xx????xx????xx????xx????");
	AddAddress("TimeCheckpointChange", TMMODULESTART, TMMODULESIZE, (PBYTE)"\x8B\x10\x89\x96\x00\x00\x00\x00\x5E\xC2\x04\x00", "xxxx????xxxx", 0x2, false);
	AddAddress("TimeNormalChange", TMMODULESTART, TMMODULESIZE, (PBYTE)"\x89\x8D\x00\x00\x00\x00\x8B\x86\x00\x00\x00\x00\x85\xC0\x74\x05", "xx????xx????xxxx");
	AddAddress("NoGrasDirt", TMMODULESTART, TMMODULESIZE, (PBYTE)"\x66\x89\x86\x28\x01\x00\x00\x8B\x4F\x40\x85\xC9", "xxxxxxxxxxxx");
	AddAddress("BoostHack", TMMODULESTART, TMMODULESIZE, (PBYTE)"\xD8\x4C\x24\x20\x8B\x00\x51\x6A\x01", "xxxxxxxxx"); // im Prinzip 2x vorhanden. Allerdings wird nur die erste benutzt, die in dem fall die richtige ist. Liegt daran, dass es noch den Turbo mit ID: 1A gibt
	AddAddress("DisableWallFriction", TMMODULESTART, TMMODULESIZE, (PBYTE)"\x89\x8B\x00\x00\x00\x00\xD9\x47\x18", "xx????xxx");
	AddAddress("DisableFreeDrive", TMMODULESTART, TMMODULESIZE, (PBYTE)"\xC7\x86\x00\x00\x00\x00\x00\x00\x00\x00\x55\x8B\xCE", "xx????????xxx");
	AddAddress("PlatformTriesCounter", TMMODULESTART, TMMODULESIZE, (PBYTE)"\x83\x87\xC4\x02\x00\x00\x01\x8B\x87\x00\x00\x00\x00", "xxxxxxxxx????");
	AddAddress("CheckpointFix1", TMMODULESTART, TMMODULESIZE, (PBYTE)"\x0F\x83\x00\x00\x00\x00\x8B\x06\x8B\x50\x10", "xx????xxxxx");
	AddAddress("CheckpointFix2", TMMODULESTART, TMMODULESIZE, (PBYTE)"\x89\x9F\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x8B\x8E", "xx????x????xx");
	AddAddress("NoRestrictions", TMMODULESTART, TMMODULESIZE, (PBYTE)"\x0F\x85\x00\x00\x00\x00\x68\x00\x00\x00\x00\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x50\x8D\x4C\x24\x14\xE8\x00\x00\x00\x00\x53\x8D\x8E", "xx????x????x????x????xxxxxx????xxx");
	AddAddress("NoTrackPassword", TMMODULESTART, TMMODULESIZE, (PBYTE)"\x0F\x85\x00\x00\x00\x00\x8B\x07\x8B\x88\x00\x00\x00\x00\x85\xC9", "xx????xxxx????xx");
	AddAddress("Version", TMMODULESTART, TMMODULESIZE, (PBYTE)"\x8B\x15\x00\x00\x00\x00\x89\x54\x24\x48\xEB\x04", "xx????xxxxxx", 0x2, true);
	AddAddress("NadeoGhosts",TMMODULESTART, TMMODULESIZE, (PBYTE)"\x75\x0D\x83\xC6\x01\x3B\xF3\x72\xD5", "xxxxxxxxx");
	AddAddress("UnlockTrackPictures", TMMODULESTART, TMMODULESIZE, (PBYTE)"\x74\x25\x8D\x4C\x24\x04\x51\x8B\x4C\x24\x18", "xxxxxxxxxxx");
	AddAddress("UnlockAllTracks", TMMODULESTART, TMMODULESIZE, (PBYTE)"\x74\x47\x89\xBE\x00\x00\x00\x00\xBB", "xxxx????x");
	AddAddress("GroundNumBase", 0x00D5772C);

	AddAddress("NoGravity", 0x0053346C);
	SetAddress("BaseP1", 0x00d74518);
	SetAddress("BaseP3", 0x00d76a7c);
	SetAddress("MeterHack1", 0x0047F71D);
	SetAddress("MeterHack2", 0x00483035);
	SetAddress("TimeCheckpointChange", 0x0047af30);
	SetAddress("TimeNormalChange", 0x0047d919);
	SetAddress("NoGrasDirt", 0x007C0D95);
	SetAddress("BoostHack", 0x007C6DE9);
	SetAddress("DisableWallFriction", 0x007C0D2E);
	SetAddress("DisableFreeDrive", 0x007C6EEC);
	SetAddress("PlatformTriesCounter", 0x0047BEEF);
	SetAddress("CheckpointFix1", 0x0048066A);
	SetAddress("CheckpointFix2", 0x00480A55);
	SetAddress("NoRestrictions", 0x00686EB8);
	SetAddress("NoTrackPassword", 0x00495BDA);
	SetAddress("NadeoGhosts", 0x004BD442);
	SetAddress("UnlockTrackPictures", 0x004EA05D);
	SetAddress("UnlockAllTracks", 0x004EC235);
}

void cTrackManiaHack::OnGameFound(void)
{
	//InjectDLL::RemoteLoadLib(m_hGameHandle, "d3dhook.dll");
}

void cTrackManiaHack::CheckpointHack(void)
{
	static CCodeChange CPFix1;
	static CNop CPFix2;

	BYTE FIX1[] = {0xE9, 0x3A, 0x01, 0x00, 0x00, 0x90};

	CPFix1.Initialize(this, GetAddress("CheckpointFix1"), FIX1, 6);
	CPFix2.Initialize(this, GetAddress("CheckpointFix2"), 11);

	if (!CPFix1.IsEnabled())
	{
		/*
		Anschalten
		*/
		int iOffset = 0x13A; BYTE *pOffset = (BYTE*)&iOffset;
		ReadAddress(GetAddress("CheckpointFix1")+2, &iOffset, 4); iOffset++;
		for (int i = 0; i < 4; i++) FIX1[i+1] = pOffset[i]; CPFix1.SetNewCode(FIX1);
		CPFix1.Enable();
		CPFix2.Enable();
		SoundUpdate(SOUND_ON);
	}
	else
	{
		/*
		Ausschalten - Daten wiederherstellen
		*/
		CPFix1.Disable();
		CPFix2.Disable();
		SoundUpdate(SOUND_OFF);
	}
}

void cTrackManiaHack::EnableNadeoGhosts(void)
{
	static CCodeChange Fix1;
	BYTE newCode[] = {0xEB};
	Fix1.Initialize(this, GetAddress("NadeoGhosts"), newCode, 1);
	if (!Fix1.IsEnabled())
	{
		/*
		Enable
		*/
		Fix1.Enable();
		SoundUpdate(SOUND_ON);
	}
	else
	{
		/*
		Disable
		*/
		Fix1.Disable();
		SoundUpdate(SOUND_OFF);
	}
}

void cTrackManiaHack::UnlockAllTracks(void)
{
	static CNop VisualUnlock;
	static CNop TrackUnlock;

	VisualUnlock.Initialize(this, GetAddress("UnlockTrackPictures"), 2);
	TrackUnlock.Initialize(this, GetAddress("UnlockAllTracks"), 2);

	if (!TrackUnlock.IsEnabled())
	{
		VisualUnlock.Enable();
		TrackUnlock.Enable();
		SoundUpdate(SOUND_ON);
	}
	else 
	{
		VisualUnlock.Disable();
		TrackUnlock.Disable();
		SoundUpdate(SOUND_OFF);
	}
}

void cTrackManiaHack::DisableTriesCounter(void)
{
	static CNop TCFix;
	TCFix.Initialize(this, GetAddress("PlatformTriesCounter"), 7);
	if (!TCFix.IsEnabled())
	{
		/*
		Anschalten
		*/
		TCFix.Enable();
		SoundUpdate(SOUND_ON);
	}
	else
	{
		/*
		Ausschalten - Daten wiederherstellen
		*/
		TCFix.Disable();
		SoundUpdate(SOUND_OFF);
	}
}

void cTrackManiaHack::SetStuntPoints(int iPoints)
{
	DWORD dwOffsets[] = {0x454, 0x28, 0x0, 0x1C, 0x2D4};
	WriteAddress(ReadPointer(GetAddress("BaseP1"), dwOffsets, sizeof(dwOffsets)), &iPoints, sizeof(iPoints));
}

DWORD cTrackManiaHack::GetPositionAddress(void)
{	
	DWORD dwPos = 0;
	DWORD dwOffsets[] = {0x188, 0x58, 0x32C, 0x0};
	dwPos = ReadPointer(GetAddress("BaseP3"), dwOffsets, sizeof(dwOffsets));
	return dwPos;
}

void cTrackManiaHack::TeleportToPos(int iIndex)
{
	if (m_SavedPos[iIndex].fPosX == 0 && m_SavedPos[iIndex].fPosZ == 0 && m_SavedPos[iIndex].fPosY == 0)
		return;

	DWORD dwPos = GetPositionAddress();

	WriteAddress(dwPos, &m_SavedDirections[iIndex], sizeof(DIRECTION));

	dwPos += 0x34;

	WriteAddress(dwPos, &m_SavedPos[iIndex], sizeof(m_SavedPos[iIndex]));
}

void cTrackManiaHack::SaveCurPos(int iIndex)
{
	DWORD dwPos = GetPositionAddress();

	DIRECTION CurDirection;
	ReadAddress(dwPos, &CurDirection, sizeof(DIRECTION));
	m_SavedDirections[iIndex] = CurDirection;

	dwPos += 0x34;

	POSITION CurPos;
	ReadAddress(dwPos, &CurPos, sizeof(CurPos));
	m_SavedPos[iIndex] = CurPos;
}

void cTrackManiaHack::SaveCurPosToSeries()
{
	DWORD dwPos = GetPositionAddress();
	dwPos += 0x34;
	POSITION CurPos;
	ReadAddress(dwPos, &CurPos, sizeof(CurPos));
	m_vecAutoPositions.push_back(CurPos);
}

void cTrackManiaHack::LoadTeleportArray()
{
	DWORD dwPos = GetPositionAddress();
	dwPos += 0x34;

	for (unsigned int i = 0; i < m_vecAutoPositions.size(); i++)
	{
		WriteAddress(dwPos, &m_vecAutoPositions[i], sizeof(m_vecAutoPositions[i]));
		Sleep(100);
	}
}

void cTrackManiaHack::MeterHack(void)
{
	static CNop MeterChangeFix;
	static CNop MeterChangeFix2;
	MeterChangeFix.Initialize(this, GetAddress("MeterHack1"), 6);
	MeterChangeFix2.Initialize(this, GetAddress("MeterHack2"), 6);
	if (!MeterChangeFix.IsEnabled())
	{
		/*
		Anschalten
		*/
		MeterChangeFix.Enable(); MeterChangeFix2.Enable();
		int iMeter = 99999999;
		DWORD dwOffsets[] = {0x454, 0x268};
		DWORD dwAddress = ReadPointer(GetAddress("BaseP1"), dwOffsets, sizeof(dwOffsets));
		WriteAddress(dwAddress, &iMeter, sizeof(iMeter));
		SoundUpdate(SOUND_ON);
	}
	else
	{
		/*
		Ausschalten - Daten wiederherstellen
		*/
		MeterChangeFix.Disable(); MeterChangeFix2.Disable();
		SoundUpdate(SOUND_OFF);
	}
}

void cTrackManiaHack::DisableWallFriction(void)
{
	static CNop WFFix;
	WFFix.Initialize(this, GetAddress("DisableWallFriction"), 6);
	if (!WFFix.IsEnabled())
	{
		/*
		Anschalten
		*/
		WFFix.Enable();
		SoundUpdate(SOUND_ON);
	}
	else
	{
		/*
		Ausschalten - Daten wiederherstellen
		*/
		WFFix.Disable();
		SoundUpdate(SOUND_OFF);
	}
}

void cTrackManiaHack::NoRestrictions(void)
{
	static CCodeChange NoRes;
	static CCodeChange NoPw;
	BYTE FIX[] = {0xE9, 0x8C, 0x01, 0x00, 0x00, 0x90};
	BYTE FIX2[] = {0xE9, 0x10, 0x02, 0x00, 0x00, 0x90};
	NoRes.Initialize(this, GetAddress("NoRestrictions"), FIX, 6);
	NoPw.Initialize(this, GetAddress("NoTrackPassword"), FIX2, 6);

	if (!NoRes.IsEnabled())
	{
		int iOffset = 0x18C; BYTE *pOffset = (BYTE*)&iOffset;
		ReadAddress(GetAddress("NoRestrictions")+2, &iOffset, 4); iOffset++;
		for (int i = 0; i < 4; i++) FIX[i+1] = pOffset[i]; NoRes.SetNewCode(FIX);

		int iOffset2 = 0x210; BYTE *pOffset2 = (BYTE*)&iOffset2;
		ReadAddress(GetAddress("NoTrackPassword")+2, &iOffset2, 4); iOffset2++;
		for (int i = 0; i < 4; i++) FIX2[i+1] = pOffset2[i]; NoPw.SetNewCode(FIX2);

		NoRes.Enable();
		NoPw.Enable();
		SoundUpdate(SOUND_ON);
	}
	else
	{
		NoRes.Disable();
		NoPw.Disable();
		SoundUpdate(SOUND_OFF);
	}
}

void cTrackManiaHack::DisableFreeDrive(void)
{
	static CNop FDFix;
	FDFix.Initialize(this, GetAddress("DisableFreeDrive"), 10);
	if (!FDFix.IsEnabled())
	{
		/*
		Anschalten
		*/
		FDFix.Enable();
		SoundUpdate(SOUND_ON);
	}
	else
	{
		/*
		Ausschalten - Daten wiederherstellen
		*/
		FDFix.Disable();
		SoundUpdate(SOUND_OFF);
	}
}

void cTrackManiaHack::NoGrasDrive(void)
{
	BYTE INJECT[] = {	0x66, 0x3D, 0x02, 0x00,						// cmp ax,0002
						0x74, 0x06,									// je (fix)
						0x66, 0x3D, 0x06, 0x00,						// cmp ax,0006
						0x75, 0x04,									// jne (orignal)
						0x66, 0xB8, 0x10, 0x00,						// mov ax, 10 (fix)
						0x66, 0x89, 0x86, 0x28, 0x01, 0x00, 0x00,	// mov [esi+00000128],ax (original)
						0xE9, 0x00, 0x00, 0x00, 0x00};				// jmp 009DA50C

	static CCodeInjection NoGrasDirtInject;
	NoGrasDirtInject.Initialize(this, GetAddress("NoGrasDirt"), INJECT, sizeof(INJECT), 7);
	if (!NoGrasDirtInject.IsInjected())
	{
		NoGrasDirtInject.Enable();
		SoundUpdate(SOUND_ON);
	}
	else
	{
		NoGrasDirtInject.Disable();
		SoundUpdate(SOUND_OFF);
	}
}

void cTrackManiaHack::Boost(bool bKeyDown)
{
	if (!m_bBoostEnabled)
		return;

	if (bKeyDown)
	{
		if (m_bUseBoostMultiHack)
		{
			SetBoostMulti(m_fBoostMultiHack);
		}

		DWORD dwOffsets[] = {0x35C, 0x0, 0x0, 0x2EC, 0x128};
		DWORD dwAddressGroundNum = ReadPointer(GetAddress("BaseP1"), dwOffsets, sizeof(dwOffsets));
		BYTE BoostValue = 0x7;
		WriteAddress(dwAddressGroundNum, &BoostValue, sizeof(BoostValue));
	}
	else
	{
		if (m_bUseBoostMultiHack)
		{
			SetBoostMulti(m_fBoostMulti);
		}
	}
}

void cTrackManiaHack::BoostHack(void)
{
	BYTE INJECTCODE[] = {	0x81, 0x64, 0x24, 0x20, 0xFF, 0xFF, 0xFF, 0x7F,	// and [esp+*8+20],7FFFFFFF
							0xD8, 0x4C, 0x24, 0x20,							// fmul dword ptr [esp+20]
							0x8B, 0x00,										// mov eax,[eax]
							0xE9, 0x8E, 0x2D, 0x0A, 0x00};					// jmp (back)

	static CCodeInjection BoostInject;
	BoostInject.Initialize(this, GetAddress("BoostHack"), INJECTCODE, sizeof(INJECTCODE), 6);

	if (!m_bBoostEnabled)
	{
		BoostInject.Enable();
		SoundUpdate(SOUND_ON);
		m_bBoostEnabled = true;
	}
	else
	{
		BoostInject.Disable();
		SoundUpdate(SOUND_OFF);
		m_bBoostEnabled = false;
	}
}

void cTrackManiaHack::DoCarJump(int iHeight)
{
	DWORD dwZPos = GetPositionAddress() + 0x34 + 0x4;

	float fPos = 0;
	ReadAddress(dwZPos, &fPos, sizeof(fPos));
	fPos += iHeight;
	WriteAddress(dwZPos, &fPos, sizeof(fPos));
}

void cTrackManiaHack::TimeFreeze(int iTimeSec, int iTimeCentiSec)
{
	BYTE bInjectCode[33] =	{	0x52,									// push edx
								0x8B, 0x96, 0xAC, 0x02, 0x00, 0x00,		// mov edx, [esi+2AC]
								0x81, 0xC2, 0x0A, 0x00, 0x00, 0x00,		// add edx, A
								0x39, 0x95, 0x74, 0x02, 0x00, 0x00,		// cmp [ebp+274], edx
								0x5A,									// pop edx
								0x73, 0x06,								// jae #nächste Zeile überspringen#
								0x89, 0x8D, 0x74, 0x02, 0x00, 0x00,		// mov [ebp+274], ecx #ORIGINAL#
								0xE9, 0xFF, 0xFF, 0xFF, 0xFF			// jmp #back#
							};
	static CCodeInjection TFreezeInject;
	TFreezeInject.Initialize(this, GetAddress("TimeNormalChange"), bInjectCode, sizeof(bInjectCode), 6);
	static CNop TFCPFix;
	TFCPFix.Initialize(this, GetAddress("TimeCheckpointChange"), 6);

	if (!TFreezeInject.IsInjected())
	{
		TFCPFix.Enable();
		TFreezeInject.Enable();

		int iTimeStop = iTimeSec * 1000 + iTimeCentiSec * 10;
		if (iTimeStop <= 0)
		{
			iTimeStop = 10;
		}
		WriteAddress(TFreezeInject.GetBaseAddress() + 9, &iTimeStop, sizeof(iTimeStop)); // Stopzeit einschreiben

		SoundUpdate(SOUND_ON);
	}
	else
	{
		TFCPFix.Disable();
		TFreezeInject.Disable();
		SoundUpdate(SOUND_OFF);
	}
}

float cTrackManiaHack::GetBoostMulti(void)
{
	DWORD dwOffsetsSpeed[] = {0x4, 0x64, 0x18, m_dwModeOffset, 0xF0};
	float fBoostMulti = 5;
	DWORD dwBoostOptionAddress = ReadPointer(GetAddress("GroundNumBase"), dwOffsetsSpeed, sizeof(dwOffsetsSpeed));
	if (dwBoostOptionAddress) ReadAddress(dwBoostOptionAddress, &fBoostMulti, sizeof(fBoostMulti));
	return fBoostMulti;
}
void cTrackManiaHack::SetBoostMulti(float fNewMulti)
{
	DWORD dwOffsetsSpeed[] = {0x4, 0x64, 0x18, m_dwModeOffset, 0xF0};
	DWORD dwBoostOptionAddress = ReadPointer(GetAddress("GroundNumBase"), dwOffsetsSpeed, sizeof(dwOffsetsSpeed));
	if (dwBoostOptionAddress) WriteAddress(dwBoostOptionAddress, &fNewMulti, sizeof(fNewMulti));
}
int cTrackManiaHack::GetBoostDuration(void)
{
	DWORD dwOffsetsSpeed[] = {0x4, 0x64, 0x18, m_dwModeOffset, 0xF8};
	int iBoostDuration = 250;
	DWORD dwBoostOptionAddress = ReadPointer(GetAddress("GroundNumBase"), dwOffsetsSpeed, sizeof(dwOffsetsSpeed));
	if (dwBoostOptionAddress) ReadAddress(dwBoostOptionAddress, &iBoostDuration, sizeof(iBoostDuration));
	return iBoostDuration;
}
void cTrackManiaHack::SetBoostDuration(int iNewDuration)
{
	DWORD dwOffsetsSpeed[] = {0x4, 0x64, 0x18, m_dwModeOffset, 0xF8};
	DWORD dwBoostOptionAddress = ReadPointer(GetAddress("GroundNumBase"), dwOffsetsSpeed, sizeof(dwOffsetsSpeed));
	if (dwBoostOptionAddress) WriteAddress(dwBoostOptionAddress, &iNewDuration, sizeof(iNewDuration));
}
void cTrackManiaHack::SetBasePointerGroundNum(int iIndex)
{
	m_GameMode = (GAME_MODE)iIndex;
	switch (m_GameMode)
	{
	case MODE_STADIUM:
		m_dwModeOffset = 0x74;
		break;
	case MODE_ISLAND:
		m_dwModeOffset = 0x54;
		break;
	case MODE_DESERT:
		m_dwModeOffset = 0x28;
		break;
	case MODE_RALLY:
		m_dwModeOffset = 0x24;
		break;
	case MODE_BAY:
		m_dwModeOffset = 0x40;
		break;
	case MODE_COAST:
		m_dwModeOffset = 0x68;
		break;
	case MODE_SNOW:
		m_dwModeOffset = 0x2C;
		break;
	default:
		m_dwModeOffset = 0x74;
		break;
	}
}

void cTrackManiaHack::CheckVersion(void)
{
	if (!this->m_hGameHandle)
	{
		MessageBox(NULL, "Start Trackmania first to do a version check", "ERROR - Invalid Game Handle", MB_ICONERROR);
		return;
	}
	char gameversion[8];
	DWORD gameVersionAddress = 0;
	ReadAddress(GetAddress("Version"), &gameVersionAddress, sizeof(DWORD));
	ReadAddress(gameVersionAddress, gameversion, sizeof(gameversion));

	char versionInfo[255];
	sprintf_s(versionInfo, sizeof(versionInfo), "Supported Version: %s\nGame Version: %s\n-----------\nIf the version differs you may have luck. The Trainer does pattern scanning so it should find the new (or older) addresses as well", GAMEVERSION, gameversion);

	MessageBox(NULL, versionInfo, "Version Check", NULL);
}

void cTrackManiaHack::NoGravity(void)
{
	static CNop Fix;
	Fix.Initialize(this, GetAddress("NoGravity"), 4);

	if (!Fix.IsEnabled())
	{
		Fix.Enable();
	}
	else
	{
		Fix.Disable();
	}
}

void cTrackManiaHack::SwitchNoGravity(void)
{
	m_bNoGravityEnabled = !m_bNoGravityEnabled;
	if (m_bNoGravityEnabled)
		SoundUpdate(SOUND_ON);
	else
		SoundUpdate(SOUND_OFF);
}
