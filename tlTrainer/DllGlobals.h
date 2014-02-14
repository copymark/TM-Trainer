#pragma once

#pragma warning(disable: 4251)
/*
INCLUDES
*/
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <direct.h>
#include <Commctrl.h>

#pragma comment (lib, "Winmm.lib")


#include "resource.h"
#include "Error.h"

extern void add_log(const char * fmt, ...);

extern HMODULE g_hDllModule;

_declspec(dllexport) extern HMODULE GetDllModule(void);