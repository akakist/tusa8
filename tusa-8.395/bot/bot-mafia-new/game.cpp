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
#include <unistd.h>
#include "day.h"
#include "errs.h"
#include "mutex_inspector.h"

time_t reg_start_time;

bool game_started;
bool registration;
bool roles_assign;
bool night;
bool day;

bool morning;
time_t morning_time;

time_t roles_assign_time;

bool roles_assign_half;

time_t night_time;

time_t day_time;

bool inv1sent;

bool voiced;

bool vote1;
bool vote1_half;
time_t vote1_time;

bool vote2;
bool vote2_half;
time_t vote2_time;

bool vote_say;
time_t vote_say_time;

void start_game(const c_command &)
{MUTEX_INSPECTOR;
	if(registration || game_started || roles_assign) return;
	if(!voiced){
		sock.response("MODE "+params["CHANNEL"]+" +v "+params["NICKNAME"]);
		voiced=true;
	}
	clear_users();
	action(_MSG("startgame"));
	reg_start_time=time(NULL);
	registration=true;
	inv1sent=false;
	roles_assign=false;
	roles_assign_half=false;
	night=false;
	day=false;
	morning=false;
	register_counter=0;
	game_started=false;
	vote1=false;
	vote2=false;
	vote1_half=false;
	vote2_half=false;
	vote_say=false;
}

void remove_left_users()
{MUTEX_INSPECTOR;
	for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){
		c_user *u=i->second;
		if(u->marked_for_delete) continue;
		if(u->left){
			action(_MSG("userkilled",u->gender,get_role_name2(u->role).c_str(),u->nick.c_str()));
			u->devoiced=true;
			u->marked_for_delete=true;
		}
	}
}


void assign_role(int cnt,int role)
{MUTEX_INSPECTOR;
	int sz=gamers_by_id.size();
	int cc=0;
	while(cnt>0){
		int z=(rand()%sz)+1;
		if(gamers_by_id.find(z)!=gamers_by_id.end()){
			c_user *u=gamers_by_id[z];
			if(u->role==ROLE_MIR){
				u->role=role;
				cnt--;
				privmsg(u->nick,_MSG("you"+itostring(role),u->gender));
			}
		}
		cc++;
		if(cc>1000) break;
	}

}

void assign_roles()
{MUTEX_INSPECTOR;
	int sz=gamers_by_nick.size();
	int mir=0;
	int maf=0;
//	int mir=(sz*3/5);
//	int maf=(sz*25/100);
	if(sz>20){
		mir=(sz*45/100);
		maf=(sz*25/100);
	}else{
		mir=(sz*40/100);
		maf=(sz*25/100);
	}
	if (mir<1) mir=1;
	if (maf<1) maf=1;
	int kat=1;
	int doc=0;
	int man=0;
	int bom=0;
	int val=0;
	int left=sz-maf-kat-mir;
	while(left>0){
		if(left>0) { if(doc>0 && sz<30*doc){} else{doc++; left--;} }
		if(left>0) { if(man>0 && sz<10*man){} else{man++; left--;} }
		if(left>0) { if(bom>0 && sz<15*man){} else{bom++; left--;} }
		if(val>0 && left>0) { if(kat>0 && sz<20*kat){}else{kat++; left--;} }
		if(left>0) { if(val>0 && sz<30*val){} else{val++; left--;} }
		if(left>0) {left--; mir++;}
	}
	assign_role(doc,ROLE_DOC);
	assign_role(val,ROLE_VAL);
	assign_role(bom,ROLE_BOM);
	assign_role(man,ROLE_MAN);
	assign_role(kat,ROLE_KAT);
	assign_role(maf,ROLE_MAF);

}

string get_mafia()
{MUTEX_INSPECTOR;
	string mafs;
	map<int,c_user*> mf;
	for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){
		if(i->second->marked_for_delete) continue;
		mf[i->second->maf_n]=i->second;
	}
	for(map<int,c_user*>::iterator i=mf.begin();i!=mf.end();i++){
		if(i->second->marked_for_delete) continue;
		if(i->second->role!=ROLE_MAF) continue;
		mafs+=itostring(i->second->maf_n)+". "+i->second->nick+" ";
	}
	mafs="Все мафы: [ "+mafs+"]";
	return mafs;
}

