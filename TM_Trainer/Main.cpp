#include "Globals.h"
#include "cTrackManiaUnitedHack.h"
#include "DllGlobals.h"
#include "Registration.h"
#include "Config.h"

HINSTANCE g_hInstance = 0;
HWND g_hWindow = 0;

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK BoostOptionsDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

HHOOK g_hLowLevelKeyHook;
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);


/*
MAIN - EINSTIEGSPUNKT
*/
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HANDLE hMutex = CreateMutex(NULL, true, TRAINERNAME);

	if (GetLastError() == ERROR_ALREADY_EXISTS || hMutex == INVALID_HANDLE_VALUE)
		return FALSE;

	g_hInstance = hInstance;

	// "Software\\Nadeo\\TMUltraTrainer"
	Registration::SetSavePath("Software\\Nadeo\\TMUltraTrainer");

	if (!Registration::CheckRegistration())
	{
		DialogBox(GetDllModule(), MAKEINTRESOURCE(IDD_DIALOG3), NULL, Registration::RegistrationProc);
	}

	if (!Registration::IsRegistered())
		return FALSE;

	g_hLowLevelKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), NULL);
	
	CreateDialog(hInstance,MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);
	ShowWindow(g_hWindow, SW_SHOW);

	// Hauptnachrichtenschleife:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

typedef void (*PSETSTATUSLCD)(char *);
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

			AddSetting(g_hWindow, IDC_CHECK4, "No Wall Friction");
			AddSetting(g_hWindow, IDC_CHECK8, "No Gras / Dirt");
			AddSetting(g_hWindow, IDC_CHECK5, "No Free Wheeling");
			AddSetting(g_hWindow, IDC_CHECK6, "Boost Hack");
			AddSetting(g_hWindow, IDC_CHECK9, "No Platform Counter");
			AddSetting(g_hWindow, IDC_CHECK3, "Time Freeze");
			AddSetting(g_hWindow, IDC_CHECK2, "Checkpoint Hack");
			AddSetting(g_hWindow, IDC_BUTTON3, "Stunt Points");
			AddSetting(g_hWindow, IDC_BUTTON2, "Jump!");
			AddSetting(g_hWindow, IDC_CHECK1, "Meter Hack");
			AddSetting(g_hWindow, IDC_CHECK10, "No Restrictions");
			AddSetting(g_hWindow, IDC_CHECK11, "Nadeo Ghosts Unlock");
			AddSetting(g_hWindow, IDC_CHECK12, "Track unlock");
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

void InitConfig(void)
{
	CConfig *pConfig = GetDllGlobalConfig();

	pConfig->Initialize(&g_hWindow, "config.cfg");

	pConfig->AddOption(IDC_CHECK4, "No Wall Friction");
	pConfig->AddOption(IDC_CHECK8, "No Gras / Dirt");
	pConfig->AddOption(IDC_CHECK5, "No Free Wheeling");
	pConfig->AddOption(IDC_CHECK6, "Boost Hack");
	pConfig->AddOption(IDC_CHECK9, "No Platform Counter");
	pConfig->AddOption(IDC_CHECK3, "Time Freeze");
	pConfig->AddOption(IDC_CHECK2, "Checkpoint Hack");
	pConfig->AddOption(IDC_CHECK1, "Meter Hack");
	pConfig->AddOption(IDC_CHECK7, "G15Applet");
	pConfig->AddOption(IDC_CHECK10, "No Restrictions");
	pConfig->AddOption(IDC_CHECK11, "Nadeo Ghosts Unlock");
	pConfig->AddOption(IDC_CHECK12, "Track unlock");

	pConfig->Load();
}

void ProgressBarControl(int iParam)
{
	SendDlgItemMessage(g_hWindow, IDC_PROGRESS1, iParam, 0, 0);
}

void UpdateStatus(char *szText)
{
	char szNewText[255];
	sprintf_s(szNewText, 255, "Status: %s", szText);

	SetDlgItemText(g_hWindow, IDC_STATUS, szNewText);

	if (!SetStatusLCD)
		return;

	SetStatusLCD(szText);
}


INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	try
	{
		switch(uMsg)
		{
			case WM_INITDIALOG:
			{
				// Double Check^^ für Registrierung
				if (!Registration::IsRegistered())
				{
					PostQuitMessage(0);
				}

				/*
				Icon vom Fenster setzen
				*/
				HICON hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_ICON1));
				if (!hIcon)
					throw (CError("Failed to load Icon", ERRORPOS));
				SendMessage(hwndDlg, WM_SETICON, WPARAM (ICON_SMALL), LPARAM (hIcon));

				/*
				Einige Dialog - Elemente funktionieren nicht ohne die Initialisierung von CommonControls
				*/
				INITCOMMONCONTROLSEX iccex;
				iccex.dwICC = ICC_PROGRESS_CLASS;
				iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
				if (!InitCommonControlsEx(&iccex))
					throw (CError("InitCommonControlsEx failed", ERRORPOS));

				// Global machen
				g_hWindow = hwndDlg;

				/*
				Standartwerte einsetzen
				*/
				BOOL bSuccess = TRUE;
				if (!SetDlgItemText(hwndDlg, IDC_EDIT1, "5"))
					bSuccess = FALSE;
				if (!SetDlgItemText(hwndDlg, IDC_EDIT2, "0"))
					bSuccess = FALSE;
				if (!SetDlgItemText(hwndDlg, IDC_EDIT3, "0"))
					bSuccess = FALSE;
				if (!SetDlgItemText(hwndDlg, IDC_EDIT4, "99999"))
					bSuccess = FALSE;

				InitConfig();

				g_pTMHack = new cTrackManiaHack();

				g_pTMHack->SendStatusExtern = &UpdateStatus;
				g_pTMHack->ProgressUpdateExtern = &ProgressBarControl;

				// Fortschrittsbalken einstellen
				SendDlgItemMessage(hwndDlg, IDC_PROGRESS1, PBM_SETRANGE, 0, MAKELPARAM(0, g_pTMHack->GetAddressVector()->size()));
				SendDlgItemMessage(hwndDlg, IDC_PROGRESS1, PBM_SETSTEP, 1, 0);

				UpdateStatus("Waiting for TrackMania United Forever");

				if (!SetDlgItemText(hwndDlg, IDC_VERSION, VERSION))
					bSuccess = FALSE;

				if (bSuccess == FALSE)
					throw (CError("Can not set the text of the edit-field", ERRORPOS));

				return TRUE;
			}
			case WM_COMMAND:
			{
				int wmId    = LOWORD(wParam);
				int wmEvent = HIWORD(wParam);

				switch (wmId)
				{
				case IDC_CHECK1: // Meterhack
					{
						g_pTMHack->MeterHack();
						break;
					}
				case IDC_CHECK2: // Checkpoint hack
					{
						g_pTMHack->CheckpointHack();
						break;
					}
				case IDC_BUTTON2: // CarJump
					{
						BOOL bReadSuccess = FALSE;
						int iHeightValue = GetDlgItemInt(hwndDlg, IDC_EDIT1, &bReadSuccess, true);
						if (bReadSuccess == FALSE)
							throw (CError("Can not read from edit field", ERRORPOS));

						g_pTMHack->DoCarJump(iHeightValue);
						break;
					}
				case IDC_CHECK3: // TimeFreeze
					{
						BOOL bReadSuccess = FALSE;
						int iTime = GetDlgItemInt(hwndDlg, IDC_EDIT2, &bReadSuccess, true);
						if (bReadSuccess == FALSE)
							throw (CError("Can not read from edit field", ERRORPOS));

						int iTimeCenti = GetDlgItemInt(hwndDlg, IDC_EDIT3, &bReadSuccess, true);
						if (bReadSuccess == FALSE)
							throw (CError("Can not read from edit field", ERRORPOS));

						g_pTMHack->TimeFreeze(iTime, iTimeCenti);
						break;
					}
				case IDC_CHECK4: // No Wall Friction
					{
						g_pTMHack->DisableWallFriction();
						break;
					}
				case IDC_CHECK5: // No Free Drive
					{
						g_pTMHack->DisableFreeDrive();
						break;
					}
				case IDC_CHECK6: // Booster
					{
						g_pTMHack->BoostHack();
						break;
					}
				case IDC_BUTTON1: // Boost Options
					{
						INT_PTR iSuccess = DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG2), hwndDlg, BoostOptionsDialogProc);
						if (iSuccess == -1)
							throw (CError("Failed to create DialogBox", ERRORPOS));
						break;
					}
				case IDC_CHECK7: // G15 Applet
					{
						G15Applet();
						EnableWindow(GetDlgItem(hwndDlg, IDC_CHECK7), FALSE);
						break;
					}
				case IDC_CHECK8: // No Gras No Dirt Drive
					{
						g_pTMHack->NoGrasDrive();
						break;
					}
				case IDC_BUTTON3: // Stuntpoints
					{
						BOOL bReadSuccess = FALSE;
						int iPoints = GetDlgItemInt(hwndDlg, IDC_EDIT4, &bReadSuccess, true);
						if (bReadSuccess == FALSE)
							throw (CError("Can not read from edit field", ERRORPOS));

						g_pTMHack->SetStuntPoints(iPoints);
						break;
					}
				case IDC_CHECK9: // No Tries counter
					{
						g_pTMHack->DisableTriesCounter();
						break;
					}
				case IDC_CHECK10: // No Restrictions
					{
						g_pTMHack->NoRestrictions();
						break;
					}
				case IDC_BUTTON4: // Check 	compatibility
					{
						g_pTMHack->CheckVersion();
						break;
					}
				case IDC_CHECK11: // Enable Nadeo Ghosts
					{
						g_pTMHack->EnableNadeoGhosts();
						break;
					}
				case IDC_CHECK12: // Unlock all tracks
					{
						g_pTMHack->UnlockAllTracks();
						break;
					}
				case IDC_CHECK13: // No gravity
					{
						g_pTMHack->SwitchNoGravity();
						break;
					}
				}
				return TRUE;
			}
			case WM_DESTROY:
				PostQuitMessage(0);
				return TRUE;
			case WM_CLOSE:
				PostQuitMessage(0);
				return TRUE;
		}
		return FALSE;
	}
	catch (CError &e)
	{
		e.Show();
		return FALSE;
	}
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	HWND hForeground = 0;
	hForeground = GetForegroundWindow();
	char szForegroundWindowTitle[64];
	GetWindowText(hForeground, szForegroundWindowTitle, 64);

	if (!strcmp(szForegroundWindowTitle, "TrackMania United Forever"))
	{
		KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;
	
		if (nCode == HC_ACTION && wParam == WM_KEYUP)
		{
			if (pkbhs->vkCode == LOBYTE(g_pTMHack->m_wBoostHotkey))
			{
				g_pTMHack->Boost(false);
			}
			else if (pkbhs->vkCode == 0x4A) // j
			{
				SendDlgItemMessage(g_hWindow, IDC_BUTTON2, BM_CLICK, NULL, NULL);
			}
			else if (pkbhs->vkCode == LOBYTE(g_pTMHack->m_wNoGravityHotkey))
			{
				if (g_pTMHack->m_bNoGravityEnabled)
				{
					g_pTMHack->NoGravity();
				}
			}
			else if (pkbhs->vkCode == 0x46) // f
			{
				
			}
			else if (pkbhs->vkCode == VK_F1)
			{
				if (GetAsyncKeyState(VK_SHIFT))
					g_pTMHack->SaveCurPos(0);
				else
					g_pTMHack->TeleportToPos(0);
			}
			else if (pkbhs->vkCode == VK_F2)
			{
				if (GetAsyncKeyState(VK_SHIFT))
					g_pTMHack->SaveCurPos(1);
				else
					g_pTMHack->TeleportToPos(1);
			}
			else if (pkbhs->vkCode == VK_F3)
			{
				if (GetAsyncKeyState(VK_SHIFT))
					g_pTMHack->SaveCurPos(2);
				else
					g_pTMHack->TeleportToPos(2);
			}
			else if (pkbhs->vkCode == VK_F4)
			{
				if (GetAsyncKeyState(VK_SHIFT))
					g_pTMHack->SaveCurPos(3);
				else
					g_pTMHack->TeleportToPos(4);
			}
			else if (pkbhs->vkCode == VK_F8)
			{
				if (GetAsyncKeyState(VK_SHIFT))
					g_pTMHack->SaveCurPosToSeries();
				else if (GetAsyncKeyState(VK_DELETE))
					g_pTMHack->m_vecAutoPositions.clear();
				else
					g_pTMHack->LoadTeleportArray();
			}
			else if (pkbhs->vkCode == 0x59) // Y
			{
			}
		}
		else if (nCode == HC_ACTION && wParam == WM_KEYDOWN)
		{
			if (pkbhs->vkCode == LOBYTE(g_pTMHack->m_wBoostHotkey))
			{
				g_pTMHack->Boost(true);
			}
		}
	}

	return CallNextHookEx (g_hLowLevelKeyHook, nCode, wParam, lParam);
}

