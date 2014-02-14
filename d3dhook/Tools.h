#pragma once

#include <Windows.h>
#include <time.h>
#include <stdio.h>

namespace Tools
{
	void *DetourFunc(BYTE *src, const BYTE *dst, const int len);
	DWORD dwFindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask,char *szMask);
	void add_log(const char * fmt, ...);
}