void make_main_maf(bool init)
{MUTEX_INSPECTOR;
	if(init){MUTEX_INSPECTOR;
		map<int,c_user*> mf;
		for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){
			if(i->second->marked_for_delete) continue;
			if(i->second->role==ROLE_MAF){
				int r=rand();
				while (mf.find(r)!=mf.end()) r=rand();
				mf[rand()]=i->second;
			}
		}
		if(mf.size()>1){MUTEX_INSPECTOR;
			int n=1;
			int nn=1;
			string mafs;
			for(map<int,c_user*>::iterator i=mf.begin();i!=mf.end();i++){
				if(i->second->marked_for_delete) continue;
				mafs+=itostring(nn)+". "+i->second->nick+" ";
				nn++;
			}
			mafs="Все мафы: [ "+mafs+"]";
			for(map<int,c_user*>::iterator i=mf.begin();i!=mf.end();i++){
				if(i->second->marked_for_delete) continue;
				i->second->maf_n=n;
				privmsg(i->second->nick,_MSG("youmaf",i->second->gender,n));
				privmsg(i->second->nick,mafs);
				n++;
			}
		}else{MUTEX_INSPECTOR;
			if(mf.size()){
				c_user *z=mf.begin()->second;
				privmsg(z->nick,_MSG("youmafalone",z->gender));
				z->maf_n=1;
				z->main_maf=true;
			}
		}
	}
	c_user *u=NULL;
	unsigned int n=100000;
	for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){
		if(i->second->marked_for_delete) continue;
		if(i->second->role==ROLE_MAF && i->second->maf_n<n){
			n=i->second->maf_n;
			u=i->second;
		}
	}
	if(u){
		u->main_maf=true;
	}
}

void check_for_main_maf()
{MUTEX_INSPECTOR;
	bool mm=false;
	for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){
		if(i->second->marked_for_delete) continue;
		if(i->second->main_maf) mm=true;
	}
	if(!mm) make_main_maf(false);
}

void process_game()
{MUTEX_INSPECTOR;
	time_t ct=time(NULL);
	if(registration){MUTEX_INSPECTOR;
		if(ct-reg_start_time>REG1_TIMEOUT && !inv1sent){		//60
			action(_MSG("gamestarted1"));
			inv1sent=true;
		}
		if(ct-reg_start_time>REG2_TIMEOUT){			//120
			if(gamers_by_nick.size()<3){
				action(_MSG("cantstartgame"));
				clear_users();
				registration=false;
				game_started=false;
				return;
			}
			action(_MSG("regend",0,gamers_by_nick.size()));
			assign_roles();
			registration=false;
			roles_assign=true;
			roles_assign_time=time(NULL);
		}
		return;
	}
	if(roles_assign && !roles_assign_half && ct-roles_assign_time>ROLE_ACCEPT_TIME/2){MUTEX_INSPECTOR;
		roles_assign_half=true;
		vector<c_user*> v;
		for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){MUTEX_INSPECTOR;
			c_user *u=i->second;
//			if(u->marked_for_delete) continue
			if(u->role==ROLE_MIR) continue;
			if(!u->accepted){
				v.push_back(u);
			}
		}
		for(unsigned int i=0;i<v.size();i++){
			decline_role(v[i]->nick,false);
		}
	}
	if(roles_assign && ct-roles_assign_time>ROLE_ACCEPT_TIME){MUTEX_INSPECTOR;
		check_roles();
		roles_assign=false;
		make_main_maf(true);
		action(_MSG("gamestarted2"));
		sleep(2);
		sock.response("MODE "+params["CHANNEL"]+" +m");
		show_roles();
		game_started=true;
		for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){MUTEX_INSPECTOR;
			c_user *u=i->second;
			if(u->marked_for_delete) continue;
			u->need_update=true;
		}
		do_night();
	}
	if(night && ct-night_time>NIGHT_TIME){MUTEX_INSPECTOR;
		night=false;
		do_day();
	}
	if(morning && ct-morning_time>MORNING_TIME){MUTEX_INSPECTOR;
		morning=false;
		do_day();
	}
	if(day){MUTEX_INSPECTOR;
		if(vote1 && !vote1_half && ct-vote1_time>VOTE1_TIMEOUT/2){
			invite_vote1();
			vote1_half=true;
		}
		if(vote1 && ct-vote1_time>VOTE1_TIMEOUT){MUTEX_INSPECTOR;
			end_vote1();
			vote1=false;
		}
		if(vote_say && ct-vote_say_time>VOTE_SAY_TIMEOUT){MUTEX_INSPECTOR;
			do_vote2();
			vote_say=false;
		}
		if(vote2 && !vote2_half && ct-vote2_time>VOTE2_TIMEOUT/2){MUTEX_INSPECTOR;
			invite_vote2();
			vote2_half=true;
		}
		if(vote2 && ct-vote2_time>VOTE2_TIMEOUT){MUTEX_INSPECTOR;
			end_vote2();
			vote2=false;
		}
	}
}


