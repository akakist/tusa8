#include "command.h"
#include <string>
#include <stdio.h>
#include "utils.h"
#include "mafia.h"
#include "errs.h"
#include "misc.h"
#include "game.h"
#include "user.h"
#include "messages.h"
#include "config.h"
#include "night.h"
#include "day.h"
#include <unistd.h>
#include "antiflood.h"
#include "config.h"
#include "mutex_inspector.h"
//#include "oscar_buffer.h"


map<string,bool> greet_send;

void process_command(c_command &cmd)
{MUTEX_INSPECTOR;
	try{
		if(cmd.cmd=="PRIVMSG"){MUTEX_INSPECTOR;
			check_for_flood(cmd);
			if(cmd.tonick==params["CHANNEL"]){MUTEX_INSPECTOR;
				process_channel_command(cmd);
			}else{MUTEX_INSPECTOR;
				process_private_command(cmd);
			}
		}
		if(cmd.cmd=="QUIT" || cmd.cmd=="KICK"){MUTEX_INSPECTOR;
			if(game_started || registration || roles_assign){MUTEX_INSPECTOR;
				string nk;
				if(cmd.cmd=="QUIT") nk=str_nick(cmd.fromnick);
				else nk=str_nick(cmd.tonick);
//				printf("nick='%s'\n",nk.c_str());
				if(gamers_by_nick.find(nk)!=gamers_by_nick.end()){MUTEX_INSPECTOR;
					c_user *u=gamers_by_nick.find(nk)->second;
					if(u->marked_for_delete) return;
					u->left=true;
					if(!registration){MUTEX_INSPECTOR;
						action(_MSG("userleft",0,u->nick.c_str()));
					}
				}
			}
		}
		if(cmd.cmd=="JOIN"){MUTEX_INSPECTOR;
			if(!voiced){MUTEX_INSPECTOR;
				sock.response("MODE "+params["CHANNEL"]+" +v "+params["NICKNAME"]);
				voiced=true;
			}
			if(!greet_send[cmd.fromnick]){MUTEX_INSPECTOR;
				greet_send[cmd.fromnick]=true;
				privmsg(cmd.fromnick,_MSG("greeting"));
			}
			if(game_started || registration || roles_assign){MUTEX_INSPECTOR;
				if(gamers_by_nick.find(str_nick(cmd.fromnick))!=gamers_by_nick.end()){MUTEX_INSPECTOR;
					c_user *u=gamers_by_nick.find(str_nick(cmd.fromnick))->second;
					if(u->marked_for_delete) return;
					if(u->chnick) return;
					if(u->left){
						u->left=false;
						if(!registration) action(_MSG("userreturned",0,u->nick.c_str()));
						sock.response("MODE "+params["CHANNEL"]+" +v "+u->nick);
					}
				}
			}
		}
		if(cmd.cmd=="NICK"){MUTEX_INSPECTOR;
			if(game_started || roles_assign || registration){MUTEX_INSPECTOR;
				if(gamers_by_nick.find(str_nick(cmd.fromnick))!=gamers_by_nick.end()){MUTEX_INSPECTOR;
					c_user *u=gamers_by_nick.find(str_nick(cmd.fromnick))->second;
					if(u->marked_for_delete) return;
					if(game_started || roles_assign){MUTEX_INSPECTOR;
						action(_MSG("nickchanging",u->gender,u->nick.c_str()));
						sock.response("MODE "+params["CHANNEL"]+" -v "+u->nick);
						u->left=true;
						u->chnick=true;
					}else{MUTEX_INSPECTOR;
						gamers_by_nick.erase(str_nick(u->nick));
						u->nick=cmd.msg;
						gamers_by_nick[str_nick(u->nick)]=u;
					}
				}
			}
		}
		if(cmd.cmd=="EUI_REPLY" && registration){MUTEX_INSPECTOR;
			map<string,string> m=parse_strmap(cmd.msg);
//			vector<string> pr=splitTOK(":",cmd.msg);
//			if(pr.size()!=7) return;
			int lev=atoi(m["LEVEL"]);
			if(lev<20){MUTEX_INSPECTOR;
			//	if(pr[3]=="1") privmsg(pr[0],_MSG("unregistered",atoi(pr[2]),pr[0].c_str()));
				//else 
				privmsg(m["NICK"],_MSG("lowlevel",atoi(m["GENDER"]),m["NICK"].c_str()));
			}else{MUTEX_INSPECTOR;
//				register_user(m["NICK"],m["LOGIN"],atoi(pr[2]),pr[6]);
				register_user(m["NICK"],m["LOGIN"],atoi(m["GENDER"]),m["IP"]);

			}
		}
	}catch(cError e){
		logErr("---Error during process command: %s",e.error_string.c_str());
	}
}

