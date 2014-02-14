#include "Log.h"

#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <time.h>

void add_log(const char * fmt, ...)
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

	if ((fp = fopen("C:\\TMUF_TRAINER_LOG.txt", "a")) != NULL )
	{
		fprintf( fp, "%s\n", logbuf );
		fclose(fp);
	}
}