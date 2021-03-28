#ifndef HTTP_USER_H__
#define HTTP_USER_H__
#include "_mutex.h"
#include <time.h>
#include <list>
#include <deque>
#include "PTR.h"
#include "db_var.h"
class web_server;
class c_socket;
class web_user:public db_row
{
	 web_user(const  web_user&);             // Not defined
	 web_user& operator=(const  web_user&);  // Not defined

protected:
public:
	unsigned long map_key; /*map key which used in server->_users*/
	unsigned long get_map_key() const {return map_key;}
	
//DATA STUFF
public:	


	
	time_t last_access_time;	// last access time (last http_request)
	unsigned long database_id; /*Used to restore old session*/

//	string language; // default - en then ru,de,fr,...
	string sid_name;	
	
	

   
	web_user(/*const string &deflang*/);
	virtual ~web_user();
	virtual vector<string> description(bool is_caption);
	bool erased;

	string rvfiles(const string & s);
	string rvfiles(map < string, string > &m, const string & s);
	virtual void postinit_rvmap(map<string,string> &m);
};
#endif
