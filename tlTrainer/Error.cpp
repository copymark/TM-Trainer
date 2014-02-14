#include "Error.h"


CError::CError(char *pszText, char *pszFile, int iLine)
{
	this->m_pszText = pszText;
	this->m_pszFile = pszFile;
	this->m_iLine = iLine;
}


CError::~CError(void)
{
}


// Gibt Messagebox mit Fehler aus
void CError::Show(void)
{
	DWORD dwErrorCode = GetLastError();

	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL );

	char buffer[256];
	sprintf_s(buffer, sizeof(buffer), "File: %s | Line: %d\n\n%s\nErrorCode: %d :: %s", this->m_pszFile, this->m_iLine, this->m_pszText, dwErrorCode, lpMsgBuf);
	
	MessageBox(0, buffer, "ERROR", MB_ICONERROR | MB_TOPMOST | MB_OK);

	LocalFree(lpMsgBuf);
}
