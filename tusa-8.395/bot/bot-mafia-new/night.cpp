#include "mafia.h"
#include <string>
#include "command.h"
#include "misc.h"
#include "game.h"
#include "messages.h"
#include "user.h"
#include <stdio.h>
#include "utils.h"
#include "config.h"
#include "night.h"
#include "errs.h"
#include "mutex_inspector.h"

void do_night()
{MUTEX_INSPECTOR;
	remove_left_users();
	action(_MSG("night"));
	night=true;
	night_time=time(NULL);
	string l=get_gamers_list(false);
	string ll=get_gamers_list(true);
	for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){MUTEX_INSPECTOR;
		c_user *u=i->second;
		if(u->marked_for_delete) continue;
		u->order=0;
		u->order_done=false;
		u->doced=false;
		u->valed=false;
		u->phrase="";
		if(u->role==ROLE_MAN) privmsg(u->nick,_MSG("manorder",u->gender)+"<br>"+l);
		if(u->role==ROLE_MAF && u->main_maf) privmsg(u->nick,_MSG("maforder",u->gender)+"<br>"+ll);
		if(u->role==ROLE_DOC) privmsg(u->nick,_MSG("docorder",u->gender)+"<br>"+l);
		if(u->role==ROLE_BOM) privmsg(u->nick,_MSG("bomorder",u->gender)+"<br>"+l);
		if(u->role==ROLE_KAT) privmsg(u->nick,_MSG("katorder",u->gender)+"<br>"+l);
		if(u->role==ROLE_VAL) privmsg(u->nick,_MSG("valorder",u->gender)+"<br>"+l);
		if(u->role==ROLE_MAF && !u->main_maf) continue;
//		if(u->role!=ROLE_MIR) privmsg(u->nick,l);
	}
}


void add_phrase(const c_command &cmd)
{MUTEX_INSPECTOR;
	int pz=cmd.msg.find(" ",0);
	if(pz!=-1){
		string fr=cmd.msg.substr(pz+1,cmd.msg.size()-pz-1);
		if(gamers_by_nick.find(str_nick(cmd.fromnick))!=gamers_by_nick.end()){MUTEX_INSPECTOR;
			c_user *u=gamers_by_nick.find(str_nick(cmd.fromnick))->second;
			if(u->marked_for_delete) return;
			if(fr.find("<")!=-1 || fr.find("&")!=-1){
				privmsg(u->nick,_MSG("phrasenotaccepted",u->gender));
				return;
			}
			if(u->role!=ROLE_MIR){MUTEX_INSPECTOR;
				if(u->role==ROLE_MAF && !u->main_maf) return;
				u->phrase=fr;
				privmsg(u->nick,_MSG("phraseaccepted",u->gender,fr.c_str()));
			}
		}
	}
}

void process_night(unsigned int fromuser,unsigned int touser,bool dbl)
{MUTEX_INSPECTOR;
	if(!gamers_by_id.count(fromuser))return;
	if(!gamers_by_id.count(touser))return;
	c_user *u=gamers_by_id.find(fromuser)->second;
	c_user *z=gamers_by_id.find(touser)->second;
	if(!u || !z) return;
	if(u->marked_for_delete || z->marked_for_delete) return;
	if(u->role!=ROLE_DOC){MUTEX_INSPECTOR;
		if(fromuser==touser){
			privmsg(u->nick,_MSG("killyourself",u->gender));
			return;
		}
	}
	if(u->role==ROLE_MIR) return;
	if(u->role==ROLE_DOC){MUTEX_INSPECTOR;
		if(touser==u->id && u->doc_self>2){
			privmsg(u->nick,"doc-cure-self");
			return;
		}
		if(u->order_done){MUTEX_INSPECTOR;
			if(gamers_by_id.count(u->order))
			gamers_by_id.find(u->order)->second->doced=false;
		}
		u->order=touser;
		privmsg(u->nick,_MSG("docdone",u->gender,z->nick.c_str()));
		if(!u->order_done) action(_MSG("docready"));
		u->order_done=true;
		z->doced=true;
		z->parent=fromuser;
	}
	if(u->role==ROLE_BOM || (u->role==ROLE_KAT && !dbl)){MUTEX_INSPECTOR;
		u->order=touser;
		privmsg(u->nick,_MSG("checkdone",u->gender,z->nick.c_str()));
		u->order_type=false;
		if(u->role==ROLE_BOM && !u->order_done) action(_MSG("bomready"));
		if(u->role==ROLE_KAT && !u->order_done) action(_MSG("katready"));
		u->order_done=true;
//		return;	
	}
	if(u->role==ROLE_MAF || u->role==ROLE_MAN || (u->role==ROLE_KAT && dbl)){MUTEX_INSPECTOR;
		if(u->role==ROLE_MAF && !u->main_maf) return;
		u->order=touser;
		u->order_type=true;
		privmsg(u->nick,_MSG("killdone",u->gender,z->nick.c_str()));
		if(u->role==ROLE_MAF && !u->order_done) action(_MSG("mafready"));
		if(u->role==ROLE_MAN && !u->order_done) action(_MSG("manready"));
		if(u->role==ROLE_KAT && !u->order_done) action(_MSG("katready"));
		u->order_done=true;
//		return;	
	}
	if(u->role==ROLE_VAL){MUTEX_INSPECTOR;
		if(u->order_done){MUTEX_INSPECTOR;
			if(gamers_by_id.count(u->order))
			gamers_by_id.find(u->order)->second->valed=false;
		}
		u->order=touser;
		privmsg(u->nick,_MSG("otvldone",u->gender,z->nick.c_str()));
		if(!u->order_done) action(_MSG("valready"));
		u->order_done=true;
		z->valed=true;
//		return;
	}
	if(check_all_orders()){MUTEX_INSPECTOR;
		night=false;
		morning=true;
		morning_time=time(NULL);
		action(_MSG("morning"));
	}
}

