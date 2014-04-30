#include "TM2Hack.h"

CTM2Hack *g_pTM2Hack;

CTM2Hack::CTM2Hack(void) : cHack(GAMENAME)
{
	m_bBoostEnabled = false;
	m_wBoostHotkey = 0x58; // X
	m_fBoostMulti = 5;
	m_fBoostMultiHack = 5;
	m_bUseBoostMultiHack = false;
	m_bNoGravityEnabled = false;
	m_wNoGravityHotkey = 0x46; // F

	POSITION NullPos;
	NullPos.fPosX = 0; NullPos.fPosZ = 0; NullPos.fPosY = 0;
	m_SavedPos[0] = NullPos; m_SavedPos[1] = NullPos; m_SavedPos[2] = NullPos; m_SavedPos[3] = NullPos;

	this->Start();
}


CTM2Hack::~CTM2Hack(void)
{
}

void CTM2Hack::DefineAddresses(void)
{
	const DWORD TMMODULESTART = 0x00401000;
	const DWORD TMMODULESIZE = 0x00b7a000;

	m_vecAddresses.clear();

	/*AddAddress("BaseP1", TMMODULESTART, TMMODULESIZE, (BYTE*)"\xA1\x00\x00\x00\x00\x83\x7C\x02\x00\x00\x74", "x????xxx?xx", 0x1, true);
	AddAddress("MeterHack", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x89\x86\x00\x00\x00\x00\x85\xFF\x74\x00\x8B\xCF", "xx????xxx?xx");
	AddAddress("NoPlatCount", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x40\x3D\xE7\x03\x00\x00", "xxxxxx");
	AddAddress("NoGrasDirt", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x66\x89\x4E\x00\x8B\x47\x00", "xxx?xx?", -0x4);
	AddAddress("NoFreeDrive", TMMODULESTART, TMMODULESIZE, (BYTE*)"\xC7\x86\x00\x00\x00\x00\x01\x00\x00\x00\x83\xBE\x00\x00\x00\x00\x00\x74\x14", "xx????xxxxxx????xxx");
	AddAddress("NoGravity", TMMODULESTART, TMMODULESIZE, (BYTE*)"\xD8\x44\x24\x10\xD9\x5E\x28", "xxxxxxx");
	AddAddress("BoostHack", TMMODULESTART, TMMODULESIZE, (BYTE*)"\xD8\x4C\x24\x00\xD9\x9E\x00\x00\x00\x00\x8B\x47\x00\xD9\x44\x24", "xxx?xx????xx?xxx");
	AddAddress("CPFix1", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x0F\x83\x00\x00\x00\x00\x8B\x55\x00\x8B\x42", "xx????xxxxx");
	AddAddress("CPFix2", TMMODULESTART, TMMODULESIZE, (BYTE*)"\xE8\x00\x00\x00\x00\x83\xC4\x18\x85\xC0\x0F\x84\x00\x00\x00\x00\x8B\x83", "x????xx?xxxx????xx", 0xA);
	AddAddress("SHHack", TMMODULESTART, TMMODULESIZE, (BYTE*)"\xD9\x43\x00\xDC\x0D\x00\x00\x00\x00\x51\x8B\x4B\x00\xD9\x5C\x24", "xx?xx????xxx?xxx", 0x5, true);
	AddAddress("Version", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x83\xEC\x08\x8B\x0D\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x8D\x14\x24\x89\x4C\x24\x04\x8B\x4C\x24\x0C\x52\x89\x44\x24\x04\xE8\x00\x00\x00\x00\x83\xC4\x08\xC3", "xx?xx????x????xxxxxx?xxx?xxxx?x????xx?x", 0xA, true);
	AddAddress("NoWallFriction", TMMODULESTART, TMMODULESIZE, (BYTE*)"\xC7\x85\x00\x00\x00\x00\x01\x00\x00\x00\x83\x7E\x00\x00\x74", "xx????xxxxxx?xx");
	AddAddress("NadeoUnlock", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x56\x8B\xF1\x8B\x86\x00\x00\x00\x00\x8B\x48\x00\x85\xC9", "xxxxx????xx?xx");
	AddAddress("TUVis", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x83\xBE\x8C\x00\x00\x00\x00\x74\x25\x8D\x4C\x24\x04\x51\x8B\x4C\x24\x18\xC7\x44\x24\x00\x00\x00\x00\x00\xC7\x44\x24\x00\x14\x00\x00\x00\xE8\x00\x00\x00\x00\x5E\x83\xC4", "xx????xx?xxx?xxxx?xxx?????xxx?xxxxx????xxx", 0x7);
	AddAddress("TUStat", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x0F\x84\x00\x00\x00\x00\x8B\x86\x00\x00\x00\x00\x3B\xC7\x0F\x84\x00\x00\x00\x00\x3B\xC1", "xx????xx????xxxx????xx");
	AddAddress("TUReal", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x74\x26\xE8\x00\x00\x00\x00\x8B\xCE\x89\xAE", "x?x????xxxx");
	AddAddress("TimeFreezeChange", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x89\x0A\x83\xB8\x00\x00\x00\x00\x00\x5E", "xxxx????xx");
	AddAddress("TimeFreezeFinishFix", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x3B\xFE\x74\x34\x8B\x8D\x00\x00\x00\x00\x8B\x95\x00\x00\x00\x00\x8B\x83\x00\x00\x00\x00\x51", "xxxxxx????xx????xx????x", 0x2);*/


	/*
	Standart Addressen definieren
	*/
	AddAddress("NoGravity", 0x00FA12CC);
	AddAddress("BaseP1", 0x18C1410);
	AddAddress("MeterHack", 0x00EF98A7);
	AddAddress("NoPlatCount", 0x00A57AAE); // WRONG!
	AddAddress("NoGrasDirt", 0x0064C412);
	AddAddress("NoFreeDrive", 0x0064B2FF);
	AddAddress("BoostHack", 0x0064B2A3);
	AddAddress("CPFix1", 0x00E9ED44);
	AddAddress("CPFix2", 0x00E9F0FF);
	AddAddress("SHHack", 0x0163FAA8);
	AddAddress("Version", 0x0138E37C); // WRONG!
	AddAddress("NoWallFriction", 0x0064C3C7);
	AddAddress("NadeoUnlock", 0x00A7B060); // WRONG!
	AddAddress("TUVis", 0x006EC770); // WRONG!
	AddAddress("TUStat", 0x006EDD81); // WRONG!
	AddAddress("TUReal", 0x006EDF05); // WRONG!
	AddAddress("TimeFreezeChange", 0x00A565DA); // WRONG!
	AddAddress("TimeFreezeFinishFix", 0x00A5D9AE); // WRONG!
}

