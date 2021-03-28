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
#include "day.h"
#include <unistd.h>
#include <map>
#include "night.h"
#include "errs.h"
#include "mutex_inspector.h"


map<c_user*, map<c_user*,bool>  > vote_info1;

map<c_user*,bool> vote_info2;
c_user* vote2_person;


void do_day()
{MUTEX_INSPECTOR;
	night=false;
	morning=false;
	day=true;
	action(_MSG("day"));
	for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){MUTEX_INSPECTOR;
//		printf("process for %s role=%d valled=%d doced=%d\n",i->second->nick.c_str(),i->second->role,i->second->valed,i->second->doced);
		c_user *u=i->second;	
		if(u->marked_for_delete) continue;
		if(!u->order_done) continue;
		c_user *z=NULL;
		usleep(600000);
		if(gamers_by_id.find(u->order)!=gamers_by_id.end()) z=gamers_by_id.find(u->order)->second;
		if(!z) continue;
//		printf("target: %s role=%d valled=%d doced=%d\n",z->nick.c_str(),z->role,z->valed,z->doced);
		string add;
		int score=0;

		if(u->role==ROLE_MAF && u->main_maf){MUTEX_INSPECTOR;
			if(u->valed) continue;
			if(u->phrase.size()) add=_MSG("maf-says")+" "+u->phrase;
			if(z->doced){
				add+=" (+/-хз, доку - +20)";
				if(gamers_by_id.count(z->parent))
					gamers_by_id.find(z->parent)->second->score+=20;
				action(_MSG("maf-fail",z->gender,z->nick.c_str(),add.c_str()));
			}else{MUTEX_INSPECTOR;
				sock.response("MODE "+params["CHANNEL"]+" -v "+z->nick);
				if(z->role==ROLE_MIR) score=12;
				if(z->role==ROLE_KAT) score=60;
				if(z->role==ROLE_MAF) score=-150;
				if(z->role==ROLE_MAN) score=49;
				if(z->role==ROLE_BOM || z->role==ROLE_DOC) score=40;
				if(z->role==ROLE_VAL) score=9;
				if(score<0){
					u->score+=score;
				}else{MUTEX_INSPECTOR;
					for(map<unsigned int,c_user*>::iterator q=gamers_by_id.begin();q!=gamers_by_id.end();q++){
						if(q->second->marked_for_delete) continue;
						if(q->second->role==ROLE_MAF) q->second->score+=score;
					}
				}
				add+=" (+"+itostring(score)+")";
				z->killed=true;
				z->devoiced=true;
				action(_MSG("maf-"+get_role_shortname(z->role),z->gender,z->nick.c_str(),add.c_str()));
			}
		}

	}
	for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){MUTEX_INSPECTOR;
//		printf("process for %s role=%d valled=%d doced=%d\n",i->second->nick.c_str(),i->second->role,i->second->valed,i->second->doced);
		c_user *u=i->second;	
		if(u->marked_for_delete) continue;
		if(!u->order_done) continue;
		c_user *z=NULL;
		usleep(600000);
		if(gamers_by_id.find(u->order)!=gamers_by_id.end()) z=gamers_by_id.find(u->order)->second;
		if(!z) continue;
//		printf("target: %s role=%d valled=%d doced=%d\n",z->nick.c_str(),z->role,z->valed,z->doced);
		string add;
		int score=0;

		if(u->role==ROLE_MAN){MUTEX_INSPECTOR;
			if(u->valed) continue;
			if(u->phrase.size()) add=_MSG("man-says")+" "+u->phrase;
			if(z->doced){MUTEX_INSPECTOR;
				add+=" (+/-хз, доку - +20)";
				if(gamers_by_id.count(z->parent))
					gamers_by_id.find(z->parent)->second->score+=20;
				action(_MSG("man-fail",z->gender,z->nick.c_str(),add.c_str()));
			}else{MUTEX_INSPECTOR;
				sock.response("MODE "+params["CHANNEL"]+" -v "+z->nick);
				z->devoiced=true;
				z->killed=true;
				if(z->role==ROLE_MIR) score=14;
				if(z->role==ROLE_KAT) score=61;
				if(z->role==ROLE_MAF) score=55;
				if(z->role==ROLE_MAN) score=39;
				if(z->role==ROLE_BOM || z->role==ROLE_DOC) score=43;
				if(z->role==ROLE_VAL) score=10;
				u->score+=score;
				add+=" (+"+itostring(score)+")";
				action(_MSG("man-"+get_role_shortname(z->role),z->gender,z->nick.c_str(),add.c_str()));
			}
		}
	}
	for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){MUTEX_INSPECTOR;
//		printf("process for %s role=%d valled=%d doced=%d\n",i->second->nick.c_str(),i->second->role,i->second->valed,i->second->doced);
		c_user *u=i->second;	
		if(u->marked_for_delete) continue;
		if(!u->order_done) continue;
		c_user *z=NULL;
		usleep(600000);
		if(gamers_by_id.find(u->order)!=gamers_by_id.end()) z=gamers_by_id.find(u->order)->second;
		if(!z) continue;
//		printf("target: %s role=%d valled=%d doced=%d\n",z->nick.c_str(),z->role,z->valed,z->doced);
		string add;
		int score=0;

		if(u->role==ROLE_DOC){MUTEX_INSPECTOR;
			if(u->order==u->id) u->doc_self++;
			else u->doc_self=0;
			if(u->valed) continue;
			if(u->phrase.size()) add=_MSG("doc-says")+" "+u->phrase;
			u->score+=5;
			add+=" (+5)";
//			action(_MSG("doc-action",z->gender,z->nick.c_str(),add.c_str()));
			action(_MSG("doc-action",z->gender,add.c_str()));
		}
	}
	for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){MUTEX_INSPECTOR;
//		printf("process for %s role=%d valled=%d doced=%d\n",i->second->nick.c_str(),i->second->role,i->second->valed,i->second->doced);
		c_user *u=i->second;	
		if(u->marked_for_delete) continue;
		if(!u->order_done) continue;
		c_user *z=NULL;
		usleep(600000);
		if(gamers_by_id.find(u->order)!=gamers_by_id.end()) z=gamers_by_id.find(u->order)->second;
		if(!z) continue;
//		printf("target: %s role=%d valled=%d doced=%d\n",z->nick.c_str(),z->role,z->valed,z->doced);
		string add;
		int score=0;

		if(u->role==ROLE_KAT){MUTEX_INSPECTOR;
			if(u->valed) continue;
			if(u->phrase.size()) add=_MSG("kat-says")+" "+u->phrase;
			if(u->order_type){MUTEX_INSPECTOR;
				if(z->doced){MUTEX_INSPECTOR;
					add+=" (+/-хз, доку - +20)";
					if(gamers_by_id.count(z->parent))
						gamers_by_id.find(z->parent)->second->score+=20;
					action(_MSG("kat-fail",z->gender,z->nick.c_str(),add.c_str()));
				}else{MUTEX_INSPECTOR;
					sock.response("MODE "+params["CHANNEL"]+" -v "+z->nick);
					z->killed=true;
					z->devoiced=true;
					if(z->role==ROLE_MIR) score=-41;
					if(z->role==ROLE_KAT) score=-70;
					if(z->role==ROLE_MAF) score=58;
					if(z->role==ROLE_MAN) score=61;
					if(z->role==ROLE_BOM || z->role==ROLE_DOC) score=-45;
					if(z->role==ROLE_VAL) score=9;
					u->score+=score;
					add+=" (+"+itostring(score)+")";
					action(_MSG("kat-"+get_role_shortname(z->role),z->gender,z->nick.c_str(),add.c_str()));
					if (z->role==ROLE_BOM || z->role==ROLE_DOC || z->role==ROLE_MIR || z->role==ROLE_KAT) add=_MSG("kat-thanks")+" "+u->nick;
				}
			}else{MUTEX_INSPECTOR;
				u->score+=10;
				add+=" (+10)";
				action(_MSG("kat-check",z->gender,z->nick.c_str(),add.c_str()));
				if(!u->killed) privmsg(u->nick,"<B>"+z->nick+"</B> - <U>"+get_role_name2(z->role)+"</U>");
			}
		}
        }
	for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){MUTEX_INSPECTOR;
//		printf("process for %s role=%d valled=%d doced=%d\n",i->second->nick.c_str(),i->second->role,i->second->valed,i->second->doced);
		c_user *u=i->second;	
		if(u->marked_for_delete) continue;
		if(!u->order_done) continue;
		c_user *z=NULL;
		usleep(600000);
		if(gamers_by_id.find(u->order)!=gamers_by_id.end()) z=gamers_by_id.find(u->order)->second;
		if(!z) continue;
//		printf("target: %s role=%d valled=%d doced=%d\n",z->nick.c_str(),z->role,z->valed,z->doced);
		string add;
		int score=0;

		if(u->role==ROLE_BOM){MUTEX_INSPECTOR;
			if(u->valed) continue;
			if(u->phrase.size()) add=_MSG("bom-says")+" "+u->phrase;
			u->score+=10;
			add+=" (+10)";
			action(_MSG("bom-action",z->gender,z->nick.c_str(),add.c_str()));
			if(!u->killed) privmsg(u->nick,"<B>"+z->nick+"</B> - <U>"+get_role_name2(z->role)+"</U>");
		}
	}
	for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){MUTEX_INSPECTOR;
//		printf("process for %s role=%d valled=%d doced=%d\n",i->second->nick.c_str(),i->second->role,i->second->valed,i->second->doced);
		c_user *u=i->second;	
		if(u->marked_for_delete) continue;
		if(!u->order_done) continue;
		c_user *z=NULL;
		usleep(600000);
		if(gamers_by_id.find(u->order)!=gamers_by_id.end()) z=gamers_by_id.find(u->order)->second;
		if(!z) continue;
//		printf("target: %s role=%d valled=%d doced=%d\n",z->nick.c_str(),z->role,z->valed,z->doced);
		string add;
		int score=0;

		if(u->role==ROLE_VAL){MUTEX_INSPECTOR;
			if(u->valed) continue;
			if(u->phrase.size()) add=_MSG("val-says")+" "+u->phrase;
			int score=0;
			if(z->role==ROLE_MIR) score=25;
			if(z->role==ROLE_VAL) score=50;
			if(z->role==ROLE_MAF) score=30;
			if(z->role==ROLE_MAN) score=35;
			if(z->role==ROLE_BOM) score=20;
			if(z->role==ROLE_KAT) score=35;
			if(z->role==ROLE_DOC) score=35;
			u->score+=score;
			add+=" (+"+itostring(score)+")";
			if(z->main_maf){
				action(_MSG("val-mmaf",z->gender,add.c_str()));
			}else{
				action(_MSG("val-"+get_role_shortname(z->role),z->gender,add.c_str()));
			}
		}
	}
