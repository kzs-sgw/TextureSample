#include "Date.h"


Date::Date(void)
{
}


Date::~Date(void)
{
}

char* Date::getYMD_Time()
{
	time_t now = time(NULL);
	struct tm *pnow = localtime(&now);
	
	char time_char[100];
	sprintf( time_char, "%2d_%02d_%02d-%02d_%02d_%02d",
		pnow->tm_year+1900,
		pnow->tm_mon+1,
		pnow->tm_mday,
		pnow->tm_hour,
		pnow->tm_min,
		pnow->tm_sec );
	
	return time_char;
}