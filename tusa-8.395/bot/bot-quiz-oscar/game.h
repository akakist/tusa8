#ifndef __GAME__H
#define __GAME__H

#include "misc.h"

void process_command(c_command &cmd);

void check_state();
void check_answer(const string &nick,const string &login,const string &answer);
void greeting(const string &nick,const string &login);


#endif

