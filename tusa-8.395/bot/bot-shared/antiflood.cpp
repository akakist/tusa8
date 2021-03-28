#include <stdio.h>
#include "c_conn.h"
#include "errs.h"
#include "config.h"
#include "utils.h"
#include "misc.h"
#include <vector>
//#include "quiz.h"
#include "antiflood.h"
#include <time.h>
#include "mutex_inspector.h"

map<string,check_entry> kicks;
map<string,check_entry> last_words;

void check_for_ban(const string &nick,const string &login)
{MUTEX_INSPECTOR;
	kicks[login].cnt++;
	kicks[login].last_time=time(NULL);
	if(kicks[login].cnt>2){
		sock.response("MODE "+params["CHANNEL"]+" +bi "+nick);
		kicks.erase(login);
	}
}

void clean_lists()
{MUTEX_INSPECTOR;
	time_t ct=time(NULL);
	for(typeof(kicks.begin()) i=kicks.begin();i!=kicks.end();i++){
		if(ct>i->second.last_time+600) kicks.erase(i->first);
	}
	for(typeof(last_words.begin()) i=last_words.begin();i!=last_words.end();i++){
		if(ct>i->second.last_time+5) last_words.erase(i->first);
	}
}

void check_for_flood(c_command &cmd)
{MUTEX_INSPECTOR;
	clean_lists();
	string msg=cmd.msg;
	
	if(last_words.find(cmd.fromnick)!=last_words.end()){
		string lw=last_words[cmd.fromnick].s;
		int len=lw.size()-2;
		if(len<=lw.size()) len=lw.size()-1;
		if(msg.size()<2) len=msg.size();
		if(len>0){
			if(msg.substr(0,len)==lw.substr(0,len)){
				if(last_words[cmd.fromnick].cnt>4){
					last_words.erase(cmd.fromnick);
					check_for_ban(cmd.fromnick,cmd.fromnick);
					sock.response("KICK "+params["CHANNEL"]+" "+cmd.fromnick+" :флуд");
					return;
				}else{
					last_words[cmd.fromnick].cnt++;
				}
			}
//			else{
//				last_words[cmd.user].cnt=0;
//			}
		}
	}
	last_words[cmd.fromnick].last_time=time(NULL);
	last_words[cmd.fromnick].s=msg;
	
	{vector<string> v=splitTOK(" ,.\t",msg);
	for(unsigned int i=0;i<v.size();i++){
		string s=v[i];
		if(s.size()>=50){
			if(s.find("http://")==-1 && s.find("ftp://")==-1){
				check_for_ban(cmd.fromnick,cmd.fromnick);
				sock.response("KICK "+params["CHANNEL"]+" "+cmd.fromnick+" :флуд");
				return;
			}
		}
	}}
	int rep=0;
	vector<string> v=splitTOK(" ,.\t!?_",msg);
	for(unsigned int i=0;i<v.size();i++){
		if(i!=0){
			if(v[i]==v[i-1]) rep++;
			else rep=0;
			if(rep>4){
				check_for_ban(cmd.fromnick,cmd.fromnick);
				sock.response("KICK "+params["CHANNEL"]+" "+cmd.fromnick+" :флуд");
				return;
			}
		}
	}
}