//	printf("erasing users\n");
	for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){
		if(i->second->marked_for_delete) continue;
		c_user *u=i->second;
		if(u->killed) u->marked_for_delete=true; // unregister_user(u->nick);
		if(u->role!=ROLE_MIR && !u->order_done){
			if(u->role==ROLE_MAF && !u->main_maf) continue;
			sock.response("MODE "+params["CHANNEL"]+" -v "+u->nick);
			action(_MSG("orderfailed",u->gender,get_role_name2(u->role).c_str(),u->nick.c_str()));
			u->marked_for_delete=true;
			u->devoiced=true;
		}
	}
	remove_left_users();
	day=true;
	day_time=time(NULL);
	if(!process_for_end_game()){MUTEX_INSPECTOR;
		check_for_main_maf();
		sleep(1);
		do_vote1();
	}
}

void do_vote1()
{MUTEX_INSPECTOR;
	action(_MSG("vote-begin"));
	action(get_gamers_list(false));
		
	vote_info1.clear();
	vote_info2.clear();
	vote2_person=NULL;
	vote1=false;
	vote2=false;
	vote1_half=false;
	vote2_half=false;
	vote_say=false;

	for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){
		i->second->voted=false;
	}
		
	vote1=true;
	vote1_time=time(NULL);
}

