#ifndef GAME__H
#define GAME__H

#include "command.h"

#ifdef DEBUG111

#define REG1_TIMEOUT		10
#define REG2_TIMEOUT		20
#define ROLE_ACCEPT_TIME	90
#define NIGHT_TIME		60
#define MORNING_TIME		15
#define DAY_TIME		60
#define VOTE1_TIMEOUT		30
#define VOTE2_TIMEOUT		30
#define VOTE_SAY_TIMEOUT	30

#else

#define REG1_TIMEOUT		60
#define REG2_TIMEOUT		120
#define ROLE_ACCEPT_TIME	90
#define NIGHT_TIME		180
#define MORNING_TIME		15
#define DAY_TIME		180
#define VOTE1_TIMEOUT		120
#define VOTE2_TIMEOUT		120
#define VOTE_SAY_TIMEOUT	30

#endif

void start_game(const c_command &);

void process_game();

void remove_left_users();

void accept_role(const string &n);
void decline_role(const string &,bool sam);
void make_main_maf(bool init);

void check_roles();
void show_roles();

void check_for_main_maf();

bool check_all_orders();

string get_gamers_list(bool);

bool process_for_end_game();

string get_mafia();

extern bool game_started;
extern bool registration;
extern bool roles_assign;
extern bool night;
extern bool day;
extern bool morning;

extern bool voiced;

extern time_t night_time;
extern time_t morning_time;
extern time_t day_time;

extern bool vote1;
extern bool vote1_half;
extern time_t vote1_time;

extern bool vote_say;
extern time_t vote_say_time;

extern bool vote2;
extern bool vote2_half;
extern time_t vote2_time;

#endif

