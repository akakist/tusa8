#ifndef MAFIA_HH
#define MAFIA_HH
#include <sys/types.h>
#include <sys/socket.h>
#include <map>
#include "c_conn.h"
#include "mysql_extens.h"
#include <string>

extern cMYSQL *dbh;

#define HINT_1_TIME	10
#define HINT_2_TIME	30
#define TIME_LIMIT	55
#define QUESTION_PERIOD	7

extern int questions_count;
extern bool game_started;
extern time_t next_question_time;
extern bool question_asked;
extern time_t question_asked_time;
extern int answers_count;
extern bool hint1;
extern bool hint2;

extern string question;
extern string answer;
extern int question_id;


class user
{
public:
	int gender;
	string login;
//	string nick;
	bool unreg;
	bool rules_sent;
#ifdef WITH_BANK	
	double credits;
#endif	
	user(){gender=0; unreg=1; rules_sent=0; 
#ifdef WITH_BANK	
	credits=0;
#endif
	};
};

extern map<string,user*> users;

#endif

