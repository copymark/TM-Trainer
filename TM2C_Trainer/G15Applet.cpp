#include "Globals.h"

PSETSTATUSLCD SetStatusLCD = 0;
typedef void (*PADDSETTING)(HWND, int, char *);
PADDSETTING AddSetting = 0;
typedef void (*PINIT)(char*);
PINIT Initialize = 0;
typedef void (*PUNLOAD)(void);
PUNLOAD Unload = 0;

void G15Applet(void)
{
	static bool bAppletEnabled = false;
	static HINSTANCE hDLL = 0;

	if (!bAppletEnabled)
	{
		hDLL = LoadLibrary("lgLcdConnect.dll");
		if (hDLL)
		{
			SetStatusLCD = (PSETSTATUSLCD)GetProcAddress(hDLL, "SetStatusLCD");
			AddSetting = (PADDSETTING)GetProcAddress(hDLL, "AddSetting");
			Initialize = (PINIT)GetProcAddress(hDLL, "Initialize");
			Unload = (PUNLOAD)GetProcAddress(hDLL, "Unload");

			Initialize("TM2 - Ultra Trainer");

			AddSetting(g_hWindow, IDC_CHECK9, "No Wall Friction");
			AddSetting(g_hWindow, IDC_CHECK5, "No Gras / Dirt");
			AddSetting(g_hWindow, IDC_CHECK6, "No Free Wheeling");
			AddSetting(g_hWindow, IDC_CHECK3, "Boost Hack");
			AddSetting(g_hWindow, IDC_CHECK4, "No Platform Counter");
			AddSetting(g_hWindow, IDC_CHECK2, "Checkpoint Hack");
			AddSetting(g_hWindow, IDC_CHECK7, "Speed/Handling Hack");
			AddSetting(g_hWindow, IDC_BUTTON3, "Stunt Points");
			AddSetting(g_hWindow, IDC_BUTTON2, "Jump!");
			AddSetting(g_hWindow, IDC_CHECK1, "Meter Hack");
			AddSetting(g_hWindow, IDC_CHECK8, "No Gravity");
			AddSetting(g_hWindow, IDC_CHECK10, "Nadeo Ghosts Unlock");
			AddSetting(g_hWindow, IDC_CHECK11, "Track unlock");
		}
		bAppletEnabled = true;
	}
	else
	{
		Unload();
		if (hDLL)
			FreeLibrary(hDLL);
		bAppletEnabled = false;
	}
}