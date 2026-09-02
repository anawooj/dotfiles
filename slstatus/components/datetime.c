/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "../slstatus.h"
#include "../util.h"

typedef struct {
	int dayOfMonthNum;
	char dayChar[3];
	int dayNum;
	char suffix[3];
	char monthChar[3];
	int monthNum;
	int era;
	char year[3];
	char hour[3];
	char minute[3];

} date;

const char *
datetime(const char *fmt)
{
	time_t t;

	t = time(NULL);
	if (!strftime(buf, sizeof(buf), fmt, localtime(&t))) {
		warn("strftime: Result string exceeds buffer size");
		return NULL;
	}

	// optimize variables
	// like dayOfMonth -> dayOfWeek
	// is the amount of them necessary?

	date* date1 = (date*)malloc(sizeof(date*) + 5 * sizeof(int));

	// i should check if the memory was really allocated

	date1->dayOfMonthNum = buf[0] - '0';
						  
	date1->dayChar[0] = buf[2];
	date1->dayChar[1] = buf[3]; // there must be some better way of getting this
				    
	sscanf(date1->dayChar, "%d", &date1->dayNum);

	date1->monthChar[0] = buf[5];
	date1->monthChar[1] = buf[6];

	sscanf(date1->monthChar, "%d", &date1->monthNum);

	date1->era = 2;
	date1->year[0] = buf[8];
	date1->year[1] = buf[9];

	date1->hour[0] = buf[11];
	date1->hour[1] = buf[12];

	date1->minute[0] = buf[14];
	date1->minute[1] = buf[15];

	// i should dump this into a function that sets everything up
	
	const char *dayOfMonthNames[] = { "Sundas", "Mornidas", "Tirdas", "Middas", "Turdas", "Fredas", "Loredas" }; 
	
	const char *monthNames[] = { "Morning Star", "Sun\'s Dawn", "First Seed", "Rain\'s Hand", "Second Seed", "Midyear", "Sun\'s Height", "Last Seed", "Hearthfire", "Frostfall", "Sun\'s Dusk", "Evening Star" };

	// const char *dayOfMonthNames[] = { "Lunae", "Martis", "Mercurii", "Iovis", "Veneris", "Saturni", "Solis" };

	// const char *monthNames[] = { "Ianuarius", "Februarius", "Martius", "Aprilis", "Maius", "Iunius", "Iulius", "Augustus", "September", "October", "November", "December"};

	switch(date1->dayNum) {
		case 1: case 21: case 31:
			memcpy(date1->suffix, "st", 2);
			break;
		case 2: case 22:
			memcpy(date1->suffix, "nd", 2);
			break;
		case 3: case 23:
			memcpy(date1->suffix, "rd", 2);
			break;
		default:
			memcpy(date1->suffix, "th", 2);
	}

	char result[1024];

	snprintf(result, sizeof result, "%s, %d%s of %s %dE%s   %s:%s", dayOfMonthNames[date1->dayOfMonthNum], date1->dayNum, date1->suffix, monthNames[date1->monthNum - 1], date1->era, date1->year, date1->hour, date1->minute);

	memset(buf, 0, sizeof buf);
	
	memcpy(buf, result, sizeof result);

	free(date1);

	return buf;
}

const char * latinizeDayNames( char array ) {
		
}
