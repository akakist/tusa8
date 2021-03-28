#ifndef __GAME__H
#define __GAME__H

#include "misc.h"

void process_command(c_command &cmd);

void init();

class user
{
public:

	time_t last_time;
	time_t last_time_per5;
	int cnt;
	int cnt_per5;
	string s;
	bool ignored;
	int ig_cnt;
	int max_cnt;
	user();
};

#endif
