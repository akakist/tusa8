#ifndef MAFIA_HH
#define MAFIA_HH
#include <sys/types.h>
#include <sys/socket.h>
#include <map>
#include "c_conn.h"
#include "mysql_extens.h"
#include <string>

extern cMYSQL *dbh;


struct pay
{
    unsigned int id;
    string type;// num odd even
    set<unsigned int> nums;
    double summa;
};

class user
{
public:
	int gender;
	string login;
	string nick;
	bool unreg;
	bool rules_sent;
	user(){gender=0; unreg=1; rules_sent=0; 
	};
	map<unsigned int,pay> pays;
};


extern map<string,user*> users;
user* get_user(const string& login);

#endif

