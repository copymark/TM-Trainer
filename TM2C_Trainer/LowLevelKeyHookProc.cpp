#include "Globals.h"
#include "TM2Hack.h"

HHOOK g_hLowLevelKeyHook;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	HWND hForeground = 0;
	hForeground = GetForegroundWindow();
	char szForegroundWindowTitle[64];
	GetWindowText(hForeground, szForegroundWindowTitle, 64);

	if (!strcmp(szForegroundWindowTitle, GAMENAME))
	{
		KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;
	
		if (nCode == HC_ACTION && wParam == WM_KEYUP)
		{
			if (pkbhs->vkCode == LOBYTE(g_pTM2Hack->m_BoostHack.m_wBoostHotkey))
			{
				g_pTM2Hack->m_BoostHack.Boost(false);
			}
			else if (pkbhs->vkCode == 0x4A) // j
			{
				SendDlgItemMessage(g_hWindow, IDC_BUTTON2, BM_CLICK, NULL, NULL);
			}
			else if (pkbhs->vkCode == LOBYTE(g_pTM2Hack->m_wNoGravityHotkey))
			{
				if (g_pTM2Hack->m_bNoGravityEnabled)
				{
					g_pTM2Hack->NoGravity(false);
				}
			}
			else if (pkbhs->vkCode == VK_F1)
			{
				if (GetAsyncKeyState(VK_SHIFT))
					g_pTM2Hack->SaveCurPos(0);
				else
					g_pTM2Hack->TeleportToPos(0);
			}
			else if (pkbhs->vkCode == VK_F2)
			{
				if (GetAsyncKeyState(VK_SHIFT))
					g_pTM2Hack->SaveCurPos(1);
				else
					g_pTM2Hack->TeleportToPos(1);
			}
			else if (pkbhs->vkCode == VK_F3)
			{
				if (GetAsyncKeyState(VK_SHIFT))
					g_pTM2Hack->SaveCurPos(2);
				else
					g_pTM2Hack->TeleportToPos(2);
			}
			else if (pkbhs->vkCode == VK_F4)
			{
				if (GetAsyncKeyState(VK_SHIFT))
					g_pTM2Hack->SaveCurPos(3);
				else
					g_pTM2Hack->TeleportToPos(3);
			}
			else if (pkbhs->vkCode == VK_F8)
			{
				if (GetAsyncKeyState(VK_SHIFT))
					g_pTM2Hack->SaveCurPosToSeries();
				else if (GetAsyncKeyState(VK_DELETE))
					g_pTM2Hack->m_vecAutoPositions.clear();
				else
					g_pTM2Hack->LoadTeleportArray();
			}
		}
		else if (nCode == HC_ACTION && wParam == WM_KEYDOWN)
		{
			if (pkbhs->vkCode == LOBYTE(g_pTM2Hack->m_BoostHack.m_wBoostHotkey))
			{
				g_pTM2Hack->m_BoostHack.Boost(true);
			}
		}
	}

	return CallNextHookEx (g_hLowLevelKeyHook, nCode, wParam, lParam);
}