void process_day(unsigned int fromuser,unsigned int touser,bool dbl)
{MUTEX_INSPECTOR;
	if(!vote1) return;
	if(!gamers_by_id.count(fromuser))return;
	if(!gamers_by_id.count(touser))return;

	c_user *u=gamers_by_id.find(fromuser)->second;
	c_user *z=NULL;
	if(touser!=1000000) z=gamers_by_id.find(touser)->second;
	if(!u) return;
	if(u->marked_for_delete) return;
	if(z) if(z->marked_for_delete) return;
	for(map<c_user*, map<c_user*,bool> >::iterator i=vote_info1.begin();i!=vote_info1.end();i++){
		i->second.erase(u);
	}
	vote_info1[z][u]=true;
	u->voted=true;
	
	int cnt=get_active_users_count();
	
	string l;
	string nick;

	if(z) nick=z->nick;
	else nick="!!ночь";
	
	int total=0;
	
	for(map<c_user*,map<c_user*,bool> >::iterator i=vote_info1.begin();i!=vote_info1.end();i++){MUTEX_INSPECTOR;
		if(i->second.size()>0){
			total+=i->second.size();
			string n;
			if(i->first) n=i->first->nick;
			else n="!!ночь";
			l+="< <B>"+n+" "+itostring(i->second.size())+"</B> > ";
		}
	}
	
	action("["+itostring(total)+"/"+itostring(cnt)+"] [<U>"+u->nick+"</U> за <U>"+nick+"</U>] "+l);
	if(check_for_vote1_end()) end_vote1();
}

