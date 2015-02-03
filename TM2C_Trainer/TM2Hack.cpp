#include "TM2Hack.h"

CTM2Hack *g_pTM2Hack;

CTM2Hack::CTM2Hack(CConsole *pConsole) : 
	cHack(GAMENAME), 
	m_CarMover(this), 
	m_BoostHack(this)
{
	m_bNoGravityEnabled = false;
	m_wNoGravityHotkey = 0x46; // F

	POSITION NullPos;
	NullPos.fPosX = 0; NullPos.fPosZ = 0; NullPos.fPosY = 0;
	m_SavedPos[0] = NullPos; m_SavedPos[1] = NullPos; m_SavedPos[2] = NullPos; m_SavedPos[3] = NullPos;

	SetConsolePointer(pConsole);

	this->Start();
}

CTM2Hack::~CTM2Hack(void)
{
}

void CTM2Hack::DefineAddresses(void)
{
	const DWORD TMMODULESTART = 0x00401000;
	const DWORD TMMODULESIZE = 0x00e61000;

	m_vecAddresses.clear();

	AddAddress("BaseP1", TMMODULESTART, TMMODULESIZE, (BYTE*)"\xA1\x00\x00\x00\x00\x83\x7C\x01\x48\x00\x74\x46\xF3\x0F\x10\x87", "x????xxx?xx?xxxx", 0x1, true);
	AddAddress("MeterHack", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x89\x83\x00\x00\x00\x00\x85\xED\x74\x39", "xx????xxxx");
	AddAddress("NoPlatCount", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x40\x3B\xC1\x0F\x43\xC1", "xxxxxx");
	AddAddress("NoGrasDirt", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x66\x8B\x47\x0C\x66\x89\x46\x78\x8B\x47", "xxx?xxx?xx");
	AddAddress("NoFreeDrive", TMMODULESTART, TMMODULESIZE, (BYTE*)"\xC7\x86\x00\x00\x00\x00\x01\x00\x00\x00\x83\xBE\xA4\x03\x00\x00\x00\x74\x14", "xx????xxxxxx????xxx");
	AddAddress("NoGravity", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x89\x7C\x24\x20\xF3\x0F\x11\x46\x28\xF3\x0F\x10\x46\x2C\xF3\x0F\x58\xC3", "xxx?xxxx?xxxx?xxxx", 0x4);
	AddAddress("BoostHack", TMMODULESTART, TMMODULESIZE, (BYTE*)"\xF3\x0F\x10\x03\xF3\x0F\x59\x44\x24\x1C\xF3\x0F\x11\x86\x00\x00\x00\x00\x8B\x43\x04\x03\xC7", "xxxxxxxxx?xxxx????xx?xx", 0x4);
	AddAddress("CPFix1", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x0F\x83\x00\x00\x00\x00\x8B\x07\x68", "xx????xxx");
	AddAddress("CPFix2", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x74\x2E\xFF\xB7\x00\x00\x00\x00\x8B\x8B", "xxxx????xx");
	AddAddress("SHHack", TMMODULESTART, TMMODULESIZE, (BYTE*)"\xF3\x0F\x59\x15\x00\x00\x00\x00\xFF\x76\x04\x8B\xCF\xE8\x00\x00\x00\x00\x8B\x0E", "xxxx????xx?xxx????xx", 0x4, true);
	AddAddress("Version", TMMODULESTART, TMMODULESIZE, (BYTE*)"\xC7\x44\x24\x00\x00\x00\x00\x00\xC7\x44\x24\x00\x31\x00\x00\x00\xE8\x00\x00\x00\x00\xC7\x44\x24\x00\xFF\xFF\xFF\xFF", "xxx?????xxx?xxxxx????xxx?xxxx", 0x4, true);
	AddAddress("NadeoUnlock", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x83\xB8\xEC\x00\x00\x00\x05\x74\x5C", "xxxxxxxxx", 0x7);
	AddAddress("NoWallFriction", TMMODULESTART, TMMODULESIZE, (BYTE*)"\xC7\x80\x00\x00\x00\x00\x01\x00\x00\x00\x83\x7E\x00\x00", "xx????xxxxxx?x");
	AddAddress("TUVis", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x74\x1C\xC7\x44\x24\x0C\x00\x00\x00\x00\xC7\x44\x24\x10\x14\x00\x00\x00\xE8","xxxxx?????xxx?xxxxx");
	AddAddress("TURanking", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x0F\x84\x3B\x01\x00\x00\x8B\x87\xC0\x01\x00\x00\x3B\xC3\x0F\x84", "xx????xxxxxxxxxx");
	AddAddress("TUStart", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x74\x2A\xE8\x00\x00\x00\x00\x8B\xCF\xC7\x87\xB0\x0D\x00\x00\x01\x00\x00\x00", "xxx????xxxxxxxxxxxx");
	AddAddress("TimeFreezeChange", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x89\x10\x8B\x86\x00\x00\x00\x00\x85\xC0", "xxxx????xx");
	AddAddress("TimeFreezeFinishFix", TMMODULESTART, TMMODULESIZE, (BYTE*)"\x74\x2E\xFF\xB7", "xxxx");

	/*
	Standart Addressen definieren
	*/
	SetAddress("NoGravity", 0x00FA12CC);
	SetAddress("BaseP1", 0x18C1410);
	SetAddress("MeterHack", 0x00EF98A7);
	SetAddress("NoPlatCount", 0x00A57AAE); // WRONG!
	SetAddress("NoGrasDirt", 0x0064C412);
	SetAddress("NoFreeDrive", 0x0064B2FF);
	SetAddress("BoostHack", 0x0064B2A3);
	SetAddress("CPFix1", 0x00E9ED44);
	SetAddress("CPFix2", 0x00E9F0FF);
	SetAddress("SHHack", 0x0163FAA8);
	SetAddress("Version", 0x0157C96C);
	SetAddress("NoWallFriction", 0x0064C3C7);
	SetAddress("NadeoUnlock", 0x00D21FF6);
	SetAddress("TUVis", 0x00BA6028);
	SetAddress("TURanking", 0x00BA6344);
	SetAddress("TUStart", 0x00BA64B3);
	SetAddress("TimeFreezeChange", 0x00E989EC);
	SetAddress("TimeFreezeFinishFix", 0x00E9F0FF);
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
		0x81, 0xFA, 0xFF, 0xFF, 0xFF, 0xFF,			// cmp edx, FFFFFFFF
		0x73, 0x02,									// jae #overjump next line#
		0x89, 0x10,									// mov [eax], edx (original code)
		0x8B, 0x86, 0x20, 0x02, 0x00, 0x00,			// mov eax, [esi+220] (original code)
		0xE9, 0xC9, 0x65, 0x8D, 0xFD				// jmp #back#
	};
	TimeChangeFix.Initialize(this, GetAddress("TimeFreezeChange"), bInjectCode, sizeof(bInjectCode), 8);

	static CCodeChange FinishFix;
	BYTE JMPSHORT[] = {0xEB};
	FinishFix.Initialize(this, GetAddress("TimeFreezeFinishFix"), JMPSHORT, sizeof(JMPSHORT));

	if (!TimeChangeFix.IsEnabled())
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
	BYTE FIX1[] = {0xEB};
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
	RankingFix.Initialize(this, GetAddress("TURanking"), 6);

	static CNop RealFix;
	RealFix.Initialize(this, GetAddress("TUStart"), 2);

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
	if (!NoGrasDirtInject.IsEnabled())
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
	char gameversion[50];
	DWORD gameVersionAddress = GetAddress("Version");
	//ReadAddress(GetAddress("Version"), &gameVersionAddress, sizeof(DWORD));
	ReadAddress(gameVersionAddress, gameversion, sizeof(gameversion));

	char versionInfo[512];
	sprintf_s(versionInfo, sizeof(versionInfo), "Supported Version:\t%s\nGame Version:\t%s\n-----------\nIf the version differs you may have luck. The Trainer does pattern scanning so it should find the new (or older) addresses as well", GAMEVERSION, gameversion);

	MessageBox(NULL, versionInfo, "Version Check", NULL);
}
