#include "Globals.h"
#include "TMTurboHack.h"
#include "Registration.h"

void ProgressBarControl(int iParam);
void UpdateStatus(char *szText);

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	try
	{
		switch (uMsg)
		{
		case WM_INITDIALOG:
		{
			/*
			Icon vom Fenster setzen
			*/
			HICON hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_ICON1));
			if (!hIcon) {
				throw (CError("Failed to load Icon", ERRORPOS));
			} else {
				SendMessage(hwndDlg, WM_SETICON, WPARAM(ICON_SMALL), LPARAM(hIcon));
			}

			/*
			Einige Dialog - Elemente funktionieren nicht ohne die Initialisierung von CommonControls
			*/
// 			INITCOMMONCONTROLSEX iccex;
// 			iccex.dwICC = ICC_PROGRESS_CLASS;
// 			iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
// 			if (!InitCommonControlsEx(&iccex))
// 				throw (CError("InitCommonControlsEx failed", ERRORPOS));

			// Global machen
			g_hWindow = hwndDlg;

			g_pTMTHack = new CTMTurboHack(g_pConsole);

			g_pTMTHack->SendStatusExtern = &UpdateStatus;
			g_pTMTHack->ProgressUpdateExtern = &ProgressBarControl;

			// Fortschrittsbalken einstellen
			//SendDlgItemMessage(hwndDlg, IDC_PROGRESS1, PBM_SETRANGE, 0, MAKELPARAM(0, g_pTMTHack->GetAddressVector()->size()));
			//SendDlgItemMessage(hwndDlg, IDC_PROGRESS1, PBM_SETSTEP, 1, 0);

			UpdateStatus("Waiting for Trackmania Turbo");

			return TRUE;
		}
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			int wmEvent = HIWORD(wParam);

			switch (wmId)
			{
			case IDC_NWALLFRICTION:
				g_pTMTHack->NoWallFriction();
				break;
			case IDC_NOENGINEOFF:
				g_pTMTHack->NoEngineOff();
				break;
			}
			break;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			break;
		}
		}
		return FALSE;
	}
	catch (CError &e)
	{
		e.Show();
		return FALSE;
	}
}

void ProgressBarControl(int iParam)
{
	//SendDlgItemMessage(g_hWindow, IDC_PROGRESS1, iParam, 0, 0);
}

void UpdateStatus(char *szText)
{
	char szNewText[255];
	sprintf_s(szNewText, 255, "Status: %s", szText);

	SetDlgItemText(g_hWindow, IDC_STATUS, szNewText);
}