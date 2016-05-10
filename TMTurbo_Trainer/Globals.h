#pragma once

#include <Windows.h>
#include "Console.h"
#include "resource.h"
//#include <Commctrl.h>

//#pragma comment (lib, "Comctl32.lib")

#define TRAINERNAME "TM-Turbo Ultra Trainer Copymark"
#define GAMENAME "TrackmaniaTurbo"
#define VERSION "Version: 0.0.0.1"
//#define GAMEVERSION "Date=2014-12-12_12_00 Svn=59700 GameVersion=3.3.0"

/*
Globals
*/
extern HINSTANCE g_hInstance;
extern HWND g_hWindow;

/*
Dialog Procs
*/
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);


void add_log(const char * fmt, ...);

extern CConsole *g_pConsole;
