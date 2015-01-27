#include "Console.h"

CConsole::CConsole(char *title)
{
	AllocConsole();
	m_OUT_HANDLE = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitle(title);
}


CConsole::~CConsole()
{
	FreeConsole();
}

void CConsole::print(char *text) {
	WriteConsole(m_OUT_HANDLE, text, strlen(text), NULL, NULL);
}

void CConsole::SetConsoleColor(WORD layout) {
	SetConsoleTextAttribute(m_OUT_HANDLE, layout);
}