void CTM2Hack::NoFreeDrive(void)
{
	static CNop Fix;
	Fix.Initialize(this, GetAddress("NoFreeDrive"), 10);

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

void CTM2Hack::TimeFreeze(int iTimeSec, int iTimeCentiSec)
{
	static CCodeInjection TimeChangeFix;
	BYTE bInjectCode[] = {
		0x81, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF,			// cmp ecx, FFFFFFFF
		0x73, 0x02,									// jae #overjump next line#
		0x89, 0x0A,									// mov [edx], ecx (original code)
		0x83, 0xB8, 0x10, 0x02, 0x00, 0x00, 0x00,	// cmp dword ptr [eax+000000210],00 (original code)
		0xE9, 0xC9, 0x65, 0x8D, 0xFD				// jmp #back#
	};
	TimeChangeFix.Initialize(this, GetAddress("TimeFreezeChange"), bInjectCode, sizeof(bInjectCode), 9);

	static CCodeChange FinishFix;
	BYTE JMPSHORT[] = {0xEB};
	FinishFix.Initialize(this, GetAddress("TimeFreezeFinishFix"), JMPSHORT, sizeof(JMPSHORT));

	if (!TimeChangeFix.IsInjected())
	{
		TimeChangeFix.Enable();
		FinishFix.Enable();

		int iTimeStop = iTimeSec * 1000 + iTimeCentiSec * 10 + 10;
		if (iTimeStop <= 10)
		{
			iTimeStop = 20;
		}
		WriteAddress(TimeChangeFix.GetBaseAddress() + 2, &iTimeStop, sizeof(iTimeStop)); // Stopzeit einschreiben

		SoundUpdate(SOUND_ON);
	}
	else
	{
		TimeChangeFix.Disable();
		FinishFix.Disable();
		SoundUpdate(SOUND_OFF);
	}
}

void CTM2Hack::UnlockNadeos(void)
{
	static CCodeChange Fix;
	BYTE FIX1[] = {0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3};
	Fix.Initialize(this, GetAddress("NadeoUnlock"), FIX1, sizeof(FIX1));

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

void CTM2Hack::UnlockTracks(void)
{
	static CNop VisualFix;
	VisualFix.Initialize(this, GetAddress("TUVis"), 2);

	static CNop RankingFix;
	RankingFix.Initialize(this, GetAddress("TUStat"), 6);

	static CNop RealFix;
	RealFix.Initialize(this, GetAddress("TUReal"), 2);

	if (!VisualFix.IsEnabled())
	{
		VisualFix.Enable();
		RankingFix.Enable();
		RealFix.Enable();
		SoundUpdate(SOUND_ON);
	}
	else
	{
		VisualFix.Disable();
		RankingFix.Disable();
		RealFix.Disable();
		SoundUpdate(SOUND_OFF);
	}
}

void CTM2Hack::CheckpointHack(void)
{
	static CCodeChange Fix1;
	static CCodeChange Fix2;

	BYTE FIX1[] = {0xE9, 0x19, 0x01, 0x00, 0x00, 0x90};
	Fix1.Initialize(this, GetAddress("CPFix1"), FIX1, sizeof(FIX1));

	BYTE FIX2[] = { 0xEB, 0x2E };
	Fix2.Initialize(this, GetAddress("CPFix2"), FIX2, sizeof(FIX2));

	if (!Fix1.IsEnabled())
	{
		Fix1.Enable();
		Fix2.Enable();
		SoundUpdate(SOUND_ON);
	}
	else
	{
		Fix1.Disable();
		Fix2.Disable();
		SoundUpdate(SOUND_OFF);
	}
}

void CTM2Hack::NoPlatformCounterIncrease(void)
{
	static CNop Fix;
	Fix.Initialize(this, GetAddress("NoPlatCount"), 1);

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

void CTM2Hack::SpeedHandlingHack(float newValue)
{
	unsigned __int32 standartValue = 0x40666666; // 3.599999905;
	DWORD dwOld;
	DWORD shhack_address = GetAddress("SHHack");
	VirtualProtectEx(this->m_hGameHandle, (LPVOID)shhack_address, 4, PAGE_EXECUTE_READWRITE, &dwOld);
	static bool bEnabled = false;
	if (!bEnabled)
	{
		WriteAddress(shhack_address, &newValue, sizeof(newValue));
		SoundUpdate(SOUND_ON);
	}
	else
	{
		WriteAddress(shhack_address, &standartValue, sizeof(standartValue));
		SoundUpdate(SOUND_OFF);
	}
	bEnabled = !bEnabled;
	DWORD temp;
	VirtualProtectEx(this->m_hGameHandle, (LPVOID)shhack_address, 4, dwOld, &temp);
}

DWORD CTM2Hack::GetPositionAddress(void)
{
	DWORD dwPos = 0;
	DWORD dwOffsets[] = {0x4C, 0xC8, 0x80, 0x1C, 0x28};
	dwPos = ReadPointer(GetAddress("BaseP1"), dwOffsets, sizeof(dwOffsets));
	return dwPos;
}

void CTM2Hack::TeleportToPos(int iIndex)
{
	if (m_SavedPos[iIndex].fPosX == 0 && m_SavedPos[iIndex].fPosZ == 0 && m_SavedPos[iIndex].fPosY == 0)
		return;

	DWORD dwPos = GetPositionAddress();

	WriteAddress(dwPos, &m_SavedPos[iIndex], sizeof(m_SavedPos[iIndex]));
}

void CTM2Hack::SaveCurPos(int iIndex)
{
	DWORD dwPos = GetPositionAddress();

	POSITION CurPos;
	ReadAddress(dwPos, &CurPos, sizeof(CurPos));
	m_SavedPos[iIndex] = CurPos;
}

void CTM2Hack::SaveCurPosToSeries(void)
{
	DWORD dwPos = GetPositionAddress();

	POSITION CurPos;
	ReadAddress(dwPos, &CurPos, sizeof(CurPos));
	m_vecAutoPositions.push_back(CurPos);
}

void CTM2Hack::LoadTeleportArray(void)
{
	DWORD dwPos = GetPositionAddress();

	for (unsigned int i = 0; i < m_vecAutoPositions.size(); i++)
	{
		WriteAddress(dwPos, &m_vecAutoPositions[i], sizeof(m_vecAutoPositions[i]));
		Sleep(100);
	}
}

void CTM2Hack::DoCarJump(int iHeight)
{
	DWORD dwZPos = GetPositionAddress() + 0x4;

	float fPos = 0;
	ReadAddress(dwZPos, &fPos, sizeof(fPos));
	fPos += iHeight;
	WriteAddress(dwZPos, &fPos, sizeof(fPos));
}

void CTM2Hack::NoWallFriction(void)
{
	static CNop Fix;
	Fix.Initialize(this, GetAddress("NoWallFriction"), 10);

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

void CTM2Hack::MeterHack(void)
{
	static CCodeInjection Fix;

	BYTE newcode[] = {
		0xC7, 0x83, 0x14, 0x03, 0x00, 0x00, 0xFF, 0xE0, 0xF5, 0x05, //mov [ebx+314], 05F5E0FF
		0xE9, 0x00, 0x00, 0x00, 0x00 // jmp (back)
	};

	Fix.Initialize(this, GetAddress("MeterHack"), newcode, sizeof(newcode), 6);
						

	if (!Fix.IsInjected())
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

void CTM2Hack::NoGravity(bool playSound)
{
	static CNop Fix;
	Fix.Initialize(this, GetAddress("NoGravity"), 5);

	if (!Fix.IsEnabled())
	{
		Fix.Enable();
		if (playSound)
			SoundUpdate(SOUND_ON);
	}
	else
	{
		Fix.Disable();
		if (playSound)
			SoundUpdate(SOUND_OFF);
	}
}

void CTM2Hack::Boost(bool bKeyDown)
{
	if (!m_bBoostEnabled)
		return;

	if (bKeyDown)
	{
		if (m_bUseBoostMultiHack)
		{
			SetBoostMulti(m_fBoostMultiHack);
		}
		DWORD dwOffsets[] = {0x4C, 0xC8, 0x7C, 0x38, 0x198};
		DWORD dwAddressGroundNum = ReadPointer(GetAddress("BaseP1"), dwOffsets, sizeof(dwOffsets));
		BYTE BoostValue = GROUND_ID::BOOSTER;	
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

void CTM2Hack::BoostHack(void)
{
	static CCodeInjection Fix1;

	BYTE NewCode[] =	{	0x81, 0x64, 0x24, 0x1C, 0xFF, 0xFF, 0xFF, 0x7F,		// and [esp+1C],7FFFFFFF
							0xF3, 0x0F, 0x59, 0x44, 0x24, 0x1C,					// mulss xmm0, [esp+1C]
							0xE9, 0x00, 0x00, 0x00, 0x00						// jmp #back#
						};

	Fix1.Initialize(this, GetAddress("BoostHack"), NewCode, sizeof(NewCode), 6);

	if (!m_bBoostEnabled)
	{
		Fix1.Enable();
		m_bBoostEnabled = true;
		SoundUpdate(SOUND_ON);
	}
	else
	{
		Fix1.Disable();
		m_bBoostEnabled = false;
		SoundUpdate(SOUND_OFF);
	}
}

void CTM2Hack::NoGrasDrive(void)
{
	BYTE INJECT[] = {
		0x66, 0x8B, 0x47, 0x0C,				// mov ax, [edi+0c] -- eigentlicher wert in cx
		0x66, 0x3D, GROUND_ID::GRAS, 0x00,	// cmp ax, 0002 -- ist spieler auf gras?
		0x0F, 0x84, 0x0A, 0x00, 0x00, 0x00, // je #fix# -- wenn ja -> fix it
		0x66, 0x3D, GROUND_ID::DIRT, 0x00,	// cmp ax, 0006 -- ist spieler auf dirt?
		0x0F, 0x85, 0x04, 0x00, 0x00, 0x00,	// jne #orig# -- wenn nein dann nichts ‰ndern ansonsten eine zeile weiter
		0x66, 0xB8, GROUND_ID::STREET, 0x00,// mov ax, 0010 -- FIX: 10 = straﬂe
		0x66, 0x89, 0x46, 0x78,				// mov [esi+78], ax -- fertigen wert in var schreiben
		0xE9, 0x00, 0x00, 0x00, 0x00,		// jmp #back#
	};

	static CCodeInjection NoGrasDirtInject;
	NoGrasDirtInject.Initialize(this, GetAddress("NoGrasDirt"), INJECT, sizeof(INJECT), 8);
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

float CTM2Hack::GetBoostMulti(void)
{
	DWORD dwOffsetsSpeed[] = {0x4C, 0xC8, 0x80, 0x28, 0xC8};
	float fBoostMulti = 5;
	DWORD dwBoostOptionAddress = ReadPointer(GetAddress("BaseP1"), dwOffsetsSpeed, sizeof(dwOffsetsSpeed));
	ReadAddress(dwBoostOptionAddress, &fBoostMulti, sizeof(fBoostMulti));
	return fBoostMulti;
}
void CTM2Hack::SetBoostMulti(float fNewMulti)
{
	DWORD dwOffsetsSpeed[] = {0x4C, 0xC8, 0x80, 0x28, 0xC8};
	DWORD dwBoostOptionAddress = ReadPointer(GetAddress("BaseP1"), dwOffsetsSpeed, sizeof(dwOffsetsSpeed));
	WriteAddress(dwBoostOptionAddress, &fNewMulti, sizeof(fNewMulti));
}
int CTM2Hack::GetBoostDuration(void)
{
	DWORD dwOffsetsSpeed[] = {0x4C, 0xC8, 0x80, 0x28, 0xCC};
	int iBoostDuration = 250;
	DWORD dwBoostOptionAddress = ReadPointer(GetAddress("BaseP1"), dwOffsetsSpeed, sizeof(dwOffsetsSpeed));
	ReadAddress(dwBoostOptionAddress, &iBoostDuration, sizeof(iBoostDuration));
	return iBoostDuration;
}
void CTM2Hack::SetBoostDuration(int iNewDuration)
{
	DWORD dwOffsetsSpeed[] = {0x4C, 0xC8, 0x80, 0x28, 0xCC};
	DWORD dwBoostOptionAddress = ReadPointer(GetAddress("BaseP1"), dwOffsetsSpeed, sizeof(dwOffsetsSpeed));
	WriteAddress(dwBoostOptionAddress, &iNewDuration, sizeof(iNewDuration));
}

void CTM2Hack::SwitchNoGravityStatus(void)
{
	m_bNoGravityEnabled = !m_bNoGravityEnabled;
	if (m_bNoGravityEnabled)
		SoundUpdate(SOUND_ON);
	else
		SoundUpdate(SOUND_OFF);
}

void CTM2Hack::CheckVersion(void)
{
	char gameversion[17];
	DWORD gameVersionAddress = 0;
	ReadAddress(GetAddress("Version"), &gameVersionAddress, sizeof(DWORD));
	ReadAddress(gameVersionAddress, gameversion, sizeof(gameversion));

	char versionInfo[255];
	sprintf_s(versionInfo, sizeof(versionInfo), "Supported Version: %s\nGame Version: %s\n-----------\nIf the version differs you may have luck. The Trainer does pattern scanning so it should find the new (or older) addresses as well", GAMEVERSION, gameversion);

	MessageBox(NULL, versionInfo, "Version Check", NULL);
}
