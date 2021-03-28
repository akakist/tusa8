#ifndef MAFIA_HH
#define MAFIA_HH
#include <sys/types.h>
#include <sys/socket.h>
#include <map>
#include "c_conn.h"
#include "mysql_extens.h"

extern cMYSQL *dbh;

void privmsg(const string &nick,const string &msg);

#endif