void process_day_or_night(c_command &cmd)
{MUTEX_INSPECTOR;
	string msg=cmd.msg;
//	printf("msg='%s'\n",msg.c_str());
	if(msg.size()<1) return;
	if(msg.substr(0,1)!="!") return;
	if(night || day){MUTEX_INSPECTOR;
		string d;
		bool dbl;
		if(msg.size()<2) return;
		if(msg.substr(1,1)=="!" && msg.size()>2){MUTEX_INSPECTOR;
			d=msg.substr(2,msg.size()-2);
			dbl=true;
		}else{
			d=msg.substr(1,msg.size()-1);
			dbl=false;
		}
//		printf("dbl=%d n=%s\n",dbl,d.c_str());
		int id=atoi(d);
//		printf("cond id=%d\n",id);
		if(gamers_by_nick.find(str_nick(cmd.fromnick))==gamers_by_nick.end()) return;
		int uid=gamers_by_nick.find(str_nick(cmd.fromnick))->second->id;
		if(night){MUTEX_INSPECTOR;
			if(gamers_by_id.find(uid)->second->role==ROLE_MIR) return;
		}
		if(!id){MUTEX_INSPECTOR;
			if(gamers_by_nick.find(str_nick(d))!=gamers_by_nick.end()){MUTEX_INSPECTOR;
				id=gamers_by_nick.find(str_nick(d))->second->id;
			}
		}
		if(gamers_by_id.find(id)==gamers_by_id.end()){MUTEX_INSPECTOR;
			privmsg(cmd.fromnick,_MSG("wrongcommand"));
			return;
		}
//		printf("fromid=%d toid=%d\n",uid,id);
//		printf("process!!!!!!!\n");
		if(day) process_day(uid,id,dbl);
		if(night) process_night(uid,id,dbl);
	}

}

