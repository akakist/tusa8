#include <stdio.h>
#include "c_conn.h"
#include "errs.h"
#include "mafia.h"
#include "command.h"
#include "user.h"
#include "game.h"
#include "messages.h"
#include "config.h"
#include "utils.h"
#include "mysql_extens.h"
#include <signal.h>
#include "mutex_inspector.h"
#define CONFIG_FILE "mafia.conf"

cMYSQL *dbh;


void onterm(int signum)
{

	if(signum==1){
		load_config(CONFIG_FILE);
		logErr("Config reloaded.");
	}else{
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

            string out;
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
	
		try{
		sock.response("MODE "+params["CHANNEL"]+" -m");
		sock.disconnect();
		}
		catch(...){}
		logErr("Got signal %d, terminating...",signum);
		exit(1);
	}
}

int main(int argc,char* argv[])
{
	printf("BOT Mafia v1.1.0 (C) www.belialov.com\n");
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


	load_config(CONFIG_FILE);
	srandom(time(NULL));
	logErr("mafia started");
	
	load_messages();
	try{MUTEX_INSPECTOR;
		dbh=std_new_cMYSQL();
	}catch(cError e){MUTEX_INSPECTOR;
		logErr("---Error: %s",e.error_string.c_str());
		exit(1);
	}

	
	game_started=false;
	registration=false;
	register_counter=0;
	voiced=false;
		
	while(1){MUTEX_INSPECTOR;
	logErr("Connecting...\n");
	try{MUTEX_INSPECTOR;
	
		connect_to_server();
		bool logined=false;
		bool joined=false;
		
//		strcpy((char*)0,(char*)1000);
		while(sock.connected()){MUTEX_INSPECTOR;
			if(!logined){MUTEX_INSPECTOR;
				sock.response("USER bot bot bot: Botter");
				sleep(1);
				sock.response("NICK "+params["NICKNAME"]);
				sleep(1);
				sock.response("PASS "+params["PASSWD"]);
				logined=true;
			}
			if(!sock.readable(0)){MUTEX_INSPECTOR;
				usleep(10000);
				{MUTEX_INSPECTOR;
				process_game();
				}
				continue;
			}
			if(!joined){
				sock.response("JOIN "+params["CHANNEL"]);
				joined=true;
			}
			string data=sock.gets();
			if(data.find(":"+params["HOST"]+" 433 * :"+params["NICKNAME"]+": Nick is already in use.")!=-1){
				printf("This nickname is already in use. May be bot is already running.\n");
				logErr("This nickname is already in use. May be bot is already running.");
				sock.disconnect();
				exit(1);
			}
			//
#ifdef DEBUG
			printf("|>> %s\n",data.c_str());
#endif			
			logErr("|>> %s",data.c_str());
			//process received buffer
			
			if(data.substr(0,1)!="."){
				if(data.substr(0,4)=="PING"){
					string h=data.substr(5,data.size()-5);
					sock.response("PONG "+h);
					continue;
				}
			}
			{MUTEX_INSPECTOR;
			c_command cmd=parse_string(data);
			process_command(cmd);
			}
		}
		
	
	}catch(socketError e){
		logErr("---Socket error: %s",e.error_string.c_str());
	}catch(cError e){
		logErr("---Error: %s",e.error_string.c_str());
	}catch(...){
		logErr("---Error: unknow!");
	}
	{MUTEX_INSPECTOR;
	logErr("Connection lost...");
	sock.disconnect();
	voiced=false;
	sleep(5);
	}
	}
}

