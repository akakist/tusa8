#ifndef MAFIA_HH
#define MAFIA_HH
#include <sys/types.h>
#include <sys/socket.h>
#include <map>
#include "c_conn.h"
#include "mysql_extens.h"
#include <string>
#include <vector>

extern cMYSQL *dbh;

extern vector<string> user_msg;
extern vector<string> simple_msg;
extern vector<string> greeting_msg;

void check_state();

#endif
