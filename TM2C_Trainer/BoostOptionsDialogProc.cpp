#include "Globals.h"
#include "TM2Hack.h"

INT_PTR CALLBACK BoostOptionsDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
			{
				char szSpeedMulti[8];
				sprintf_s(szSpeedMulti, sizeof(szSpeedMulti), "%.3f", g_pTM2Hack->m_BoostHack.GetBoostMulti());
				SetDlgItemText(hwndDlg, IDC_EDIT1, szSpeedMulti);

				char szSpeedMultiHack[8];
				sprintf_s(szSpeedMultiHack, sizeof(szSpeedMultiHack), "%.3f", g_pTM2Hack->m_BoostHack.m_fBoostMultiHack);
				SetDlgItemText(hwndDlg, IDC_EDIT3, szSpeedMultiHack);

				char szSpeedDuration[8];
				sprintf_s(szSpeedDuration, sizeof(szSpeedDuration), "%d", g_pTM2Hack->m_BoostHack.GetBoostDuration());
				SetDlgItemText(hwndDlg, IDC_EDIT2, szSpeedDuration);
				
				SendDlgItemMessage(hwndDlg, IDC_HOTKEY1, HKM_SETHOTKEY, MAKEWPARAM(g_pTM2Hack->m_BoostHack.m_wBoostHotkey, 0), 0);
				SendDlgItemMessage(hwndDlg, IDC_HOTKEY1, HKM_SETRULES, 0xFE /*any modifiers*/, MAKELPARAM(HKCOMB_NONE, 0));


				SendDlgItemMessage(hwndDlg, IDC_CHECK1, BM_SETSTATE, g_pTM2Hack->m_BoostHack.m_bUseBoostMultiHack, 0);
				EnableWindow(GetDlgItem(hwndDlg, IDC_STATIC4), g_pTM2Hack->m_BoostHack.m_bUseBoostMultiHack);
				EnableWindow(GetDlgItem(hwndDlg, IDC_EDIT3), g_pTM2Hack->m_BoostHack.m_bUseBoostMultiHack);

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
						sscanf_s(szBoostMulti, "%f", &g_pTM2Hack->m_BoostHack.m_fBoostMulti);

						char szBoostMultiHack[8];
						GetDlgItemText(hwndDlg, IDC_EDIT3, szBoostMultiHack, sizeof(szBoostMultiHack));
						sscanf_s(szBoostMultiHack, "%f", &g_pTM2Hack->m_BoostHack.m_fBoostMultiHack);

						int iBoostDuration = GetDlgItemInt(hwndDlg, IDC_EDIT2, NULL, true);

						g_pTM2Hack->m_BoostHack.SetBoostMulti(g_pTM2Hack->m_BoostHack.m_fBoostMulti);
						g_pTM2Hack->m_BoostHack.SetBoostDuration(iBoostDuration);
						g_pTM2Hack->m_BoostHack.m_wBoostHotkey = LOBYTE(SendDlgItemMessage(hwndDlg, IDC_HOTKEY1, HKM_GETHOTKEY, NULL, NULL));
						EndDialog(hwndDlg, NULL);
						return TRUE;
					}
				case IDC_CHECK1:
					{
						g_pTM2Hack->m_BoostHack.m_bUseBoostMultiHack = !g_pTM2Hack->m_BoostHack.m_bUseBoostMultiHack;
						EnableWindow(GetDlgItem(hwndDlg, IDC_STATIC4), g_pTM2Hack->m_BoostHack.m_bUseBoostMultiHack);
						EnableWindow(GetDlgItem(hwndDlg, IDC_EDIT3), g_pTM2Hack->m_BoostHack.m_bUseBoostMultiHack);
						break;
					}
				}
				return TRUE;
			}
	}
	return FALSE;
}