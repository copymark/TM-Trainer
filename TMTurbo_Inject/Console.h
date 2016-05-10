#pragma once

#include <Windows.h>

class  CConsole
{
public:
	CConsole(char* title);
	~CConsole();

	void print(char *text);

	void SetConsoleColor(WORD layout);
private:
	HANDLE m_OUT_HANDLE;
};

