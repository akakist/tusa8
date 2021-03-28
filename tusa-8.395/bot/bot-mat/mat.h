#ifndef MAFIA_HH
#define MAFIA_HH
#include <sys/types.h>
#include <sys/socket.h>
#include <map>
#include "c_conn.h"
#include "mysql_extens.h"
#include <string>
//#include "_mutex.h"

string rndvec(const vector<string> &v);

extern map<int,vector<string> > greetingmsgs;
extern map<int,vector<string> > privmsgs;
extern map<int,vector<string> > msgs;

//extern _mutex sock_MX;

extern cMYSQL *dbh;

class user
{
public:
	int gender;
	string login;
//	string nick;
	bool unreg;
	bool rules_sent;
	user(){gender=0; unreg=1; rules_sent=0; };
};

extern map<string,user*> users;

#endif