void accept_role(const string &n)
{MUTEX_INSPECTOR;
	if(gamers_by_nick.find(str_nick(n))!=gamers_by_nick.end()){MUTEX_INSPECTOR;
		c_user *u=gamers_by_nick[str_nick(n)];
		if(u->role!=ROLE_MIR){
			if(!u->accepted){
				u->accepted=true;
				privmsg(n,_MSG("yourroleaccepted"));
			}
		}
	}
	bool allacc=true;
	for(map<string,c_user*>::iterator i=gamers_by_nick.begin();i!=gamers_by_nick.end();i++){MUTEX_INSPECTOR;
		if(i->second->marked_for_delete) continue;
		if(i->second->role!=ROLE_MIR && !i->second->accepted) allacc=false;
	}
	if(allacc) roles_assign_time-=ROLE_ACCEPT_TIME;
}

void decline_role(const string &n,bool sam)
{MUTEX_INSPECTOR;
	if(gamers_by_nick.find(str_nick(n))!=gamers_by_nick.end()){MUTEX_INSPECTOR;
		c_user *u=gamers_by_nick[str_nick(n)];
		if(u->role!=ROLE_MIR){
			if(sam) action(_MSG("roledeclined",0,get_role_name2(u->role).c_str()));
			else action(_MSG("rolenotaccepted",0,get_role_name2(u->role).c_str()));
			assign_role(1,u->role);
			u->role=ROLE_MIR;
			u->score-=50;
			privmsg(n,_MSG("yourroledeclined"));
		}
	}
}

void check_roles()
{MUTEX_INSPECTOR;
	for(map<string,c_user*>::iterator i=gamers_by_nick.begin();i!=gamers_by_nick.end();i++){MUTEX_INSPECTOR;
		if(i->second->marked_for_delete) continue;
		if(i->second->role!=ROLE_MIR && !i->second->accepted){
			if(i->second->role==ROLE_VAL) action(_MSG("roleremoved2",0,get_role_name2(i->second->role).c_str()));
			else action(_MSG("roleremoved",0,get_role_name2(i->second->role).c_str()));
			i->second->role=ROLE_MIR;
		}
	}
}

void show_roles()
{MUTEX_INSPECTOR;
	map<int,int> r;
	for(map<string,c_user*>::iterator i=gamers_by_nick.begin();i!=gamers_by_nick.end();i++){MUTEX_INSPECTOR;
		if(i->second->marked_for_delete) continue;
		r[i->second->role]++;
	}
	string res;
	for(map<int,int>::iterator i=r.begin();i!=r.end();i++){
		res+="< <B>"+get_role_name(i->first)+" "+itostring(i->second)+"</B> > ";
	}
	action("В игре: "+res);
}

string get_gamers_list(bool mafs)
{MUTEX_INSPECTOR;
	string r;
	for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){
		if(i->second->marked_for_delete) continue;
		if(mafs && i->second->role==ROLE_MAF){
			r+="<B>"+itostring(i->first)+". "+i->second->nick+"</B> ";
		}else{
			r+=itostring(i->first)+". "+i->second->nick+" ";
		}
	}
	if(mafs){
		return "Список: [ "+r+" ]";
	}else{
		return "Список: [ <B>"+r+"</B> ]";
	}
}

bool check_all_orders()
{MUTEX_INSPECTOR;
	for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){
		if(i->second->marked_for_delete) continue;
		c_user *u=i->second;
		if(u->role==ROLE_MAF && !u->main_maf) continue;
		if(u->role==ROLE_MIR) continue;
		if(!u->order_done) return false;
	}
	return true;
}


bool process_for_end_game()
{MUTEX_INSPECTOR;
	bool end=false;
//	printf("check game state\n");
	unsigned int winner=0;
	bool draw=false;
	int tmir=0;
	int tbad=0;
	map<unsigned int,vector<c_user*> > rls;
	map<unsigned int,vector<c_user*> > rlsabs;
	for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){MUTEX_INSPECTOR;
		rlsabs[i->second->role].push_back(i->second);
		if(i->second->marked_for_delete) continue;
		rls[i->second->role].push_back(i->second);
	}
