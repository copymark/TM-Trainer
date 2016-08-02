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

void CConsole::print(const char* fmt, ...) {
	char buffer[256];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	WriteConsole(m_OUT_HANDLE, buffer, strlen(buffer), NULL, NULL);
	va_end(args);
}

void CConsole::SetConsoleColor(WORD layout) {
	SetConsoleTextAttribute(m_OUT_HANDLE, layout);
}