#include "Globals.h"

HMODULE g_hModule = 0;
CEzLcd *g_pLCD = 0;
const int g_iLineHeight = 11;

#include "Page.h"

std::vector<CPage> g_vecPages;

HANDLE g_hlcdStatusText = 0;
HICON g_hIcoRight = 0;
HICON g_hIcoLeft = 0;
HICON g_hIcoDown = 0;
HICON g_hIcoCircle = 0;

extern "C" _declspec(dllexport) void AddSetting(HWND phHwndDlg, int iID, char *pszName)
{
	bool bFoundLocation = false;
	for (unsigned int i = 0; i < g_vecPages.size(); i++)
	{
		if (g_vecPages[i].AddSetting(phHwndDlg, iID, pszName))
		{
			bFoundLocation = true;
			break;
		}
	}
	if (bFoundLocation == false)
	{
		CPage Page;
		g_vecPages.push_back(Page);
		g_vecPages.back().AddSetting(phHwndDlg, iID, pszName);
	}
}

void CheckButtons(void)
{
	if (g_pLCD->ButtonTriggered(LG_BUTTON_4))
	{
		g_pLCD->ShowPage((g_pLCD->GetCurrentPageNumber() + 1) % g_pLCD->GetPageCount());
	}
	if (g_pLCD->ButtonTriggered(LG_BUTTON_3))
	{
		g_pLCD->ShowPage((g_pLCD->GetCurrentPageNumber() + (g_pLCD->GetPageCount()-1)) % g_pLCD->GetPageCount());
	}

	int iPageNum = g_pLCD->GetCurrentPageNumber();
	if (iPageNum > 0)
	{
		if (g_pLCD->ButtonTriggered(LG_BUTTON_2))
		{
			g_vecPages[iPageNum-1].SelectionChange();
		}

		if (g_pLCD->ButtonTriggered(LG_BUTTON_1))
		{
			g_vecPages[iPageNum-1].DoAction();
		}
	}
}

HANDLE g_hThread = 0;
bool g_bThreadRun = true;
extern "C" _declspec(dllexport) DWORD WINAPI dwThread(LPVOID lParam)
{
	while (g_bThreadRun)
	{
		CheckButtons();
		g_pLCD->Update();
	}
	return 0;
}

extern "C" _declspec(dllexport) void Unload(void)
{
	try
	{
	g_bThreadRun = false;
	WaitForSingleObject(g_hThread, INFINITE);
	delete g_pLCD;
	}
	catch (...)
	{
		add_log("DLL lgLCDconnect: ERROR");
	}
}

extern "C" _declspec(dllexport) void Initialize(char *pszText)
{
	g_pLCD = new CEzLcd();

	if (g_pLCD->Initialize(pszText, LG_MONOCHROME_MODE_ONLY) == E_FAIL)
		return; // Connect zum Display gescheitert

	

	g_pLCD->SetAsForeground(TRUE);

	//	/*
	//	Seite 1, Startseite (index: 0)
	//	*/
	g_pLCD->ModifyControlsOnPage(0);

	HANDLE hTitle = g_pLCD->AddText(LG_SCROLLING_TEXT, LG_BIG, DT_CENTER, 160, 1, FW_BOLD);
	g_pLCD->SetOrigin(hTitle, 0, 1);
	char Headline[128];
	sprintf(Headline, "%s - Settings © copy-mark", pszText);
	g_pLCD->SetText(hTitle, Headline);

	HANDLE hStatusStatic = g_pLCD->AddText(LG_STATIC_TEXT, LG_MEDIUM, DT_LEFT, 35, 1, 0);
	g_pLCD->SetOrigin(hStatusStatic, 0, 19);
	g_pLCD->SetText(hStatusStatic, "Status: ");

	g_hlcdStatusText = g_pLCD->AddText(LG_SCROLLING_TEXT, LG_MEDIUM, DT_LEFT, 150, 1, 0);
	g_pLCD->SetOrigin(g_hlcdStatusText, 37, 19);
	g_pLCD->SetText(g_hlcdStatusText, "Waiting for Game-Window");

	g_hIcoRight = (HICON)LoadImage(g_hModule, MAKEINTRESOURCE(IDI_NEXT), IMAGE_ICON, 16, 16, LR_MONOCHROME);
	g_hIcoLeft = (HICON)LoadImage(g_hModule, MAKEINTRESOURCE(IDI_PREV), IMAGE_ICON, 16, 16, LR_MONOCHROME);
	g_hIcoDown = (HICON)LoadImage(g_hModule, MAKEINTRESOURCE(IDI_DOWN), IMAGE_ICON, 16, 16, LR_MONOCHROME);
	g_hIcoCircle = (HICON)LoadImage(g_hModule, MAKEINTRESOURCE(IDI_CIRCLE), IMAGE_ICON, 16, 16, LR_MONOCHROME);

	HANDLE hNextIco = g_pLCD->AddIcon(g_hIcoRight, 16, 16);
	g_pLCD->SetOrigin(hNextIco, 142, 35);

	HANDLE hPrevIco = g_pLCD->AddIcon(g_hIcoLeft, 16, 16);
	g_pLCD->SetOrigin(hPrevIco, 98, 35);

	g_hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)dwThread, 0, 0, 0);
}

extern "C" _declspec(dllexport) void SetStatusLCD(char *pszStatus)
{
	g_pLCD->SetText(g_hlcdStatusText, pszStatus);
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			g_hModule = hModule;
			break;
		}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		{

			//delete g_pLCD;
			break;
		}
	}
	return TRUE;
}