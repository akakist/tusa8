#ifndef __NIGHT_H
#define __NIGHT_H

#include "command.h"
#include "game.h"

void do_night();
void process_night(unsigned int fromuser,unsigned int touser,bool dbl);
void add_phrase(const c_command &cmd);



#endif

