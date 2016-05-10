#pragma once

#include <Windows.h>
#include <stdio.h>
#include "Detours/detours.h"
#include "MemTools.h"
#include "Console.h"
#include "PreciseTime.h"

CConsole* g_pConsole;

void InitConsole()
{
	g_pConsole = new CConsole("Trackmania Turbo Ultra Trainer");

	g_pConsole->SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	g_pConsole->print("##############################################################################\n");
	g_pConsole->print("##    ______        ______  ______           _##\n");
	g_pConsole->print("##  / _  __ / ___ ___ / _  __ / / _  __ / ________ _(_)___  ___  _____##\n");
	g_pConsole->print("##    / / / __ `__ \\ / / _____ / / / ___ / __ `/ / __ \\ / _ \\ / ___ /##\n");
	g_pConsole->print("##   / / / / / / / / / _____ / / / / / / _ / / / / / / __ / /##\n");
	g_pConsole->print("##  / _ / / _ / / _ / / _ / _ / / _ / / _ / \\__, _ / _ / _ / / _ / \\___ / _ /##\n");
	g_pConsole->print("##############################################################################\n");
}

LPTHREAD_START_ROUTINE HookThread(LPVOID lpParam)
{
	//InitConsole();
	
	CPreciseTime *pFeaturePreciseTime = new CPreciseTime();

	while (true)
	{
		pFeaturePreciseTime->checkButtonState();
		Sleep(200);
	}

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HookThread, NULL, NULL, NULL);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}