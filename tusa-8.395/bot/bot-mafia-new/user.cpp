#include "user.h"
#include <map>
#include <string>
#include "utils.h"
#include "mafia.h"
#include "game.h"
#include "config.h"
#include <stdio.h>
#include "errs.h"
#include "mutex_inspector.h"

map<unsigned int,c_user*> gamers_by_id;
map<string,c_user*> gamers_by_nick;

unsigned int register_counter;

c_user::c_user(){
	win=false;
	gender=0;
	role=ROLE_MIR;
	score=0;
	last_score=0;
	id=0;
	accepted=false;
	main_maf=false;
	order=0;
	valed=false;
	marked_for_delete=false;
	killed=false;
	devoiced=false;
	left=false;
	chnick=false;
	need_update=false;
	doc_self=0;
}


string get_role_name(int role)
{MUTEX_INSPECTOR;
	if(role==ROLE_MIR) return "Мирных";
	if(role==ROLE_MAF) return "Мафиози";
	if(role==ROLE_MAN) return "Маньяков";
	if(role==ROLE_DOC) return "Докторов";
	if(role==ROLE_KAT) return "Катани";
	if(role==ROLE_BOM) return "Бомжей";
	if(role==ROLE_VAL) return "Валюток";
	return "ХЗ";
}

string get_role_name2(int role)
{
	if(role==ROLE_MIR) return "Мирный житель";
	if(role==ROLE_MAF) return "Мафиози";
	if(role==ROLE_MAN) return "Маньяк";
	if(role==ROLE_DOC) return "Доктор";
	if(role==ROLE_KAT) return "Инспектор Катани";
	if(role==ROLE_BOM) return "Бомж";
	if(role==ROLE_VAL) return "Валютная девка";
	return "ХЗ";
}

string get_role_name_cnt(int role,int cnt)
{MUTEX_INSPECTOR;
//	printf("role=%d cnt=%d\n",role,cnt);
	if(role==ROLE_MIR && cnt<=1) return "Мирный";
	if(role==ROLE_MAF) return "Мафиози";
	if(role==ROLE_MAN && cnt<=1) return "Маньяк";
	if(role==ROLE_DOC && cnt<=1) return "Доктор";
	if(role==ROLE_KAT) return "Катани";
	if(role==ROLE_BOM && cnt<=1) return "Бомж";
	if(role==ROLE_VAL && cnt<=1) return "Валютка";

	if(role==ROLE_MIR && cnt>1) return "Мирные";
	if(role==ROLE_MAN && cnt>1) return "Маньяки";
	if(role==ROLE_DOC && cnt>1) return "Доктора";
	if(role==ROLE_BOM && cnt>1) return "Бомжи";
	if(role==ROLE_VAL && cnt>1) return "Валютки";

	return "ХЗ";
}

string get_role_shortname(int role)
{MUTEX_INSPECTOR;
	if(role==ROLE_MIR) return "mir";
	if(role==ROLE_MAF) return "maf";
	if(role==ROLE_MAN) return "man";
	if(role==ROLE_DOC) return "doc";
	if(role==ROLE_KAT) return "kat";
	if(role==ROLE_BOM) return "bom";
	if(role==ROLE_VAL) return "val";
	return "hz";
}

void register_user(const string &nick,const string &login,unsigned int gender, const string& ip)
{MUTEX_INSPECTOR;

	if(!registration) return;
	logErr("---register user %s %s %s",login.c_str(), nick.c_str(),_DMI().c_str());
	for(typeof(gamers_by_id.begin()) i=gamers_by_id.begin();i!=gamers_by_id.end();i++){
		if(i->second->marked_for_delete) continue;
		if(i->second->login==login){
//			i->second->nick=str_nick(nick);
			return;
		}
		if(i->second->ip==ip)
		{
		    privmsg(nick,"Пользователь с таким IP адресом уже зареген.");
		    return;
		}
	}
	register_counter++;
	c_user *u=new c_user;
	u->nick=nick;
	u->login=login;
	u->id=register_counter;
	u->gender=gender;
	u->ip=ip;
	gamers_by_id[u->id]=u;
	gamers_by_nick[str_nick(nick)]=u;
	sock.response("MODE "+params["CHANNEL"]+" +v "+nick);
}

void unregister_user(const string &nick)
{MUTEX_INSPECTOR;
	logErr("---register user  %s %s", nick.c_str(),_DMI().c_str());
	typeof(gamers_by_nick.begin()) i=gamers_by_nick.find(str_nick(nick));
	if(i!=gamers_by_nick.end()){
		if(!i->second->devoiced) sock.response("MODE "+params["CHANNEL"]+" -v "+i->second->nick);
		unsigned int id=i->second->id;
		c_user *u=i->second;
		if(u->need_update){
			string sc=dbh->select_1Q((QUERY)"select score from mafia_score where login='?'"<<u->login);
			if(!sc.size()){
				int win=0;
				int loose=0;
				if(u->win) win=1;
				else loose=1;
				dbh->real_queryQ((QUERY)"insert into  mafia_score (win,loose,score,login) values (?,?,?,'?')"<<win<<loose<<u->score<<u->login);
			}else{
				int win=0;
				int loose=0;
				if(u->win) win=1;
				else loose=1;
				dbh->real_queryQ((QUERY)"update  mafia_score set win=win+?,loose=loose+?,score=score+? where login='?'"<<win<<loose<<u->score<<u->login);
			}
		}
		
		delete i->second;
		gamers_by_id.erase(id);
		gamers_by_nick.erase(i);
	}
}


void clear_users(){MUTEX_INSPECTOR;
	for(typeof(gamers_by_nick.begin()) i=gamers_by_nick.begin();i!=gamers_by_nick.end();i++){
		unregister_user(i->second->nick);
	}
	gamers_by_nick.clear();
	gamers_by_id.clear();
}

int get_active_users_count()
{MUTEX_INSPECTOR;
	int cnt=0;
	for(typeof(gamers_by_id.begin()) i=gamers_by_id.begin();i!=gamers_by_id.end();i++){
		if(!i->second->marked_for_delete) cnt++;
	}
	return cnt;
}