void process_channel_command(c_command &cmd)
{MUTEX_INSPECTOR;
	string msg=cmd.msg;
//	if()
	if(day){MUTEX_INSPECTOR;
		if(msg.find("!list")!=-1 || msg.find("!список")!=-1){MUTEX_INSPECTOR;
			action(get_gamers_list(false).c_str());
			return;
		}
		if(msg.find("!!night")!=-1 || msg.find("!!ночь")!=-1 || msg.find("!!yjxm")!=-1){MUTEX_INSPECTOR;
			if(gamers_by_nick.count(str_nick(cmd.fromnick)))
			{
				int uid=gamers_by_nick.find(str_nick(cmd.fromnick))->second->id;
				process_day(uid,1000000,false);
			}
			return;
		}
		if(vote2 && !vote_say){MUTEX_INSPECTOR;
			if(strex(msg,"!да") || strex(msg,"!нуы") || strex(msg,"!yes") || strex(msg,"!lf")){MUTEX_INSPECTOR;
				do_vote_yes(cmd.fromnick);
			}
			if(strex(msg,"!нет") || strex(msg,"!тщ") || strex(msg,"!no") || strex(msg,"!ytn")){MUTEX_INSPECTOR;
				do_vote_no(cmd.fromnick);
			}
			return;
		}
		if(vote_say){MUTEX_INSPECTOR;
			if(strex(msg,"!все") || strex(msg,"!all") || strex(msg,"!dct") || strex(msg,"!фдд")){MUTEX_INSPECTOR;
				if(gamers_by_nick.count(str_nick(cmd.fromnick)))
				{
				c_user *u=gamers_by_nick.find(str_nick(cmd.fromnick))->second;
				if(u==vote2_person) vote_say_time=0;
				}
				return;
			}
		}
	}
	if(strex(msg,"!")){MUTEX_INSPECTOR;
		//help
		if(strex(msg,"!help") || strex(msg,"!хелп") || strex(msg,"!comm")){MUTEX_INSPECTOR;
			do_help(cmd);
			return;
		}
		if(strex(msg,"!rules") || strex(msg,"!правила") || strex(msg,"!кглуы")){MUTEX_INSPECTOR;
			do_rules(cmd);
			return;
		}
		if(strex(msg,"!start") || strex(msg,"!старт") || strex(msg,"!cnfhn") || strex(msg,"!ыефке")){MUTEX_INSPECTOR;
			start_game(cmd);
			return;
		}
		if(strex(msg,"!reg") || strex(msg,"!htu") || strex(msg,"!куп") || strex(msg,"!рег")){MUTEX_INSPECTOR;
			if(!registration) return;
			if(gamers_by_nick.find(str_nick(cmd.fromnick))==gamers_by_nick.end() && str_nick(cmd.fromnick)!=str_nick(params["NICKNAME"])){MUTEX_INSPECTOR;
				sock.response("EUI "+cmd.fromnick);
			}
			return;
		}
		if(strex(msg,"!unreg") || strex(msg,"!гткуп")){MUTEX_INSPECTOR;
			if(registration) unregister_user(cmd.fromnick);
			return;
		}
		if(strex(msg,"!stat") || strex(msg,"!cnfn") || strex(msg,"!стат") || strex(msg,"!ыефе")){MUTEX_INSPECTOR;
			if(game_started) show_roles();
			return;
		}
	}
	if(day || night){MUTEX_INSPECTOR;
		process_day_or_night(cmd);
		return;
	}
	if(registration)
	{MUTEX_INSPECTOR;
		string cdbM=params["CHAT_DB_NAME"];

		if(strex(msg,"!вон") || strex(msg,"!djy")){
			string nick;
			//string login;
			string ref$id;
			if(msg.size()==4 || msg.size()==5){MUTEX_INSPECTOR;
				nick=cmd.fromnick;
				ref$id=dbh->select_1Q((QUERY)"select ?.nicks.uid from ?.nicks where ?.nicks.str_nick='?'"<<cdbM<<cdbM<<cdbM<<str_nick(nick));
				//login=cmd.user;
			}else{MUTEX_INSPECTOR;
				if(msg.size()>5){
					nick=msg.substr(5,msg.size()-5);
				}
				ref$id=dbh->select_1Q((QUERY)"select ?.nicks.uid from ?.nicks where ?.nicks.str_nick='?'"<<cdbM<<cdbM<<cdbM<<str_nick(nick));
			}
			vector<string> score;
			string login=dbh->select_1Q((QUERY)"select login from ?.tbl_users where id='?'"<<cdbM<<ref$id);
			if(login.size()){MUTEX_INSPECTOR;
				score=dbh->select_1_rowQ((QUERY)"select score,win,loose from  mafia_score where login='?'"<<login);
			}
			if(score.size()==3){MUTEX_INSPECTOR;
				channelmsg("На счету у <U>"+nick+"</U> - <U>"+score[0]+"</U> очков, играл(а) в <U>"+itostring(atoi(score[1])+atoi(score[2]))+"</U> игр, выигрышей - <U>"+score[1]+"</U>, проигрышей - <U>"+score[2]+"</U>");
			}else{
				channelmsg("В базе нет игрока с ником "+nick);
			}
			return;
		}
		if(strex(msg,"!place") || strex(msg,"!здфсу")){MUTEX_INSPECTOR;
			string nick;
//			string login;
			string ref$id;
			if(msg.size()==6 || msg.size()==7){MUTEX_INSPECTOR;
				nick=cmd.fromnick;
//				login=cmd.user;
				ref$id=dbh->select_1Q((QUERY)"select ?.nicks.uid from ?.nicks where ?.nicks.str_nick='?'"<<cdbM<<cdbM<<cdbM<<str_nick(nick));
			}else{
				if(msg.size()>7){MUTEX_INSPECTOR;
					nick=msg.substr(7,msg.size()-7);
				}

				ref$id=dbh->select_1Q((QUERY)"select ?.nicks.uid from ?.nicks where ?.nicks.str_nick='?'"<<cdbM<<cdbM<<cdbM<<str_nick(nick));
			}

			string login=dbh->select_1Q((QUERY)"select login from ?.tbl_users where id='?'"<<cdbM<<ref$id);
			
//			printf("in place: n=%s l=%s\n",nick.c_str(),login.c_str());
			if(login.size()){MUTEX_INSPECTOR;
				string score=dbh->select_1Q((QUERY)"select score from  mafia_score where login='?'"<<login);
				string pl=dbh->select_1Q((QUERY)"select count(*) from  mafia_score where score>=?"<<score);
				channelmsg("<U>"+nick+"</U> занимает <U>"+pl+"</U> место в рейтинге.");
			}else{
				channelmsg("В базе нет игрока с ником "+nick);
			}
			return;
		}
		if(strex("!top",msg) || strex("!топ",msg)){MUTEX_INSPECTOR;
			vector< vector<string> > v=dbh->exec(
			"select "+cdbM+".tbl_users.last_nick, mafia_score.score from "
			+cdbM+".tbl_users,  mafia_score where "
			" mafia_score.login="+cdbM+".tbl_users.login  "
			" order by  mafia_score.score desc limit 10"
			);
			string repl="</B></B><B>TOP 10</B> Самых лучших игроков:";
			for(unsigned int i=0;i<v.size();i++){
				repl+="<br>";
				if(v[i].size()==2){
					/*oscar_buffer b;
					b.Pack(v[i][0]);
					user_state st;
					b>>st;*/
				
					if(i==0){
						repl+="<B>"+itostring(i+1)+": <U>"+v[i][0] +"</U> - "+v[i][1]+"</B>";
					}else{
						repl+=itostring(i+1)+"</B>: <U>"+v[i][0]+"</U> - "+v[i][1];
					}
				}
			}
			privmsg(cmd.fromnick,repl);
			return;
			
		}
	}
}

