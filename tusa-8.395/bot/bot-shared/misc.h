#ifndef MISC__H
#define MISC__H

#include <string>
#include "c_conn.h"

using namespace std;

void connect_to_server();

void privmsg(const string &nick,const string &msg);
void action(const string &msg);
void channelmsg(const string &msg);

class c_command{
public:
	string fromnick;
//	string user;
	string tonick;
	string host;
	string cmd;
	string msg;
	bool ppriv;
	c_command():ppriv(false) {};
};

void do_help(const c_command &cmd);

c_command parse_string(const string &);

extern c_socket sock;


#endif

