#include <stdio.h>
#include "c_conn.h"
#include "errs.h"
#include "config.h"
#include "utils.h"
#include "misc.h"
#include "mysql_extens.h"
#include "cmds.h"
#include "mat.h"
#include <signal.h>
#include "_mutex.h"
#define CONFIG_FILE "mat.conf"

cMYSQL *dbh;
//_mutex sock_MX;

string dump_mutex_inspectors ();

map<string,user*> users;

map<int,vector<string> > greetingmsgs;
map<int,vector<string> > privmsgs;
map<int,vector<string> > msgs;


string rndvec(const vector<string> &v)
{
	if(!v.size()){
		logErr("--- Error: rndvec() - index out of range");
		return "";
	}
	int s=rand()%v.size();
//	printf("s=%d sz=%d\n",s,v.size());
	if(s>=v.size()) s=0;
	return v[s];
}

void read_messages()
{
	privmsgs[0]=dbh->select_1_column("select answer from mat_privmsg where gender=0");
	privmsgs[1]=dbh->select_1_column("select answer from mat_privmsg where gender=1");
	privmsgs[2]=dbh->select_1_column("select answer from mat_privmsg where gender=2");
	msgs[0]=dbh->select_1_column("select answer from mat_msg where gender=0");
	msgs[1]=dbh->select_1_column("select answer from mat_msg where gender=1");
	msgs[2]=dbh->select_1_column("select answer from mat_msg where gender=2");
	greetingmsgs[0]=dbh->select_1_column("select answer from mat_greetingmsg where gender=0");
	greetingmsgs[1]=dbh->select_1_column("select answer from mat_greetingmsg where gender=1");
	greetingmsgs[2]=dbh->select_1_column("select answer from mat_greetingmsg where gender=2");
}

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
	printf("BOT MAT v1.1.0 (C) www.belialov.com\n");
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
	logErr("Bot started.");
	
	dbh=std_new_cMYSQL();
	
	bool logined=false;
	bool joined=false;
	
	read_messages();
	
	while(1){
//	if(greeting_sent) logErr("Connection lost. Reconnecting...");
	logErr("Connecting...");
	try{
		connect_to_server();
		while(1){
//		printf("qc=%d\n",questions_count);
			{
				//MUTEX_LOCK aaa(sock_MX);
				if(!sock.connected()) break;
			}
			if(!logined){
				//MUTEX_LOCK aaa(sock_MX);
				sock.response("USER bot bot bot: Botter");
				sleep(1);
				sock.response("NICK "+params["NICKNAME"]);
				sleep(1);
				sock.response("PASS "+params["PASSWD"]);
				logined=true;
			}
			bool need_sleep=false;
			{
				//MUTEX_LOCK aaa(sock_MX);
				if(!sock.readable(0)) need_sleep=true;
			}
			if(need_sleep){
				usleep(10000);
				continue;
			}
			if(!joined){
				//MUTEX_LOCK aaa(sock_MX);
				sock.response("JOIN "+params["CHANNEL"]);
				joined=true;
			}
			string data;
			{
				//MUTEX_LOCK aaa(sock_MX);
				data=sock.gets();
			}
//#ifdef DEBUG
			logErr("|>> %s\n",data.c_str());
//#endif			
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
					//MUTEX_LOCK aaa(sock_MX);
					sock.response("PONG "+h);
					continue;
				}
			}
			c_command cmd=parse_string(data);
			process_command(cmd);
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

