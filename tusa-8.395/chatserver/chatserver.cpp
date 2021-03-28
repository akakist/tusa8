#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <signal.h>
#include <errno.h>
#include "file_set.h"
#include "PTR.h"
#include "mutex_inspector.h"
#include "chat_def.h"
#include "web_server_data.h"
#include "mysql_extens.h"
#include "user.h"
#include "cutils.h"
#include "PTR.h"
#include "tcp_server_data.h"
#include "app_server.h"
#include "app_def.h"
#include "version.h"
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include "irc_server.h"

#include "irc_server_config.h"
#ifndef _NO_PROXY
#include "pgate_server.h"
#include "pgate_server_config.h"
#endif

#include <signal.h>
#include <errno.h>
#include "file_set.h"
#include "PTR.h"
#include "mutex_inspector.h"
#include "chat_def.h"
#include "web_server_data.h"
#include "mysql_extens.h"
#include "user.h"
#include "cutils.h"
#include "PTR.h"
#include "tcp_server_data.h"
#include "app_server.h"
#include "app_def.h"
#include "version.h"
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include "irc_server.h"

#include "irc_server_config.h"
#ifndef _NO_PROXY
#include "pgate_server.h"
#include "pgate_server_config.h"
#endif


#include "contact_vec.h"
#include "ignore_vec.h"
#include "channels.h"
#include "notes.h"
#include "user_credits.h"
#include "chat_thread_event.h"
#include "chat_config.h"
#include "global_state.h"
#include "admin.h"
#include "server_config.h"
#include "db_config.h"

#include "chat_stuff.h"
#include "tcp_sender.h"
app_server* wwwserver;

irc_server *ircserver;
m_var<bool> ex(false);

void onterm(int signum);

void kaak1();


void	load_app_configuration();
void load_license_rsa_pub_key();
void load_app_configuration();
void start_update_processor();


void start_chat_processor();
int app_main(int argc,char *argv[])
{
    MUTEX_INSPECTOR;

    try {
        printf("%s, build %s\n\n",SERVER_VERSION,__DATE__);
#ifndef WIN32

        if (argc>1) {
            if (strcmp(argv[1],"-d")==0) {
                printf("Continuing as daemon\n");
                if (fork()) exit(1);
            }
            if (strcmp(argv[1],"--help")==0) {
                printf("\t -d - to run as deamon\n");
                exit(1);
            }
        }
#endif




        srand(time(NULL));


#ifndef WIN32
        if (argc>1 && (string)argv[1]==(string)"-b") {
            int ppd=fork();
            if (ppd!=0) {
                exit(0);
            }
        }
#endif

#ifndef WIN32
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

        signal(10,onterm);
#endif

        string pn="./conf/CS.conf";
        string conf="./conf/CS.conf";

        map<string,string> m=load_m_file(pn);

        db_config.load_config(get_cfg_str(m,"db_config",pn, "./conf/db.conf"));
        {
            MUTEX_INSPECTOR
            load_app_configuration();
        }
        init_senders(conf,"sender_",m);
        {
            XTRY;
            MUTEX_INSPECTOR
            logErr2("Starting webserver");
            wwwserver=new app_server();
            LEAKCTL_ADD(wwwserver);
            wwwserver->start(conf,"www_server_",m);
            XPASS;
        }


        chat_processor * processor=new chat_processor;
        processor->start(conf,"chat_processor_",m);
        start_update_processor();
        {
            MUTEX_INSPECTOR
            logErr2("Starting ircd");
            ircserver=new irc_server();
            LEAKCTL_ADD(ircserver);
            ircserver->start(conf,"irc_server_",m);
        }

        int i=0;
        while (i<10) {
            sleep(1);
//	    i++;
        }

    } catch (cError e) {
        logErr2("--Error in start_main() - %s",e.error_string.c_str());
    } catch (...) {
        logErr2("--Error in start_main() - unknow error!");
    }
    return 0;
}

void print_term(int signum)
{
    string out;
#ifdef MUTEX_INSPECTOR_DBUG
    out+=dump_mutex_inspectors();
#endif
    string fn;
    if (signum!=10) {
        fn=(string)"/tmp/TERM."+itostring(time(NULL));
    } else {
        fn=(string)"/tmp/USERTERM."+itostring(time(NULL));
    }
    if (out.size())
    {
        out="Received on SIGNAL "+itostring(signum)+"\n"+out;

        FILE *f=fopen(fn.c_str(),"wb");
        if (f) {
            fprintf(f,"%s",out.c_str());
            fclose(f);
        }
    }
    logErr2("%s",out.c_str());
}

void onterm(int signum)
{
    int n=rand();

    try {
        if (!ex) {
            string out;
            if (signum==1&&wwwserver) {
                ex=true;
                print_term(signum);
                load_app_configuration();
                sleep(10);
                ex=false;
                return;

            }

            if (signum!=10 && signum!=1) {
                ex=true;

                logErr2("Terminating on SIGNAL %d",signum);
            }
            print_term(signum);

            if (signum!=10) {


                exit(signum);
            }
        }
    } catch (cError e) {
    } catch (...) {
        logErr2("--Error: unknow error in on_term()");
    }
}


