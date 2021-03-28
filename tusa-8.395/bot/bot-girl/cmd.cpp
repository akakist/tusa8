#include <stdio.h>
#include "c_conn.h"
#include "errs.h"
#include "config.h"
#include "utils.h"
#include "misc.h"
#include "mysql_extens.h"
#include <vector>
#include "girl.h"
#include "antiflood.h"
#include "cmd.h"
#include "mutex_inspector.h"

user::user():last_time(0),cnt(0),ignored(false),ig_cnt(0),cnt_per5(0),last_time_per5(0){MUTEX_INSPECTOR;
	max_cnt=30+(rand()%100);
};


void do_help(const string &nick)
{
MUTEX_INSPECTOR;
	privmsg(nick,"Помощь:<br>"
		"Для начала можете просто со мной поговорить, или:<br>"
		"<U>!темп</U> - Температура за окном");
}

time_t last_weather_time;
bool worked;

void do_weather()
{
	if(last_weather_time+300>time(NULL)) return;
	action("А хрен бы знал что на улице.. Давно не была там..");
	last_weather_time=time(NULL);
}

map<string,user*> nicks;

static void remove_old_nicks()
{MUTEX_INSPECTOR;
	time_t t=time(NULL);
	for(typeof(nicks.begin()) i=nicks.begin();i!=nicks.end();i++){
		if(t>i->second->last_time+900){
			delete i->second;
			nicks.erase(i->first);
		}
	}
}

void do_answer(c_command &cmd)
{MUTEX_INSPECTOR;
	user *u=nicks[cmd.fromnick];
	if(!u){
		u=new user();
		nicks[cmd.fromnick]=u;
	}
	if(u->ignored){
		u->ig_cnt++;
		if(u->ig_cnt==5){
			if(rand()%50>25) channelmsg(cmd.fromnick+", Да ты уже достал! еще раз и пну нахер!");
		}
		if(u->ig_cnt==10){
			sock.response("KICK "+params["CHANNEL"]+" "+cmd.fromnick+" :парил");
		}
		if(u->ig_cnt==13){
			sock.response("MODE "+params["CHANNEL"]+" +bi "+cmd.fromnick);
			sock.response("KICK "+params["CHANNEL"]+" "+cmd.fromnick+" :banned");
		}
		return;
	}
	u->cnt++;
	u->last_time=time(NULL);
	if(u->cnt_per5>=4 && time(NULL)-u->last_time_per5<=5){
//		printf("ignored\n");
		u->ignored=true;
		u->ig_cnt=6;
		u->cnt_per5=0;
		return;
	}
	if(time(NULL)-u->last_time_per5>=5){
//		printf("rt=%d ltp=%d div=%d\n",time(NULL),u->last_time_per5,time(NULL)-u->last_time_per5);
		u->last_time_per5=time(NULL);
		u->cnt_per5=0;
	}
	u->cnt_per5++;
	remove_old_nicks();
	if(u->cnt>u->max_cnt){
		u->ignored=true;
		channelmsg(cmd.fromnick+", Знаешь, чё? ты меня уже достал до немогу! Ты в игноре...");
		return;
	}
	
	vector<string> w=splitTOK(",. \t!)(",cmd.msg);
	string msg=cmd.msg;
	if(w.size()>5){
		if(
			msg.find("ась")!=-1 ||
			msg.find("шла")!=-1 ||
			msg.find("хая")!=-1 ||
			msg.find("ная")!=-1 ||
			msg.find("пая")!=-1 ||
			msg.find("лая")!=-1 ||
			msg.find("кая")!=-1 ||
			msg.find("бая")!=-1 ||
			msg.find("Аня")!=-1 ||
			msg.find("аня")!=-1 ||
			msg.find("бот")!=-1 ||
			msg.find("прив")!=-1 ||
			msg.find("cама")!=-1 ||
			msg.find("ила")!=-1
		){
		}else{
			if(msg.find("!!!")==-1 && msg.find("???")==-1 && msg.find(")))))")==-1 && msg.find("(((((")==-1 && msg.find("   ")==-1)
			dbh->real_queryQ((QUERY)"insert into girl_unknow (msg) values ('?')"<<cmd.msg);
			user_msg.push_back(cmd.msg);
		}
	}
	if(rand()%1000<50) return;
	int slp=(rand()%7)+3;
	sleep(slp);
	u->last_time_per5+=slp;
//	printf("tm=%d c=%d\n",time(NULL)-u->last_time_per5,u->cnt_per5);
	vector<string> v=dbh->select_1_columnQ((QUERY)"select answer from girl_msg where '?' REGEXP pattern"<<cmd.msg);
	if(v.size()){
		int r=rand()%v.size();
		if(r<v.size()){
			channelmsg(cmd.fromnick+", "+v[r]);
			return;
		}
	}
	int r=rand()%1000;
	if(r<300 && user_msg.size()){
		int z=rand()%user_msg.size();
		if(z<user_msg.size()){
			channelmsg(cmd.fromnick+", "+user_msg[z]);
			return;			
		}
	}
	int s=rand()%simple_msg.size();
	if(s<simple_msg.size()){
		channelmsg(cmd.fromnick+", "+simple_msg[s]);
		return;
	}
}

