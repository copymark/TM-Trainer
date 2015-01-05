#pragma once

/*
INCLUDES
*/
#include <Windows.h>
#include <time.h>
#include <Commctrl.h>
#include "resource.h"
#include "Error.h"
#include "Splash.h"

#pragma comment (lib, "Comctl32.lib")

/*
DEFINES
*/
#define TRAINERNAME "TM2 Ultra Trainer Copymark"
#define GAMENAME "ManiaPlanet"
#define VERSION "Version: 3.0.1.0"
#define GAMEVERSION "Date=2014-04-29_23_00 Svn=56290 GameVersion=3.3.0"

/*
Globals
*/
extern HINSTANCE g_hInstance;
extern HWND g_hWindow;

/*
Dialog Procs
*/
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK BoostOptionsDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

/*
LowLevelKeyHook
*/
extern HHOOK g_hLowLevelKeyHook;
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

/*
Various
*/
void InitConfig(void);


void add_log(const char * fmt, ...);