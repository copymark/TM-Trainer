#include "DllGlobals.h"

HMODULE g_hDllModule = 0;

_declspec(dllexport) HMODULE GetDllModule(void)
{
	return g_hDllModule;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hDllModule = hModule;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

void add_log(const char * fmt, ...)
{
	try
	{
		struct tm current_tm;
		time_t current_time;

		time(&current_time);
		localtime_s(&current_tm, &current_time);

		char logbuf[256];
		sprintf_s(logbuf, sizeof(logbuf), "-> %02d:%02d:%02d  ", current_tm.tm_hour, current_tm.tm_min, current_tm.tm_sec);

		va_list va_alist;
		va_start(va_alist, fmt);
		_vsnprintf_s(logbuf+strlen(logbuf), sizeof(logbuf) - strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
		va_end(va_alist);

		FILE *fp;
		fopen_s(&fp, "C:\\TM2_TRAINER_LOG.txt", "a");
		if (fp)
		{
			fprintf_s(fp, "%s\n", logbuf);
			fclose(fp);
		}
	}
	catch (...)
	{
	}
}