#include "Globals.h"
#include "Config.h"

void InitConfig(void)
{
	CConfig *pConfig = GetDllGlobalConfig();

	pConfig->Initialize(&g_hWindow, "config.cfg");

	pConfig->AddOption(IDC_CHECK9, "No Wall Friction");
	pConfig->AddOption(IDC_CHECK5, "No Gras / Dirt");
	pConfig->AddOption(IDC_CHECK6, "No Free Wheeling");
	pConfig->AddOption(IDC_CHECK3, "Boost Hack");
	pConfig->AddOption(IDC_CHECK4, "No Platform Counter");
	pConfig->AddOption(IDC_CHECK2, "Checkpoint Hack");
	pConfig->AddOption(IDC_CHECK1, "Meter Hack");
	pConfig->AddOption(IDC_CHECK7, "Speed Handling Hack");
	pConfig->AddOption(IDC_CHECK8, "No Gravity");
	pConfig->AddOption(IDC_CHECK10, "Nadeo Ghosts Unlock");
	pConfig->AddOption(IDC_CHECK11, "Track unlock");
	pConfig->AddOption(IDC_CHECK12, "G15Applet");

	pConfig->Load();
}