INT_PTR CALLBACK BoostOptionsDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
			{
				char szSpeedMulti[8];
				sprintf_s(szSpeedMulti, sizeof(szSpeedMulti), "%.3f", g_pTMHack->GetBoostMulti());
				SetDlgItemText(hwndDlg, IDC_EDIT1, szSpeedMulti);

				char szSpeedMultiHack[8];
				sprintf_s(szSpeedMultiHack, sizeof(szSpeedMultiHack), "%.3f", g_pTMHack->m_fBoostMultiHack);
				SetDlgItemText(hwndDlg, IDC_EDIT3, szSpeedMultiHack);

				char szSpeedDuration[8];
				sprintf_s(szSpeedDuration, sizeof(szSpeedDuration), "%d", g_pTMHack->GetBoostDuration());
				SetDlgItemText(hwndDlg, IDC_EDIT2, szSpeedDuration);

				SendDlgItemMessage(hwndDlg, IDC_HOTKEY1, HKM_SETHOTKEY, MAKEWPARAM(g_pTMHack->m_wBoostHotkey,0), 0);
				SendDlgItemMessage(hwndDlg, IDC_HOTKEY1, HKM_SETRULES, 0xFE /*any modifiers*/, MAKELPARAM(HKCOMB_NONE, 0));

				SendDlgItemMessage(hwndDlg, IDC_CHECK1, BM_SETSTATE, g_pTMHack->m_bUseBoostMultiHack, 0);
				EnableWindow(GetDlgItem(hwndDlg, IDC_STATIC4), g_pTMHack->m_bUseBoostMultiHack);
				EnableWindow(GetDlgItem(hwndDlg, IDC_EDIT3), g_pTMHack->m_bUseBoostMultiHack);


				/*
				Reihenfolge wichtig! Da so der richtige index
				*/
				SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Stadium");
				SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Island");
				SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Desert");
				SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Rally");
				SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Bay");
				SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Coast");
				SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Snow");								
				
				SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_SETCURSEL, (WPARAM)g_pTMHack->m_GameMode, (LPARAM)0);

				return TRUE;
			}
		case WM_COMMAND:
			{
				int wmId    = LOWORD(wParam);
				int wmEvent = HIWORD(wParam);

				switch (wmId)
				{
				case IDOK:
					{
						char szBoostMulti[8];
						GetDlgItemText(hwndDlg, IDC_EDIT1, szBoostMulti, sizeof(szBoostMulti));
						sscanf_s(szBoostMulti, "%f", &g_pTMHack->m_fBoostMulti);

						char szBoostMultiHack[8];
						GetDlgItemText(hwndDlg, IDC_EDIT3, szBoostMultiHack, sizeof(szBoostMultiHack));
						sscanf_s(szBoostMultiHack, "%f", &g_pTMHack->m_fBoostMultiHack);

						int iBoostDuration = GetDlgItemInt(hwndDlg, IDC_EDIT2, NULL, true);

						g_pTMHack->SetBoostMulti(g_pTMHack->m_fBoostMulti);
						g_pTMHack->SetBoostDuration(iBoostDuration);
						g_pTMHack->m_wBoostHotkey = LOBYTE(SendDlgItemMessage(hwndDlg, IDC_HOTKEY1, HKM_GETHOTKEY, NULL, NULL));
						EndDialog(hwndDlg, NULL);
						return TRUE;
					}
				case IDC_CHECK1:
					{
						g_pTMHack->m_bUseBoostMultiHack = !g_pTMHack->m_bUseBoostMultiHack;
						EnableWindow(GetDlgItem(hwndDlg, IDC_STATIC4), g_pTMHack->m_bUseBoostMultiHack);
						EnableWindow(GetDlgItem(hwndDlg, IDC_EDIT3), g_pTMHack->m_bUseBoostMultiHack);
						break;
					}
				case IDC_COMBO1:
					{
						switch (wmEvent)
						{
						case CBN_SELCHANGE:
							{
								int iModeIndex = SendDlgItemMessage(hwndDlg, IDC_COMBO1, CB_GETCURSEL, 0, 0);
								g_pTMHack->SetBasePointerGroundNum(iModeIndex);

								char szSpeedMulti[8];
								sprintf_s(szSpeedMulti, sizeof(szSpeedMulti), "%.3f", g_pTMHack->GetBoostMulti());
								SetDlgItemText(hwndDlg, IDC_EDIT1, szSpeedMulti);

								char szSpeedDuration[8];
								sprintf_s(szSpeedDuration, sizeof(szSpeedDuration), "%d", g_pTMHack->GetBoostDuration());
								SetDlgItemText(hwndDlg, IDC_EDIT2, szSpeedDuration);
								break;
							}
						}
						break;
					}
				}
				return TRUE;
			}
	}
	return FALSE;
}


void add_log(const char * fmt, ...)
{
	try
	{
		struct tm current_tm;
		time_t current_time;

		time(&current_time);
		localtime_s(&current_tm, &current_time);

		char logbuf[256];
		sprintf_s(logbuf, sizeof(logbuf), "-> %02d:%02d:%02d  ", current_tm.tm_hour, current_tm.tm_min, current_tm.tm_sec);

		va_list va_alist;
		va_start(va_alist, fmt);
		_vsnprintf_s(logbuf+strlen(logbuf), sizeof(logbuf) - strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
		va_end(va_alist);

		FILE *fp;
		fopen_s(&fp, "C:\\TMUF_TRAINER_LOG.txt", "a");
		if (fp)
		{
			fprintf_s(fp, "%s\n", logbuf);
			fclose(fp);
		}
	}
	catch (...)
	{
	}
}