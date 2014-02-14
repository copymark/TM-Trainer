#include "Tools.h"

void *Tools::DetourFunc(BYTE *src, const BYTE *dst, const int len) // credits to gamedeception
{
	BYTE *jmp = (BYTE*)malloc(len+5);
	DWORD dwback;
	VirtualProtect(src, len, PAGE_READWRITE, &dwback);
	memcpy(jmp, src, len); jmp += len;
	jmp[0] = 0xE9;
	*(DWORD*)(jmp+1) = (DWORD)(src+len - jmp) - 5;
	src[0] = 0xE9;
	*(DWORD*)(src+1) = (DWORD)(dst - src) - 5;
	VirtualProtect(src, len, dwback, &dwback);
	return (jmp-len);
}

bool bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for(;*szMask;++szMask,++pData,++bMask)
		if(*szMask=='x' && *pData!=*bMask )
			return false;

	return (*szMask) == NULL;
}

DWORD Tools::dwFindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask)
{
	for(DWORD i=0; i < dwLen; i++)
		if( bDataCompare( (BYTE*)( dwAddress+i ),bMask,szMask) )
			return (DWORD)(dwAddress+i);

	return 0;
}

#pragma warning(disable : 4996)

void Tools::add_log(const char * fmt, ...)
{
	va_list va_alist;
	char logbuf[256];
	FILE * fp;
	struct tm * current_tm;
	time_t current_time;

	time(&current_time);
	current_tm = localtime(&current_time);

	sprintf(logbuf, "-> %02d:%02d:%02d  ", current_tm->tm_hour, current_tm->tm_min, current_tm->tm_sec);

	va_start (va_alist, fmt);
	_vsnprintf (logbuf+strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
	va_end (va_alist);

	if ((fp = fopen("C:\\d3dhooklog.txt", "a")) != NULL )
	{
		fprintf( fp, "%s\n", logbuf );
		fclose(fp);
	}
}