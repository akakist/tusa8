#ifndef __DAY_H
#define __DAY_H

#include "command.h"
#include "game.h"
#include <string>
#include "user.h"

void do_day();

void process_day(unsigned int fromuser,unsigned int touser,bool dbl);

void invite_vote1();
void invite_vote2();

void end_vote1();
void end_vote2();

void do_vote1();
void do_vote2();

void do_vote_yes(const string &n);
void do_vote_no(const string &n);

bool check_for_vote1_end();
bool check_for_vote2_end(int yes, int no);

extern c_user* vote2_person;

#endif

