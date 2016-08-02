#pragma once

#include <Windows.h>
#include <stdio.h>
#include <stdarg.h>

class  CConsole
{
public:
	CConsole(char* title);
	~CConsole();

	void print(const char* fmt, ...);

	void SetConsoleColor(WORD layout);
private:
	HANDLE m_OUT_HANDLE;
};

