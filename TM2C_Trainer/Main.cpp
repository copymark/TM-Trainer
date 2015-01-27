#include "Globals.h"
#include "TM2Hack.h"
#include "Registration.h"

HINSTANCE g_hInstance = 0;
HWND g_hWindow = 0;

CConsole *g_pConsole = nullptr;

void InitConsole()
{
	g_pConsole = new CConsole("Trackmania 2 Ultra Trainer");

	g_pConsole->SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	g_pConsole->print("##############################################################################\n");
	g_pConsole->print("##  ______      ___     __  ______            ______         _              ##\n");
	g_pConsole->print("## /_  __/_ _  |_  |___/ / / / / /________ _ /_  __/______ _(_)__  ___ ____ ##\n");
	g_pConsole->print("##  / / /  ' \\/ __/___/ /_/ / / __/ __/ _ `/  / / / __/ _ `/ / _ \\/ -_) __/ ##\n");
	g_pConsole->print("## /_/ /_/_/_/____/   \\____/_/\\__/_/  \\_,_/  /_/ /_/  \\_,_/_/_//_/\\__/_/    ##\n");
	g_pConsole->print("##                                                                          ##\n");
	g_pConsole->print("##############################################################################\n");
}

/*
MAIN - EINSTIEGSPUNKT
*/
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HANDLE hMutex = CreateMutex(NULL, true, TRAINERNAME);

	if (GetLastError() == ERROR_ALREADY_EXISTS || hMutex == INVALID_HANDLE_VALUE)
		return FALSE;

	g_hInstance = hInstance;

	// "Software\\Nadeo\\TMUltraTrainer"
	Registration::SetSavePath("Software\\Nadeo\\TMUltraTrainer");

	if (!Registration::CheckRegistration())
	{
		DialogBox(GetDllModule(), MAKEINTRESOURCE(IDD_DIALOG3), NULL, Registration::RegistrationProc);
	}

	if (!Registration::IsRegistered())
		return FALSE;

	InitConsole();

	CSplash splash;
	splash.SetBitmap((HBITMAP)LoadImage(g_hInstance, MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, NULL, NULL, NULL));
	splash.SetTransparentColor(RGB(255,255,255));
	splash.ShowSplash();

	g_hLowLevelKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), NULL);
	
	Sleep(2000);
	splash.CloseSplash();
	
	CreateDialog(hInstance,MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);
	ShowWindow(g_hWindow, SW_SHOW);

	// Hauptnachrichtenschleife:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
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
		fopen_s(&fp, "C:\\Tm-Trainer Logs\\TM2_TRAINER_LOG.txt", "a");
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