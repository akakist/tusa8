#include <stdio.h>
#include "c_conn.h"
#include "errs.h"
#include "config.h"
#include "utils.h"
#include "misc.h"
#include "mysql_extens.h"
#include <vector>
#include "quiz.h"
#include "antiflood.h"
#include "oscar_buffer.h"

void get_question()
{//MUTEX_INSPECTOR;
XTRY
	question_id=(rand() % questions_count)+1;
	vector<string> v=dbh->select_1_rowQ((QUERY)"select question,answer from quiz_questions where id=?"<<question_id);
	if(v.size()!=2) throw cError("select failed.");
	question=v[0];
	answer=v[1];
XPASS	
} 

void ask_question()
{//MUTEX_INSPECTOR;
XTRY
	action("- <B>"+question+"</B> ("+itostring(answer.size())+" букв)");
XPASS	
}

void check_state()
{//MUTEX_INSPECTOR;
XTRY
	time_t t=time(NULL);
	if(game_started){
		if(t>=next_question_time && !question_asked){
			get_question();
			hint1=false;
			hint2=false;
			question_asked=true;
			question_asked_time=time(NULL);
			answers_count=0;
			ask_question();
		}
		if(question_asked && t>=question_asked_time+HINT_1_TIME && !hint1){
			hint1=true;
			if(answer.size()>1){
				string hint=answer.substr(0,1);
				action("Подсказка: <B>"+hint+"</B>");
			}
		}
		if(question_asked && t>=question_asked_time+HINT_2_TIME && !hint2){
			hint2=true;
			if(answer.size()>1){
				int ll=answer.size() / 3;
				if(ll<2 && answer.size()>1) ll=2;
				string hint=answer.substr(0,ll);
				action("Подсказка: <B>"+hint+"</B>");
			}
		}
		if(question_asked && t>=question_asked_time+TIME_LIMIT){
			question_asked=false;
			next_question_time=t+QUESTION_PERIOD;
			if(answers_count==0){
				game_started=false;
				action("Время истекло. Следущего вопроса не будет, т.к. количество ответов на предыдущий вопрос=0. Для начала викторины используйте команду !старт");
			}else{
				action("Время истекло. Сдедующий вопрос через "+itostring(QUESTION_PERIOD)+" секунд.");
			}
		}
	}
XPASS	
}



void check_answer(const string &nick,const string &login,const string &ans)
{//MUTEX_INSPECTOR;
XTRY
	if(!question_asked) return;
	if(nick==params["NICKNAME"]) return;
	answers_count++;
	user *u;
	if(users.find(nick)==users.end()){
		printf("nick not found\n");
//		privmsg(nick,"Пожалуйста перезайдите в игру");
		sock.response("EUI "+nick);
		return;
	}
	u=users[nick];
//	logErr("user %s has %2.3f credits\n",nick.c_str(),float(u->credits));
#ifdef WITH_BANK
	if(u->credits<=0){
		privmsg(nick,"У Вас нет кредитов и Вы не можете играть в игру.");
		return;
	}
#endif	
	if(strupper(answer)==strupper(ans)){
		printf("answer ok nick=%s\n",nick.c_str());

		question_asked=false;
		question_asked=false;
		int sec=time(NULL)-question_asked_time;
		int score=atoi(dbh->select_1Q((QUERY)"select score from quiz_rating where login='?'"<<u->login));
		if(!score){
			score=1;
			dbh->real_queryQ((QUERY)"insert into quiz_rating (login,score) values ('?',1)"<<u->login);
		}else{
			score++;
			dbh->real_queryQ((QUERY)"update quiz_rating set score=? where login='?'"<<score<<u->login);
		}
		next_question_time=time(NULL)+QUESTION_PERIOD;
		
#ifdef WITH_BANK
		double inc_val=bank;
#else
		double inc_val=1;
#endif		
		action("Молодец "+nick+", правильный ответ - <B>"+answer+"</B> был дан за "+itostring(sec)+" сек. "+nick+" отвечает правильно на "+itostring(score)+" вопросов и зарабатывает "+ftostring(inc_val)+" кредитов. Следущий вопрос через "+itostring(QUESTION_PERIOD)+" сек.");
//		sock.response("CTTU "+nick+" "+ftostring(inc_val)+"\n");
		sock.response("CTTU "+nick+" "+ftostring(inc_val)+" quiz_prize "+itostring(1));
	}else{
//		cc+=0.01;
	}
XPASS	
}