void invite_vote1()
{MUTEX_INSPECTOR;
	vector<string> uv;
	for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){
		if(i->second->marked_for_delete) continue;
		if(vote_info1.size()==0){MUTEX_INSPECTOR;
			uv.push_back(i->second->nick);
			break;
		}else{MUTEX_INSPECTOR;
			bool found=false;
			for(map<c_user*,map<c_user*,bool> >::iterator q=vote_info1.begin();q!=vote_info1.end();q++){
				if(q->second.find(i->second)!=q->second.end()) {
					found=true;
//					break;
				}
			}
			if(!found) uv.push_back(i->second->nick);
		}
	}
	if(vote_info1.size()) action(_MSG("vote-invite",0,join(", ",uv).c_str()));
	else action(_MSG("vote-hurryup",0,VOTE1_TIMEOUT/2));
}

bool check_for_vote1_end()
{MUTEX_INSPECTOR;
	map<int, vector<c_user*> > v;
	int tv=0;
	for(map<c_user*,map<c_user*,bool> >::iterator i=vote_info1.begin();i!=vote_info1.end();i++){
		v[i->second.size()].push_back(i->first);
		tv+=i->second.size();
	}
	int max=0;
	int all=get_active_users_count();
	map<int, vector<c_user*> >::iterator it;
	for(map<int, vector<c_user*> >::iterator i=v.begin();i!=v.end();i++){
		if(i->first>max){
			max=i->first;
			it=i;
		}
	}
//	printf("all=%d tv=%d max=%d\n",all,tv,max);
	if(max>all/2){
		if(it->second.size()==1) return true;
	}
	if(tv>=all) return true;
	return false;
}

