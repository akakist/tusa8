#include <stdio.h>
#include "c_conn.h"
#include "errs.h"
#include "config.h"
#include "utils.h"
#include "misc.h"
#include "mysql_extens.h"
#include "game.h"
#include "roulette.h"
#include <signal.h>
#define CONFIG_FILE "roulette.conf"

cMYSQL *dbh;

string dump_mutex_inspectors ();
void load_messages();
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
	printf("BOT Roulette v1.1.0 (C) www.belialov.com\n");
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
//	printf("kall\n");
	load_config(CONFIG_FILE);
	srandom(time(NULL));
	srand(time(NULL)-rand());
//	printf("kall\n");
	logErr("Bot started.");
	dbh=std_new_cMYSQL();
//	printf("kall3\n");
	}catch(cError e){
		printf("%s\n",e.error_string.c_str());
	}
		
	bool logined=false;
	bool joined=false;
	bool greeting_sent=false;
	load_messages();
	while(1){
	if(greeting_sent) logErr("Connection lost. Reconnecting...");
	else logErr("Connecting...");
	try{
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
				action("Бот игры \"Рулетка\" приветствует всех! Для начала игры введите !старт.");
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

user* get_user(const string& login)
{
    for(map<string,user*>::iterator i=users.begin();i!=users.end();i++)
    {
	if(i->second)
	{
	    if(i->second->login==login) 
	    {
//		logErr("found login %s",login.c_str());
	    return i->second;
	    }
	    
	}
    }
//	logErr("not found login %s",login.c_str());
    return NULL;
}