time_t next_greeting_time;
string next_greeting_nick;
map<string,bool> greeted_nicks;

void do_greeting(c_command &cmd)
{MUTEX_INSPECTOR;
	if(cmd.fromnick==params["NICKNAME"]) return;
	if(next_greeting_nick.size()) return;
	if(greeted_nicks.find(cmd.fromnick)!=greeted_nicks.end()) return;
	int z=rand()%1000;
	if(z<10){
		next_greeting_time=time(NULL)+(rand()%60)+5;
		next_greeting_nick=cmd.fromnick;
	}
}

void add_greeting(c_command &cmd)
{MUTEX_INSPECTOR;
	string msg=cmd.msg;
	int pz=msg.find(" ",0);
	if(pz!=-1){
		msg=msg.substr(pz+1,msg.size()-pz-1);
		privmsg(cmd.fromnick,"Added new greeting: "+msg);
		dbh->real_queryQ((QUERY)"insert into girl_greetingmsg (answer) values ('?')"<<msg);
		greeting_msg.push_back(msg);
	}
}

void process_command(c_command &cmd)
{MUTEX_INSPECTOR;
	string msg=cmd.msg;
	check_for_flood(cmd);
//	if(!worked) return;
	if(cmd.cmd=="JOIN"){
		if(worked) do_greeting(cmd);
	}
	if(cmd.cmd=="QUIT" || cmd.cmd=="PART"){
		if(next_greeting_nick==cmd.fromnick) next_greeting_nick="";
	}
	if(cmd.cmd=="PRIVMSG"){
		if(strex(msg,"!help") || strex(msg,"!хелп") || strex(msg,"!помощь")){
			do_help(cmd.fromnick);
			return;
		}
		if(strex(msg,"!темп") || strex(msg,"!temp") || strex(msg,"!погода") || strex(msg,"!weather")){
			do_weather();
			return;
		}
		
		if(strex(msg,"!ag")){
			add_greeting(cmd);
			return;
		}
		if(cmd.fromnick=="Arsenic"||cmd.fromnick=="Mazar"||cmd.fromnick=="KPOXA" ||cmd.fromnick=="VIN_$"||cmd.fromnick=="Lena"||cmd.fromnick=="БОГИНЯ_секса"){
			if(strex(msg,"!start")){
				worked=true;
				privmsg(cmd.fromnick,"Started.");
			}
			if(strex(msg,"!stop")){
				worked=false;
				privmsg(cmd.fromnick,"Stopped.");
			}
			if(strex(msg,"скажи") || strex(msg,"сделай")){
				bool act=msg.substr(0,3)=="сде";
				int pz=msg.find(" ");
				if(pz!=-1){
					msg=msg.substr(pz+1,msg.size()-pz-1);
					if(act) action(msg);
					else channelmsg(msg);
				}
			}
		}
		if(cmd.ppriv){
			if(worked) do_answer(cmd);
		}
	}
}


void check_state()
{MUTEX_INSPECTOR;
	time_t ct=time(NULL);
	if(next_greeting_nick.size() && ct>=next_greeting_time){
		int r=rand()%greeting_msg.size();
		if(r<greeting_msg.size()){
			greeted_nicks[next_greeting_nick]=true;
			channelmsg(next_greeting_nick+", "+greeting_msg[r]);
			next_greeting_nick="";
		}
	}
}

void init()
{MUTEX_INSPECTOR;
	last_weather_time=0;
	worked=true;
}