void end_vote1()
{MUTEX_INSPECTOR;
	vote1=false;
	map<int, vector<c_user*> > v;
	for(map<c_user*,map<c_user*,bool> >::iterator i=vote_info1.begin();i!=vote_info1.end();i++){
		v[i->second.size()].push_back(i->first);
	}
	int max=0;
	map<int, vector<c_user*> >::iterator it;
	for(map<int, vector<c_user*> >::iterator i=v.begin();i!=v.end();i++){
		if(i->first>max){
			max=i->first;
			it=i;
		}
	}
	if(max>0){MUTEX_INSPECTOR;
		if(it->second.size()>1){
			action(_MSG("vote-draw"));		
			day=false;
			do_night();
			return;
		}
		if(it->second.size()==1){MUTEX_INSPECTOR;
			if(it->second[0]==NULL){
				action(_MSG("vote-night"));
				day=false;
				do_night();
				return;
			}else{MUTEX_INSPECTOR;
				c_user *u=it->second[0];
				action(_MSG("vote-say",u->gender,u->nick.c_str()));
				vote2_person=u;
				vote_say=true;
				vote_say_time=time(NULL);
			}
		}
	}else{MUTEX_INSPECTOR;
		action(_MSG("vote-draw"));
		day=false;
		do_night();
		return;
	}
}

void do_vote2()
{MUTEX_INSPECTOR;
	vote2=true;
	vote2_time=time(NULL);
	action(_MSG("vote2-begin",vote2_person->gender,vote2_person->nick.c_str()));
}

void do_vote_yes(const string &n)
{MUTEX_INSPECTOR;
	if(!gamers_by_nick.count(str_nick(n)))return;
	c_user *u=gamers_by_nick.find(str_nick(n))->second;
	if(u==vote2_person) return;
	vote_info2[u]=true;
	int cnt=get_active_users_count()-1;
	int yes=0;
	int no=0;
	for(map<c_user*,bool>::iterator i=vote_info2.begin();i!=vote_info2.end();i++){
		if(i->second) yes++;
		else no++;
	}
	action(_MSG("vote2-yes",u->gender,vote_info2.size(),cnt,yes,no,u->nick.c_str(),vote2_person->nick.c_str()));
	if(check_for_vote2_end(yes,no)) end_vote2();
}

void do_vote_no(const string &n)
{
    MUTEX_INSPECTOR;
	if(!gamers_by_nick.count(str_nick(n)))return;
	{
	MUTEX_INSPECTOR;
	c_user *u=gamers_by_nick.find(str_nick(n))->second;
	if(u==vote2_person) return;
	vote_info2[u]=false;
	int cnt=get_active_users_count()-1;
	int yes=0;
	int no=0;
	for(map<c_user*,bool>::iterator i=vote_info2.begin();i!=vote_info2.end();i++){
	    MUTEX_INSPECTOR;
		if(i->second) yes++;
		else no++;
	}
	{
	MUTEX_INSPECTOR;
	action(_MSG("vote2-no",u->gender,vote_info2.size(),cnt,yes,no,u->nick.c_str(),vote2_person->nick.c_str()));
	if(check_for_vote2_end(yes,no)) end_vote2();
	}
	}
}

void invite_vote2()
{MUTEX_INSPECTOR;
	vector<string> uv;
	for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){
		if(i->second->marked_for_delete) continue;
		if(i->second==vote2_person) continue;
		if(!vote_info2.size()) uv.push_back(i->second->nick);
		else{
			if(vote_info2.find(i->second)==vote_info2.end()) uv.push_back(i->second->nick);
		}
	}
	if(uv.size()) action(_MSG("vote2-invite",0,join(", ",uv).c_str(),vote2_person->nick.c_str()));
	else action(_MSG("vote-hurryup",0,VOTE1_TIMEOUT/2));
}

bool check_for_vote2_end(int yes,int no)
{MUTEX_INSPECTOR;
	int cnt=get_active_users_count()-1;
	if(yes+no>=cnt) return true;
	if(yes>cnt/2) return true;
	if(no>cnt/2) return true;
	return false;
}

