#ifdef __GNUC__
#pragma implementation				// gcc: implement sql_udf.h
#endif

#include "stdafx.h"

#include "web_server.h"
#include "mutex_inspector.h"
#include "c_conn.h"
#include "file_set.h"
#include "web_server_data.h"
#include "str_lib.h"
#include "tcp_sender.h"
#include "server_config.h"
#include "web_server_config.h"
#include "web_tools.h"

/*void update_adminfo_net_conntime(const string &ip,time_t conntime);
*/


web_user::~web_user()
{
}


web_user::web_user(/*const string& deflang*/):map_key(0),erased(false)
{
	

//	logined= false;
//	language=deflang;
}
vector<string> web_user::description(bool is_caption)
{
	vector<string> r;
	return r;
}
string web_user::rvfiles(map < string, string > &m, const string & s)
{
	postinit_rvmap(m);
	string q = s;
/*		if (language.size()) {
			if (global_config.files_exists(language + ":" + q)) {
				q = language + ":" + q;
			}
		}*/
	return RVfiles(m, q.c_str());
}

string web_user::rvfiles(const string & s)
{	
	map < string, string > m;
	return rvfiles(m, s);
}
void web_user::postinit_rvmap(map<string,string> &m)
{
   
   string s = std::to_string(rand());
   m["~random~"] = s;
   map<string,string> mm=web_server_config.get_replacements();
   for(typeof(mm.begin()) i=mm.begin();i!=mm.end();i++)
   {
   	m[i->first]=global_config.files_get(i->second);
   }

}
