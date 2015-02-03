#include "Globals.h"
#include "TM2Hack.h"
#include "Registration.h"

void ProgressBarControl(int iParam);
void UpdateStatus(char *szText);

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

				SetDlgItemText(hwndDlg, IDC_VERSION, VERSION);
				SetDlgItemText(hwndDlg, IDC_EDIT1, "20");
				SetDlgItemText(hwndDlg, IDC_EDIT4, "3.2");
				SetDlgItemText(hwndDlg, IDC_EDIT5, "0");
				SetDlgItemText(hwndDlg, IDC_EDIT6, "0");

				g_pTM2Hack = new CTM2Hack(g_pConsole);

				InitConfig();

				g_pTM2Hack->SendStatusExtern = &UpdateStatus;
				g_pTM2Hack->ProgressUpdateExtern = &ProgressBarControl;

				// Fortschrittsbalken einstellen
				SendDlgItemMessage(hwndDlg, IDC_PROGRESS1, PBM_SETRANGE, 0, MAKELPARAM(0, g_pTM2Hack->GetAddressVector()->size()));
				SendDlgItemMessage(hwndDlg, IDC_PROGRESS1, PBM_SETSTEP, 1, 0);

				SendDlgItemMessage(hwndDlg, IDC_HOTKEY1, HKM_SETHOTKEY, MAKEWPARAM(g_pTM2Hack->m_wNoGravityHotkey,0), 0);
				SendDlgItemMessage(hwndDlg, IDC_HOTKEY1, HKM_SETRULES, 0xFE /*any modifiers*/, MAKELPARAM(HKCOMB_NONE, 0));

				UpdateStatus("Waiting for Maniaplanet");

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
						g_pTM2Hack->MeterHack();
						break;
					}
				case IDC_BUTTON2: // CarJump
					{
						BOOL bReadSuccess = FALSE;
						int iHeightValue = GetDlgItemInt(hwndDlg, IDC_EDIT1, &bReadSuccess, true);
						if (bReadSuccess == FALSE)
							throw (CError("Can not read from edit field", ERRORPOS));

						g_pTM2Hack->DoCarJump(iHeightValue);
						break;
					}
				case IDC_CHECK2: // Checkpoint Hack
					{
						g_pTM2Hack->CheckpointHack();
						break;
					}
				case IDC_CHECK3: // BoostHack
					{
						g_pTM2Hack->m_BoostHack.BoostHack();
						break;
					}
				case IDC_BUTTON3: // Boost Options
					{
						INT_PTR iSuccess = DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG2), hwndDlg, BoostOptionsDialogProc);
						if (iSuccess == -1)
							throw (CError("Failed to create DialogBox", ERRORPOS));
						break;
					}
				case IDC_CHECK4: // No Platform Counter
					{
						g_pTM2Hack->NoPlatformCounterIncrease();
						break;
					}
				case IDC_CHECK5: // Nogras dirt
					{
						g_pTM2Hack->NoGrasDrive();
						break;
					}
				case IDC_CHECK6: // No free drive
					{
						g_pTM2Hack->NoFreeDrive();
						break;
					}
				case IDC_CHECK7: // Speed Handling hack
					{
						char szInput[8];
						GetDlgItemText(hwndDlg, IDC_EDIT4, szInput, sizeof(szInput));
						float newValue = 0;
						sscanf_s(szInput, "%f", &newValue);
						g_pTM2Hack->SpeedHandlingHack(newValue);
						break;
					}
				case IDC_CHECK8: // No Gravity
					{
						g_pTM2Hack->SwitchNoGravityStatus();
						break;
					}
				case IDC_BUTTON4: // Check version
					{
						g_pTM2Hack->CheckVersion();
						break;
					}
				case IDC_CHECK9: // NO wall friction
					{
						g_pTM2Hack->NoWallFriction();
						break;
					}
				case IDC_CHECK10: // Nadeo unlock
					{
						g_pTM2Hack->UnlockNadeos();
						break;
					}
				case IDC_CHECK11: // Track unlock
					{
						g_pTM2Hack->UnlockTracks();
						break;
					}
				case IDC_CHECK13: // Timefreeze Hack
					{
						BOOL bReadSuccess = FALSE;
						int iTime = GetDlgItemInt(hwndDlg, IDC_EDIT6, &bReadSuccess, true);
						if (bReadSuccess == FALSE)
							throw (CError("Can not read from edit field", ERRORPOS));

						int iTimeCenti = GetDlgItemInt(hwndDlg, IDC_EDIT5, &bReadSuccess, true);
						if (bReadSuccess == FALSE)
							throw (CError("Can not read from edit field", ERRORPOS));

						g_pTM2Hack->TimeFreeze(iTime, iTimeCenti);
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

void ProgressBarControl(int iParam)
{
	SendDlgItemMessage(g_hWindow, IDC_PROGRESS1, iParam, 0, 0);
}

void UpdateStatus(char *szText)
{
	char szNewText[255];
	sprintf_s(szNewText, 255, "Status: %s", szText);

	SetDlgItemText(g_hWindow, IDC_STATUS, szNewText);
}