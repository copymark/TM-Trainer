#pragma once

#include <Windows.h>
#include <vector>
#include "resource.h"
#include "Log.h"

#pragma comment (lib, "msimg32.lib")
#pragma comment (lib, "gdiplus.lib")
#include "EZ_LCD.h"

extern HMODULE g_hModule;
extern CEzLcd *g_pLCD;

extern const int g_iLineHeight;
extern HICON g_hIcoRight;
extern HICON g_hIcoLeft;
extern HICON g_hIcoDown;
extern HICON g_hIcoCircle;