void do_greeting(const string &nick,const string &login, int gender);

void on_greeting(const string &nick,const string &login)
{//MUTEX_INSPECTOR;
XTRY
	if(nick==params["NICKNAME"]) return;
	if(users.find(nick)==users.end()){
		sock.response("EUI "+nick);
	}else{
		printf("founded in exists\n");
		user *u=users.find(nick)->second;
		do_greeting(nick,u->login,u->gender);
	}
XPASS	
}

void do_greeting(const string &nick,const string &login,int gender)
{//MUTEX_INSPECTOR;
XTRY
	int score=atoi(dbh->select_1Q((QUERY)"select score from quiz_rating where login='?'"<<login));
	if(!score){
		privmsg(nick,"<U>Правила игры</U>: Кто быстрее ответил, тому плюсуется очко. Очки суммируются на тот login, под которым Вы сейчас находитесь. Наберите в канале <U>!help</U> для получения справки о коммандах.");
		channelmsg(nick+", Привет. Ты ни на один вопрос не ответил(а). Удачи!");
	}else{
		string pl=dbh->select_1Q((QUERY)"select count(*) from quiz_rating where score>=?"<<score);
		string suff="";
		if(gender==0) suff="о";
		if(gender==2) suff="а";
		channelmsg(nick+", Привет. Ты ответил"+suff+" правильно на "+itostring(score)+" вопросов, и занимаешь "+pl+"-е место. Удачи!");
	}	
XPASS	
}

void do_help(const string &nick)
{//MUTEX_INSPECTOR;
XTRY
	privmsg(nick,"Помощь:<br>"
		"<U>!старт</U> - начать игру<br>"
		"<U>!вон</U> - Ваше место и кол-во очков<br>"
		"<U>!вон [ник]</U> Место и кол-во очков у [ник]<br>"
		"<U>!вопрос</U> - повтор вопроса<br>"
		"<U>!top</U> - TOP 10 самых крутых игроков");
XPASS		
}