void process_private_command(c_command &cmd)
{MUTEX_INSPECTOR;
	string msg=cmd.msg;
	if(strex(msg,"!halt")){
		if(access_users.find(cmd.fromnick)!=access_users.end()){
			if(game_started || registration || roles_assign){MUTEX_INSPECTOR;
				action("Игра остановлена администратором!");
				sock.response("MODE "+params["CHANNEL"]+" -m");
				clear_users();
				registration=false;
				roles_assign=false;
				night=false;
				day=false;
				morning=false;
				game_started=false;
				return;
			}
		}
	}
	if(strex(msg,"!lf") || strex(msg,"!yes") || strex(msg,"!да") || strex(msg,"!нуы")){MUTEX_INSPECTOR;
		if(roles_assign){MUTEX_INSPECTOR;
			accept_role(cmd.fromnick);
			return;
		}
	}
	if(strex(msg,"!ytn") || strex(msg,"!no") || strex(msg,"!нет") || strex(msg,"!тщ")){MUTEX_INSPECTOR;
		if(roles_assign){MUTEX_INSPECTOR;
			decline_role(cmd.fromnick,true);
			return;
		}
	}
//	printf("processing1p d=%d n=%d\n",day,night);
	if(night || morning){MUTEX_INSPECTOR;
		if(strex(msg,"!фраза") || strex(msg,"!ahfpf")){MUTEX_INSPECTOR;
			add_phrase(cmd);
			return;
		}
	}
	if(day || night){MUTEX_INSPECTOR;
		if(strex(msg,"!мафия") || strex(msg,"!mafia")){MUTEX_INSPECTOR;
			if(gamers_by_nick.find(str_nick(cmd.fromnick))!=gamers_by_nick.end()){MUTEX_INSPECTOR;
				c_user *u=gamers_by_nick.find(str_nick(cmd.fromnick))->second;
				if(!u->marked_for_delete){MUTEX_INSPECTOR;
					if(u->role==ROLE_MAF){MUTEX_INSPECTOR;
						string s=get_mafia();
						privmsg(cmd.fromnick,s);
					}
				}
			}
			return;
		}
	}
	if(day || night){MUTEX_INSPECTOR;
//		printf("processing1p\n");
		process_day_or_night(cmd);
		return;
	}
}

