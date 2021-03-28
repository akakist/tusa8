#include <stdio.h>
#include "c_conn.h"
#include "errs.h"
#include "config.h"
#include "utils.h"
#include "misc.h"
#include "mysql_extens.h"
#include "game.h"
#include "quiz.h"
#include <signal.h>
#define CONFIG_FILE "quiz.conf"

cMYSQL *dbh;

bool game_started;
time_t next_question_time;
bool question_asked;
time_t question_asked_time;
int answers_count;
bool hint1;
bool hint2;

string question;
string answer;
int question_id;
int questions_count;
string dump_mutex_inspectors ();

map<string,user*> users;

void onterm(int signum)
{
	if(signum==1){
		load_config(CONFIG_FILE);
		logErr("Config reloaded.");
	}else{
            string out;
        signal(SIGPIPE,SIG_IGN);  
	    signal(SIGABRT,SIG_IGN);  
	    signal(SIGTERM,SIG_IGN);  
	    signal(SIGINT,SIG_IGN);   
	    signal(SIGSEGV,SIG_IGN);  
	    signal(SIGFPE,SIG_IGN);   
	    signal(SIGQUIT,SIG_IGN);  
	    signal(SIGILL,SIG_IGN);   
	    signal(SIGBUS,SIG_IGN);   
	    signal(SIGHUP,SIG_IGN);   
#ifdef DEBUG
            out += dump_mutex_inspectors ();
#endif			
            string fn;
                fn = (string) "TERM." + itostring (time (NULL));
            if (out.size () ||true)
            {
                out = "Terminating on SIGNAL " + itostring (signum) + "\n" + out;

                FILE *f = fopen (fn.c_str (), "wb");
                if (f)
                {
                    fprintf (f, "%s", out.c_str ());
                    fclose (f);
                }
            }
	
		logErr("Got signal %d, terminating...",signum);
		exit(1);
	}
}

int main(int argc,char* argv[])
{
	printf("BOT Quiz v1.1.0 (C) www.belialov.com\n");
	if(argc>1){
		if(strcmp(argv[1],"-d")==0){
			printf("Continuing as daemon\n");
			if(fork()) exit(1);
		}
		if(strcmp(argv[1],"--help")==0){
			printf("\t -d - to run as deamon\n");
			exit(1);
		}
	}
	
	signal(SIGPIPE,SIG_IGN);
	signal(SIGABRT,onterm);
	signal(SIGTERM,onterm);
	signal(SIGINT,onterm);
	signal(SIGSEGV,onterm);
	signal(SIGFPE,onterm);
	signal(SIGQUIT,onterm);
	signal(SIGILL,onterm);
	signal(SIGBUS,onterm);
	signal(SIGHUP,onterm);
	try{
	load_config(CONFIG_FILE);
	srandom(time(NULL));
	logErr("Bot started.");
	dbh=std_new_cMYSQL();
	}catch(cError e){
		printf("%s\n",e.error_string.c_str());
	}
	game_started=false;
	next_question_time=0;
	question_asked=0;
	question_asked_time=0;
	answers_count=0;
	hint1=false;
	hint2=false;

	questions_count=-1;
		
	bool logined=false;
	bool joined=false;
	bool greeting_sent=false;
	while(1){
	if(greeting_sent) logErr("Connection lost. Reconnecting...");
	else logErr("Connecting...");
	try{
		if(questions_count==-1) questions_count=atoi(dbh->select_1("select count(*) from quiz_questions"));
		connect_to_server();
		while(sock.connected()){
		XTRY	
			if(!logined){
				sock.response("USER bot bot bot: Botter");
				sock.response("NICK "+params["NICKNAME"]);
				sock.response("PASS "+params["PASSWD"]);
				logined=true;
			}
			if(!sock.readable(0)){
				usleep(10000);
				check_state();
				continue;
			}
			if(!joined){
				sock.response("JOIN "+params["CHANNEL"]);
				joined=true;
			}
			if(!greeting_sent){
				greeting_sent=true;
				action("Бот игры \"Викторина\" приветствует всех! Для начала игры введите !старт. Вопросов в базе "+itostring(questions_count));
			}
			string data=sock.gets();
#ifdef DEBUG
			printf("|>> %s\n",data.c_str());
#endif			
			logErr("|>> %s",data.c_str());

			//process received buffer

			if(data.find(":"+params["HOST"]+" 433 * :"+params["NICKNAME"]+": Nick is already in use.")!=-1){
				printf("This nickname is already in use. May be bot is already running.\n");
				logErr("This nickname is already in use. May be bot is already running.");
				sock.disconnect();
				exit(1);
			}
			
			if(data.substr(0,1)!="."){
				if(data.substr(0,4)=="PING"){
					string h=data.substr(5,data.size()-5);
					sock.response("PONG "+h);
					continue;
				}
			}
			c_command cmd=parse_string(data);
			process_command(cmd);
		XPASS	
		}
		
	
	}catch(socketError e){
		logErr("---Socket error: %s",e.error_string.c_str());
	}catch(cError e){
		logErr("---Error: %s",e.error_string.c_str());
	}catch(...){
		logErr("---Error: unknow!");
	}
	sock.disconnect();
	logined=false;
	joined=false;
	sleep(5);
	}
}