void process_command(c_command &cmd)
{//MUTEX_INSPECTOR;
XTRY
		string cdbM=params["CHAT_DB_NAME"];

	string msg=cmd.msg;
	check_for_flood(cmd);
//	printf("cmd=%s\n",cmd.cmd.c_str());
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
#ifdef WITH_BANK	
	if(cmd.cmd=="UTTC_REPLY"){
//		vector<string> pr=splitTOK(":",cmd.msg);
		printf("increment for %s from %s\n",pr[0].c_str(),pr[1].c_str());
		bank+=0.1;
		if(users.find(pr[0])!=users.end()){
			user *u=users[pr[0]];
			u->credits=atof(pr[1].c_str());
		}
	}
#endif	
	if(cmd.cmd=="CTTU_REPLY"){
		map<string,string> m=parse_strmap(cmd.msg);
#ifdef WITH_BANK		
		bank+=0.1;
		if(users.find(pr[0])!=users.end()){
			user *u=users[pr[0]];
			u->credits=atof(pr[1].c_str());
		}
#endif	
	}
	if(cmd.cmd=="EUI_REPLY"){
		map<string,string> m=parse_strmap(cmd.msg);
		string nick=m["NICK"];
		string login=m["LOGIN"];
		int gender=atoi(m["GENDER"]);
#ifdef WITH_BANK		
		double cred=atof(pr[5].c_str());
#endif		
		bool ok=false;
		for(typeof(users.begin()) i=users.begin();i!=users.end();i++){
			if(i->second->login==login){
				printf("found old\n");
				users[nick]=i->second;
				ok=true;
			}
		}
		if(!ok){
//			logErr("greating new user %s\n",login.c_str());
			user* u=new user;
			users[nick]=u;
		}
			users[nick]->login=login;
			users[nick]->gender=gender;
			users[nick]->unreg=false;
		do_greeting(nick,login,gender);
	}
	if(cmd.cmd=="PRIVMSG"){
		if(strex(msg,"!help") || strex(msg,"!хелп") || strex(msg,"!помощь")){
			do_help(cmd.fromnick);
			return;
		}
		if(strex(msg,"!вон") || strex(msg,"!djy")){
			string nick;
			string login;
			if(msg.size()==4 || msg.size()==5){
				nick=cmd.fromnick;
//				login=cmd.user;
			}else{
				if(msg.size()>5){
					nick=msg.substr(5,msg.size()-5);
				}
			}
			string uid=dbh->select_1Q((QUERY)"select ?.nicks.uid from ?.nicks where ?.nicks.str_nick='?'"<<cdbM<<cdbM<<cdbM<<str_nick(nick));
//			printf("'%s' - '%s'\n",str_nick(nick).c_str(),login.c_str());
			login=dbh->select_1Q((QUERY)"select login from ?.tbl_users where id='?'"<<cdbM<<uid);
			string score;
			if(login.size()){
				score=dbh->select_1Q((QUERY)"select score from quiz_rating where login='?'"<<login);
			}
			if(score.size()){
				string pl=dbh->select_1Q((QUERY)"select count(*) from quiz_rating where score>=?"<<score);
				privmsg(cmd.fromnick,"У пользователя "+nick+" <B>"+score+"</B> правильных ответов и он занимает <B>"+pl+"</B>-е место в рейтинге");
			}else{
				privmsg(cmd.fromnick,"В базе нет игрока с ником "+nick);
			}
			return;
		}
		if(strex(msg,"!top") || strex(msg,"!топ")){
//			string sql=
			vector<vector<string> > v=dbh->exec("select login,score from quiz_rating order by score desc limit 20");
			    
			    
			/*string sql=(string)"select "+cdbM+".tbl_users.last_nick,"
			+cdbM+".tbl_users.id,quiz_rating.score from "
			+cdbM+".tbl_users,quiz_rating "
			"where quiz_rating.login="+cdbM+".tbl_users.login order by quiz_rating.score desc limit 20";
			logErr("%s",sql.c_str());
			vector<vector<string> > v=dbh->exec(sql);
			*/
			 
			string repl="TOP 10 Самый лучших игроков:";
			for(unsigned int i=0;i<v.size();i++){
				repl+="<br>";
				if(v[i].size()==2){
					string sql="select last_nick from "+cdbM+".tbl_users where login='"+v[i][0]+"'";
					logErr("%s",sql.c_str());
					repl+=itostring(i+1)+": <U>"+dbh->select_1(sql)+"</U> - "+v[i][1];
				}
			}
			privmsg(cmd.fromnick,repl);
			return;
		}
		if(strex(msg,"!start") || strex(msg,"!старт") || strex(msg,"!cnfhn") || strex(msg,"!ыефке")){
			if(!game_started){
				game_started=true;
				action("Начинается игра \"Викторина\". Следущий вопрос через "+itostring(QUESTION_PERIOD)+" сек.");
				next_question_time=time(NULL)+QUESTION_PERIOD;
			}
			return;
		}
		if(strex(msg,"!quest") || strex(msg,"!вопрос")){
			if(question_asked) ask_question();
			return;
		}
		if(!cmd.ppriv && cmd.tonick!=params["NICKNAME"]) check_answer(cmd.fromnick,cmd.fromnick,cmd.msg);
	}
XPASS	
}

