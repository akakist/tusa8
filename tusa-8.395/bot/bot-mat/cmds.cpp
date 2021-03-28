#include <stdio.h>
#include "c_conn.h"
#include "errs.h"
#include "config.h"
#include "utils.h"
#include "misc.h"
#include "mysql_extens.h"
#include <vector>
#include "mat.h"
#include "antiflood.h"
#include "mutex_inspector.h"

void do_greeting(const string &nick,const string &login, int gender);

void on_greeting(const string &nick,const string &login)
{MUTEX_INSPECTOR;
	if(nick==params["NICKNAME"]) return;
	if(users.find(nick)==users.end()){
		sock.response("EUI "+nick);
	}else{
		printf("founded in exists\n");
		user *u=users.find(nick)->second;
//		do_greeting(nick,u->login,u->gender);
	}
}

void do_greeting(const string &nick,const string &login,int gender)
{MUTEX_INSPECTOR;
//	printf("gend=%d\n",gender);
	if(rand()%100>30) channelmsg(nick+", "+rndvec(greetingmsgs[gender]));
}

void process_command(c_command &cmd)
{MUTEX_INSPECTOR;
	string msg=cmd.msg;
	check_for_flood(cmd);
	if(cmd.cmd=="JOIN"){
		on_greeting(cmd.fromnick,cmd.fromnick);
		return;
	}
	if(cmd.cmd=="NICK"){
		user *u;
		if(users.find(cmd.fromnick)==users.end()) return;
		u=users[cmd.fromnick];
		users[cmd.cmd]=u;
		return;
	}
	if(cmd.cmd=="EUI_REPLY"){
		map<string,string> m=parse_strmap(cmd.msg);
//		vector<string> pr=splitTOK(":",cmd.msg);
//		if(pr.size()<4) return;
		string nick=m["NICK"];
//		int pz=nick.find
		string login=m["LOGIN"];
		int gender=atoi(m["GENDER"]);
//		bool unreg=atoi(pr[3]);
		bool ok=false;
		for(typeof(users.begin()) i=users.begin();i!=users.end();i++){
			if(i->second->login==login){
//				printf("found old\n");
				users[nick]=i->second;
				ok=true;
			}
		}
		if(!ok){
//			printf("greating new\n");
			user* u=new user;
			u->login=login;
			u->gender=gender;
			if(u->gender==0) u->gender=1;
			u->unreg=false;
			users[nick]=u;
		}
//		printf("n=%s l=%s\n",nick.c_str(),login.c_str());
//		do_greeting(nick,login,gender);
	}
	if(cmd.cmd=="PRIVMSG"){
		user *u;
		if(users.find(cmd.fromnick)==users.end()){
//			printf("user %s not found\n",cmd.fromnick.c_str());
			return;
		}
		u=users[cmd.fromnick];

		string msg=cmd.msg;
		if(u->login=="Arsenic"){
		if(strex(msg,"!ag")){
			int pz=msg.find(" ",0);
			if(pz!=-1){
				string ms=msg.substr(pz+1,msg.size()-pz-1);
				int g=atoi(msg.substr(3,1));
				privmsg(cmd.fromnick,"Added new greeting for gender "+itostring(g)+": "+ms);
				dbh->real_queryQ((QUERY)"insert into mat_greetingmsg (gender,answer) values (?,'?')"<<g<<ms);
				greetingmsgs[g].push_back(ms);
				return;
			}
		
		}
		if(strex(msg,"!am")){
			int pz=msg.find(" ",0);
			if(pz!=-1){
				string ms=msg.substr(pz+1,msg.size()-pz-1);
				int g=atoi(msg.substr(3,1));
				privmsg(cmd.fromnick,"Added new message for gender "+itostring(g)+": "+ms);
				dbh->real_queryQ((QUERY)"insert into mat_msg (gender,answer) values (?,'?')"<<g<<ms);
				msgs[g].push_back(ms);
				return;
			}
		
		}
		if(strex(msg,"!скажи")){
			int pz=msg.find(" ",0);
			if(pz!=-1){
				string ms=msg.substr(pz+1,msg.size()-pz-1);
				channelmsg(ms);
			}
		}
		}
//		printf("ppriv=%d\n",cmd.ppriv);
		
		if(cmd.ppriv){
			int slp=(rand()%5)+1;
			sleep(slp);
			
			string msg=cmd.msg;
			channelmsg(cmd.fromnick+", "+rndvec(msgs[u->gender]));
			if(msg.find("бот")!=-1 || msg.find("прог")!=-1) return;
			if(msg.find("казл")!=-1) return;
			if(
			 msg.find("бля")!=-1 ||
			 msg.find("сука")!=-1 ||
			 msg.find("пизд")!=-1 ||
			 msg.find("ху")!=-1 ||
			 msg.find("бля")!=-1 ||
			 msg.find("манд")!=-1 ||
			 msg.find("муд")!=-1 ||
			 msg.find("суч")!=-1
			) {
				int g=atoi(params["GENDER"]);
				if(g==1){
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
						dbh->real_queryQ((QUERY)"insert into mat_msg (gender,answer) values (?,'?')"<<g<<cmd.msg);
						msgs[g].push_back(cmd.msg);
					}
				}
			}
		}
		
//		printf("%d\n",cmd.ppriv);
//		channelmsg(cmd.fromnick+", "+msgs[u->gender]);
	}
}

