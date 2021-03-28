#ifndef ANTI_FLOOD__H
#define ANTI_FLOOD__H

#include "misc.h"
#include <time.h>

class check_entry
{
public:

	time_t last_time;
	int cnt;
	string s;
	check_entry():last_time(0),cnt(0){};

};

void check_for_flood(c_command &cmd);


#endif