void end_vote2()
{MUTEX_INSPECTOR;
	int cnt=get_active_users_count()-1;
	int yes=0;
	int no=0;
	for(map<c_user*,bool>::iterator i=vote_info2.begin();i!=vote_info2.end();i++){
		if(i->second) yes++;
		else no++;
	}
	if((yes+no<(cnt*40/100)) || (no>cnt/2) || (yes==no)){
		action(_MSG("vote2-notkilled",vote2_person->gender,vote2_person->nick.c_str()));
		sleep(2);
		if(time(NULL)<day_time+DAY_TIME){
			do_vote1();
		}else{
			action(_MSG("vote-draw"));
			day=false;
			do_night();
		}
		return;
	}
	if(yes>cnt/2){
		map<int,int> sc;
		if(vote2_person->role==ROLE_MAF){
			sc[ROLE_MAF]=-13; sc[ROLE_MAN]=31; sc[ROLE_KAT]=39;
			sc[ROLE_BOM]=53; sc[ROLE_DOC]=39; sc[ROLE_MIR]=53;
		}
		if(vote2_person->role==ROLE_MAN){
			sc[ROLE_MAF]=28; sc[ROLE_MAN]=28; sc[ROLE_KAT]=36;
			sc[ROLE_BOM]=48; sc[ROLE_DOC]=36; sc[ROLE_MIR]=48;
		}
		if(vote2_person->role==ROLE_KAT){
			sc[ROLE_MAF]=55; sc[ROLE_MAN]=45; sc[ROLE_KAT]=-70;
			sc[ROLE_BOM]=-65; sc[ROLE_DOC]=-55; sc[ROLE_MIR]=-47;
		}
		if(vote2_person->role==ROLE_BOM){
			sc[ROLE_MAF]=23; sc[ROLE_MAN]=23; sc[ROLE_KAT]=-32;
			sc[ROLE_BOM]=-32; sc[ROLE_DOC]=-32; sc[ROLE_MIR]=-30;
		}
		if(vote2_person->role==ROLE_DOC){
			sc[ROLE_MAF]=30; sc[ROLE_MAN]=30; sc[ROLE_KAT]=-46;
			sc[ROLE_BOM]=-46; sc[ROLE_DOC]=-46; sc[ROLE_MIR]=-43;
		}
		if(vote2_person->role==ROLE_VAL){
			sc[ROLE_MAF]=11; sc[ROLE_MAN]=11; sc[ROLE_KAT]=9;
			sc[ROLE_BOM]=9; sc[ROLE_DOC]=9;	sc[ROLE_MIR]=9;
		}
		if(vote2_person->role==ROLE_MIR){
			sc[ROLE_MAF]=10; sc[ROLE_MAN]=10; sc[ROLE_KAT]=-35;
			sc[ROLE_BOM]=-35; sc[ROLE_DOC]=-25; sc[ROLE_MIR]=-15;
		}
		sock.response("MODE "+params["CHANNEL"]+" -v "+vote2_person->nick);
		action(_MSG("vote2-kill-"+get_role_shortname(vote2_person->role),vote2_person->gender,vote2_person->nick.c_str()));
		vote2_person->marked_for_delete=true;
		vote2_person->devoiced=true;
		day=false;
		vote2=false;
		sleep(2);
		string s="За этот день заработали: ";
		for(map<int,int>::iterator i=sc.begin();i!=sc.end();i++){
			s+=get_role_name_cnt(i->first,i->second)+" - "+itostring(i->second)+" ";
		}
		for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){
			if(i->second->marked_for_delete) continue;
			if(i->second->voted) i->second->score+=sc[i->second->role];
		}
		action(s);
		sleep(2);
		if(!process_for_end_game()){
			check_for_main_maf();
			do_night();
		}
		return;
	}
	action(_MSG("vote-draw"));
	day=false;
	do_night();
}