//	for(map<unsigned int,vector<c_user*> >::iterator i=rls.begin();i!=rls.end();i++){
//		printf("role=%s cnt=%d\n",get_role_name2(i->first).c_str(),i->second.size());
//		logErr("role=%s cnt=%d\n",get_role_name2(i->first).c_str(),i->second.size());
//	}
	tmir=rls[ROLE_MIR].size() + rls[ROLE_KAT].size() + rls[ROLE_DOC].size() + rls[ROLE_BOM].size();
	tbad=rls[ROLE_MAF].size()+rls[ROLE_MAN].size();
	if(rls[ROLE_MAF].size()==0 && rls[ROLE_MAN].size()==0){MUTEX_INSPECTOR;
		winner=ROLE_MIR;
		end=true;
	}
	if(rls[ROLE_MAF].size()>=tmir && tmir<3 && rls[ROLE_KAT].size()==0){MUTEX_INSPECTOR;
		winner=ROLE_MAF;
		end=true;
	}
	if(tmir<2 && rls[ROLE_MAN].size()>0 && rls[ROLE_MAF].size()==0 && rls[ROLE_KAT].size()==0){MUTEX_INSPECTOR;
		end=true;
		winner=ROLE_MAN;
	}
	if(rls[ROLE_MAF].size()==1 && rls[ROLE_KAT].size()==1 && tmir==1){MUTEX_INSPECTOR;
		end=true;
		draw=true;
	}
	if(rls[ROLE_MAF].size()==1 && rls[ROLE_MAN].size()==1 && tmir<2){MUTEX_INSPECTOR;
		end=true;
		draw=true;
	}
	if(rls[ROLE_MAN].size()==1 && rls[ROLE_KAT].size()==1 && tmir==1 && tbad==1){MUTEX_INSPECTOR;
		end=true;
		draw=true;
	}
	
	map<int,int> sc;
	map<int,bool> sr;
	
	
	if(end){MUTEX_INSPECTOR;
		sleep(2);
		if(draw){MUTEX_INSPECTOR;
			action(_MSG("gameend-draw"));
			sr[ROLE_MIR]=true;
			sr[ROLE_MAF]=true;
			sr[ROLE_MAN]=true;
			sr[ROLE_KAT]=true;
			sr[ROLE_DOC]=true;
			sr[ROLE_BOM]=true;
		}else{MUTEX_INSPECTOR;
			if(winner==ROLE_MAF || winner==ROLE_MAN){
				if(winner==ROLE_MAF) sr[ROLE_MAF]=true;
				if(winner==ROLE_MAN) sr[ROLE_MAN]=true;
			}else{MUTEX_INSPECTOR;
				sr[ROLE_MIR]=true;
				sr[ROLE_KAT]=true;
				sr[ROLE_DOC]=true;
				sr[ROLE_BOM]=true;
			}
			
			action(_MSG("gameend-"+get_role_shortname(winner)));
		}
//		sleep(1);
		if(draw){MUTEX_INSPECTOR;
			sc[ROLE_MIR]=25;
			sc[ROLE_MAF]=25;
			sc[ROLE_MAN]=25;
			sc[ROLE_KAT]=25;
			sc[ROLE_DOC]=25;
			sc[ROLE_BOM]=25;
		}else{MUTEX_INSPECTOR;
			if(winner==ROLE_MAF || winner==ROLE_MAF){MUTEX_INSPECTOR;
				sc[ROLE_MAF]=50;
				sc[ROLE_MAN]=50;
			}else{MUTEX_INSPECTOR;
				sc[ROLE_MIR]=50;
				sc[ROLE_KAT]=50;
				sc[ROLE_DOC]=50;
				sc[ROLE_BOM]=50;
			}
		}
		
		string s;
		for(map<unsigned int,c_user*>::iterator i=gamers_by_id.begin();i!=gamers_by_id.end();i++){MUTEX_INSPECTOR;
			c_user *u=i->second;
			if(u->marked_for_delete) continue;
			u->score+=sc[u->role];
			u->win=true;
		}
		for(map<int,bool>::iterator i=sr.begin();i!=sr.end();i++){MUTEX_INSPECTOR;
			if(i->second && rls[i->first].size()){
				s+=get_role_name_cnt(i->first,rls[i->first].size())+" ";
			}
		}
//		sleep(1);
		if(draw){
			action(_MSG("draw",0,s.c_str()));
		}else{
			action(_MSG("winners",0,s.c_str()));
		}
		sleep(2);
		action("-------------------------------");
		action(_MSG("roles"));
		sleep(1);
		for(map<unsigned int,vector<c_user*> >::iterator i=rlsabs.begin();i!=rlsabs.end();i++){MUTEX_INSPECTOR;
			if(i->second.size()>0){
				string ss;
				for(unsigned int q=0;q<i->second.size();q++){
					ss+="<B>"+i->second[q]->nick+"</B> ("+itostring(i->second[q]->score)+") ";
					double f=i->second[q]->score;
					f=f/200;
					f*=10;
					sock.response("CTTU "+i->second[q]->nick+" "+ftostring(f)+" maf_prize "+itostring(q));
				}
//				join(", ",i->second)
				action("<U>"+get_role_name_cnt(i->first,i->second.size())+"</U>: "+ss);
				usleep(500000);
			}
		}
		sock.response("MODE "+params["CHANNEL"]+" -m");
		clear_users();
		registration=false;
		roles_assign=false;
		night=false;
		day=false;
		morning=false;
		game_started=false;
	}
	return end;
}

