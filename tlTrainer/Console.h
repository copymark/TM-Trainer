#pragma once

#include "DllGlobals.h"

class _declspec(dllexport) CConsole
{
public:
	CConsole(char* title);
	~CConsole();

	void print(char *text);

	void SetConsoleColor(WORD layout);
private:
	HANDLE m_OUT_HANDLE;
};

