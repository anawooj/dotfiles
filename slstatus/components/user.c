/* See LICENSE file for copyright and license details. */
#include <pwd.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include "../slstatus.h"
#include "../util.h"

const char *
gid(const char *unused)
{
	return bprintf("%d", getgid());
}

const char *
username(const char *unused)
{
	time_t now = time(NULL);          
	struct tm *t = localtime(&now);

	const int curSec = ((t->tm_hour * 60) + t->tm_min ) * 60;
	const int dayPerc = (curSec * 100) / 86400;

	char panel[1024];
	char bar[] = "..........";
	int loaded = round(dayPerc/10);

	for ( int i = 0; i < loaded; i++ ) {
		bar[i] = ':';
	}

	sprintf(panel, "%s %d%%", bar, dayPerc);

	return bprintf("%s", panel);
}

const char *
uid(const char *unused)
{
	return bprintf("%d", geteuid());
}
