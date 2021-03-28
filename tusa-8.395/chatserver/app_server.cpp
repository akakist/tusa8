#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <list>
#include "_mutex.h"
#include "mutex_inspector.h"
#include <stdio.h>
#include "web_server.h"
#include "errs.h"
#include <signal.h>
#include "st_stuff.h"
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "_mutex.h"
#include <time.h>
#include "mysql_extens.h"
#include "cutils.h"
#include <algorithm>
#include "user.h"
#include "message.h"
#include "app_con.h"
#include "web_server_data.h"
#include "file_set.h"
#include "tcp_stats.h"
#include "server_config.h"
#include "app_server.h"
#include "app_def.h"
#include "tcp_sender.h"
#include "web_tools.h"
#include "version.h"
#include "user_credits.h"
#include "chat_def.h"
#include "irc_server_config.h"

#include "chat_config.h"
#include "notes.h"
#include "channels.h"
#include "irc_utils.h"
#include "update_cmd.h"
#include "ignore_vec.h"
#include "user_profile.h"
#include "contact_vec.h"
#include "chat_globals.h"
#include "admin.h"
#include "chat_thread_event.h"
#include "global_state.h"
#include "copy_file.h"
#define NO_USERS_COUNT

#define	IRC_TIMEOUT	60

void update_db_cache();
void LK_get_users();

void load_app_configuration();
extern irc_server *ircserver;
void irc_iterate();
void local_users_iterate();
void send_system_note(const DB_id& uid,const string &subj,const string& msg);

extern m_var<string> __APPNAME;
struct ext_events
{
    static void inc_credit_summa(map<string,string> &w,const string& txt,const string& d1,const string& d2);
    static void flush(map<string,string> &w,const string& txt,const string& d1,const string& d2);
    static void chat_exit(map<string,string> &w,const string& txt,const string& d1,const string& d2);
    static void systemnote(map<string,string> &w,const string& txt,const string& d1,const string& d2);
    static void sysmsg(map<string,string> &w,const string& txt,const string& d1,const string& d2);
    static void set_topic(map<string,string> &w,const string& txt,const string& d1,const string& d2);
    static void kick_user(map<string,string> &w,const string& txt,const string& d1,const string& d2);
    static void ban_ip_common(map<string,string> &w,const string& txt,const string& d1,const string& d2);
    static void ban_login_common(map<string,string> &w,const string& txt,const string& d1,const string& d2);
    static void ban_nick(map<string,string> &w,const string& txt,const string& d1,const string& d2);
    static void klap_channel(map<string,string> &w,const string& txt,const string& d1,const string& d2);

};

string add_klap$$(const klap_item& item);
string get_klap_uname(const DB_id&uid);
int get_klap_ulevel(const DB_id&uid);




void ext_events::klap_channel(map<string,string> &w,const string& txt,const string& d1,const string& d2)
{
    if (!w.count("uid") || !w.count("adm_uid")||  !w.count("channel")||  !w.count("time"))
    {
        logErr2("invalid cmd %s (%s %d)",txt.c_str(),__FILE__,__LINE__);
        return;
    }
    klap_item bi;
    bi.id=get_next_klap_id();

    CONTAINER(bi.adm_uid)=atoi(w["adm_uid"]);
    CONTAINER(bi.channel)=atoi(w["channel"]);
    bi.setcurtime(atoi(w["time"]));
    CONTAINER(bi.uid)=atoi(w["uid"]);;


    bi.descr=d1;
    bi.alevel=get_klap_ulevel(bi.adm_uid);
    bi.ulevel=get_klap_ulevel(bi.uid);
    bi.adm_name=get_klap_uname(bi.adm_uid);
    bi.user_name=get_klap_uname(bi.uid);

    add_klap$$(bi);


}

int do_nickban(const string &nick_orig,const DB_id& adm_uid,unsigned int adm_level);

void ext_events::ban_nick(map<string,string> &w,const string& txt,const string& d1,const string& d2)
{
    if (!w.count("nick") || !w.count("adm_uid"))
    {
        logErr2("invalid cmd %s (%s %d)",txt.c_str(),__FILE__,__LINE__);
        return;
    }
    DB_id adm_uid;
    CONTAINER(adm_uid)=atoi(w["adm_uid"]);
    int al;
    {
        PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(adm_uid)));
        al=u->level;
    }
    do_nickban(w["nick"],adm_uid,al/sub_levels);
}
int do_old_ipban(const string &ip_orig,time_t t,const string& za,const DB_id& adm_uid);
int do_old_loginban(const DB_id &zuid,const string &descr,time_t t,const DB_id& adm_uid,unsigned int adm_level);

void ext_events::ban_login_common(map<string,string> &w,const string& txt,const string& d1,const string& d2)
{
    if (!w.count("uid") || !w.count("time") || !w.count("adm_uid"))
    {

        logErr2("invalid cmd %s (%s %d)",txt.c_str(),__FILE__,__LINE__);
        return;
    }
    DB_id uid;
    CONTAINER(uid)=atoi(w["uid"]);
    DB_id adm_uid;
    CONTAINER(adm_uid)=atoi(w["adm_uid"]);
    int al;
    {
        PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(adm_uid)));
        al=u->level;
    }
    do_old_loginban(uid,d1,atoi(w["time"]),adm_uid,al);
}

void ext_events::ban_ip_common(map<string,string> &w,const string& txt,const string& d1,const string& d2)
{
    if (!w.count("ip") || !w.count("time") || !w.count("adm_uid")|| !w.count("za"))
    {
        logErr2("invalid cmd %s (%s %d)",txt.c_str(),__FILE__,__LINE__);
        return;
    }
    DB_id auid;
    CONTAINER(auid)=atoi(w["adm_uid"]);
    do_old_ipban(w["ip"],atoi(w["time"]),w["za"],auid);

}

void ext_events::kick_user(map<string,string> &w,const string& txt,const string& d1,const string& d2)
{
    if (!w.count("uid"))
    {
        logErr2("invalid cmd %s (%s %d)",txt.c_str(),__FILE__,__LINE__);
        return;
    }
    DB_id uid;
    CONTAINER(uid)=atoi(w["uid"]);
    ::kick_user(uid,d1);

}

void ext_events::inc_credit_summa(map<string,string> &w,const string& txt,const string& d1,const string& d2)
{
    if (!w.count("uid") || !w.count("summa"))
    {
        logErr2("invalid cmd %s (%s %d)",txt.c_str(),__FILE__,__LINE__);
        return;
    }

    DB_id uid;
    CONTAINER(uid)=atoi(w["uid"]);
    Double summa;
    summa.container=atof(w["summa"].c_str());
    string opcode=d1;
    string msg=d2;
    ::inc_credit_summa(uid,summa,opcode);
    send_system_note(uid,"начисленo "+w["summa"]+" кредит(ов)",msg);
    send_syspriv_msg(uid,"начисленo "+w["summa"]+" кредит(ов). "+opcode);

}
void flush_user_data(const DB_id&uid)
{
    chat_event_FLUSH_USER_DATA *e=new chat_event_FLUSH_USER_DATA (uid);
    send_chat_event_push_back(e);
    user$profiles.clear_key(CONTAINER(uid));
    nick_vecs.clear_key(CONTAINER(uid));
    contact_vecs.clear_key(CONTAINER(uid));
    ignore_vecs.clear_key(CONTAINER(uid));

}
void ext_events::flush(map<string,string> &w,const string& txt,const string& d1,const string& d2)
{
    if (!w.count("uid") || !w.count("obj"))
    {
        logErr2("invalid cmd %s (%s %d)",txt.c_str(),__FILE__,__LINE__);
        return;
    }

    if (w["obj"]=="tbl_users")
    {
        DB_id uid;
        CONTAINER(uid)=atoi(w["uid"]);
        user$profiles.clear_key(CONTAINER(uid));
    }
    else if (w["obj"]=="nick_vecs")
    {
        DB_id uid;
        CONTAINER(uid)=atoi(w["uid"]);
        nick_vecs.clear_key(CONTAINER(uid));
    }
    else if (w["obj"]=="contact_vecs")
    {
        DB_id uid;
        CONTAINER(uid)=atoi(w["uid"]);
        contact_vecs.clear_key(CONTAINER(uid));
    }
    else if (w["obj"]=="ignore_vecs")
    {
        DB_id uid;
        CONTAINER(uid)=atoi(w["uid"]);
        ignore_vecs.clear_key(CONTAINER(uid));
    }
    else
        logErr2("invalid command format %s %s %d",txt.c_str(),__FILE__,__LINE__);



}
void ext_events::chat_exit(map<string,string> &w,const string& txt,const string& d1,const string& d2)
{
    if (!w.count("uid"))
    {
        logErr2("invalid cmd %s (%s %d)",txt.c_str(),__FILE__,__LINE__);
        return;
    }
    DB_id uid;
    CONTAINER(uid)=atoi(w["uid"]);
    logErr2("us_PART_CHANNEL(T_WWW,%d) %s %d",CONTAINER(uid),__FILE__,__LINE__);
    us_PART_CHANNEL(T_WWW,uid);
}
void ext_events::systemnote(map<string,string> &w,const string& txt,const string& d1,const string& d2)
{
    if (!w.count("uid"))
    {
        logErr2("invalid cmd %s (%s %d)",txt.c_str(),__FILE__,__LINE__);
        return;
    }
    DB_id uid;
    CONTAINER(uid)=atoi(w["uid"]);
    send_system_note(uid,d1,d2);
}
void ext_events::sysmsg(map<string,string> &w,const string& txt,const string& d1,const string& d2)
{
    if (!w.count("uid"))
    {
        logErr2("invalid cmd %s (%s %d)",txt.c_str(),__FILE__,__LINE__);
        return;
    }
    DB_id uid;
    CONTAINER(uid)=atoi(w["uid"]);
    send_syspriv_msg(uid,d1);
}
void ext_events::set_topic(map<string,string> &w,const string& txt,const string& d1,const string& d2)
{
    if (!w.count("uid")||!w.count("channel"))
    {
        logErr2("invalid cmd %s (%s %d)",txt.c_str(),__FILE__,__LINE__);
        return;
    }
    st_update stu;
    DB_id uid;
    CONTAINER(uid)=atoi(w["uid"]);
    PTR_getter<chat_user>__UU=local_users.get$(uid,T_WWW);
    if (!__UU)
    {
        logErr2("set_topic: user not logged %d",CONTAINER(uid));
        return;
    }
    user_pair __U(__UU);

    if (!user__privileges(__U,"$topic")&& user$level(__U)/sub_levels<am_vip)
    {
        logErr2("set_topic: not enough privileges");
        return;
    }


    CH_id channel;
    CONTAINER(channel)=atoi(w["channel"]);
    if (!chat_channels.count(channel))
    {
        logErr2("set_topic: invalid channel %d",CONTAINER(channel));
        return;
    }
    string top=d1;
    if (top.size()>128) top=top.substr(0,128);
    top=str_replace("'","",top);
    top=str_replace("\\\\","",top);
    string cnick;
    {
        PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(uid)));
        cnick=u->last_nick;
    }
    __send_update_d5(channel$topic$5,channel,top,cnick,time(NULL));
    chat_event_CHTOPIC *e=new chat_event_CHTOPIC(channel,uid,top);
    LEAKCTL_ADD(e);
    send_chat_event_push_back(e);

}

void iterate_ext_events();
void iterate_ext_events()
{
    st_dbh d;
    vector<vector<string> > v=d.dbh->exec("select id, event,d1,d2 from ext_events");
    for (unsigned i=0; i<v.size(); i++)
    {
        try
        {

            if (v[i].size()==4)
            {
                string d1=v[i][2];
                string d2=v[i][3];

                string txt=v[i][1];

                vector<string> w=splitTOK(" \r\n",txt);
                map<string,string> mcmd;
                for (unsigned i=0; i<w.size(); i++)
                {
                    string &s=w[i];
                    size_t pz=s.find("=",0);
                    if (pz==-1) {
                        logErr2("--Error: iterate_ext_events syntax error in  %s",s.c_str());
                        continue;
                    }
                    string key=s.substr(0,pz);
                    string val=s.substr(pz+1,s.size()-pz-1);
                    mcmd[key]=val;

                }
                if (w.size()!=0)
                {
                    string cmd=mcmd["cmd"];
                    if (cmd=="inc_credit_summa")ext_events::inc_credit_summa(mcmd,txt,d1,d2);
                    else if (cmd=="flush")		ext_events::flush(mcmd,txt,d1,d2);
                    else if (cmd=="chat_exit")	ext_events::chat_exit(mcmd,txt,d1,d2);
                    else if (cmd=="systemnote")	ext_events::systemnote(mcmd,txt,d1,d2);
                    else if (cmd=="sysmsg")		ext_events::sysmsg(mcmd,txt,d1,d2);
                    else if (cmd=="set_topic")	ext_events::set_topic(mcmd,txt,d1,d2);
                    else if (cmd=="kick_user")	ext_events::kick_user(mcmd,txt,d1,d2);
                    else if (cmd=="ban_ip_common")	ext_events::ban_ip_common(mcmd,txt,d1,d2);
                    else if (cmd=="ban_login_common")	ext_events::ban_login_common(mcmd,txt,d1,d2);
                    else if (cmd=="ban_nick")	ext_events::ban_nick(mcmd,txt,d1,d2);
                    else if (cmd=="klap_channel")	ext_events::klap_channel(mcmd,txt,d1,d2);
                    else logErr2("invalid cmd %s",w[0].c_str());



                }
                else
                    logErr2("invalid command format %s %s %d",txt.c_str(),__FILE__,__LINE__);

            }
            else
                logErr2("if(v[i].size()!=4) %s %d",__FILE__,__LINE__);
        }
        STDCATCHS("ext_events");
        d.dbh->real_queryQ((QUERY)"delete from ext_events where id=?"<<atoi(v[i][0]));


    }
}


app_server::~app_server()
{

}
void app_server::start(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m)
{

    web_server::start(pn,prefix,m);
    load_config(pn,prefix,m);
    srand(time(NULL));
    SERVERNAME.set(SERVER_VERSION);
    {
        create_thread(users_handler,"users_handler");
    }

}
app_server::app_server()
{
}
void *app_server::new_connection(const PTR_getter<c_socket>&s)
{
    app_connection*a =new app_connection(this,s);
    LEAKCTL_ADD(a);
    return a;
}

class ignore_vec;
class contact_vec;


static M_MAP<time_t,int> users$counts;
static M_MAP<time_t,int> local_users$counts;
void local_free_handler__1()
{
    logErr2("free %s %d",__FILE__,__LINE__);
    users$counts.clear();
    local_users$counts.clear();
}
static m_var<time_t> last_update_time_users$counts(0);
time_t total_wtime();
static m_var<int> last_wtime(0);
#ifdef WIN32
static m_var<__int64> last_tcp_total_accepted_connections(0);
static m_var<__int64> last_tcp_total_recv(0);
static m_var<__int64> last_tcp_total_send(0);
#else
static m_var<long long> last_tcp_total_accepted_connections(0);
static m_var<long long> last_tcp_total_recv(0);
static m_var<long long> last_tcp_total_send(0);
#endif
void cpu_state()
{
    XTRY;
    int _DT,_MIU,_MLU;
    double _WT,_WTDT;
#ifdef WIN32
    __int64
#else
    long long
#endif
    dtcp_recv=tcp_total_recv-last_tcp_total_recv;
    last_tcp_total_recv=tcp_total_recv.get();
    unsigned int _TCPRECV=dtcp_recv;

#ifdef WIN32
    __int64
#else
    long long
#endif
    dtcp_send=tcp_total_send-last_tcp_total_send;
    last_tcp_total_send=tcp_total_send.get();
    unsigned int _TCPSEND=dtcp_send;

#ifdef WIN32
    __int64
#else
    long long
#endif
    dtcp_total_accepted_connections=tcp_total_accepted_connections-last_tcp_total_accepted_connections;
    last_tcp_total_accepted_connections=tcp_total_accepted_connections.get();
    unsigned int _TCPACCONN=dtcp_total_accepted_connections;



    time_t dt=time(NULL)-last_update_time_users$counts.get();
    if (dt==0) return ;
    int _TCPRECVs;
    _TCPRECVs=_TCPRECV/dt;
    int _TCPSENDs;
    _TCPSENDs=_TCPSEND/dt;
    int _TCPACCONNs;
    _TCPACCONNs=_TCPACCONN/dt;

    time_t twt=total_wtime();
    int dtwt=twt-last_wtime.get();
    if (dtwt==0) return ;
    last_wtime=twt;
    double nnn=dtwt;
    double DTWT=dtwt;
    DTWT/=100;
    nnn/=100;
    {
        if (dt!=0)
            nnn/=dt;
        else nnn=0;
        nnn*=100;
        _DT=dt;
        _WT=DTWT;
        _WTDT=nnn;
    }
    {
        int mu=0;
        map<time_t,int> mm=local_users$counts.get_container();
        local_users$counts.clear();
        for (map<time_t,int>::const_iterator i=mm.begin(); i!=mm.end(); i++)
        {
            if (i->second>mu)mu=i->second;
        }
        _MLU=mu;
    }
    {
        int mu=0;
        map<time_t,int> mm=users$counts.get_container();
        users$counts.clear();
        for (map<time_t,int>::const_iterator i=mm.begin(); i!=mm.end(); i++)
        {
            if (i->second>mu)mu=i->second;
        }
        time_t dt=time(NULL);
        dt-=dt%600;
        _MIU=mu;
    }
    logErr2("CPU_USAGE DT=%d WT=%2.2f WT/DT=%2.2f%  MAX_LOCAL_USERS=%d TCPRECV=%d,%d/s TCPSEND=%d,%d/s TCPACCONN=%d,%d/s ",_DT,_WT,_WTDT,_MLU,_TCPRECV,_TCPRECVs,_TCPSEND,_TCPSENDs,_TCPACCONN,_TCPACCONNs);
    XPASS;
}



void update_user_counts()
{
    MUTEX_INSPECTOR;

    XTRY;
    st_dbh d;

    {
        int mu=0;
        map<time_t,int> mm=users$counts.get_container();
        users$counts.clear();
        for (map<time_t,int>::const_iterator i=mm.begin(); i!=mm.end(); i++)
        {
            if (i->second>mu)mu=i->second;
        }
        time_t dt=time(NULL);
        dt-=dt%600;
        try {
            d.dbh->real_queryQ((QUERY)"insert into users_count (dt,cnt) values(FROM_UNIXTIME(?),?)"<<dt<<mu);
        }
        STDCATCHS("update_user_counts");
    }
    XPASS;
}
static void users_handler__update_users$counts()
{
    MUTEX_INSPECTOR;
    XTRY;
    if (time(NULL)-last_update_time_users$counts.get()>600)
    {
        XTRY;
        try {
            MUTEX_INSPECTOR;
            XTRY;
            update_user_counts();

            cpu_state();
            XPASS;

        }
        STDCATCHS("users_handler__update_users$counts");

        last_update_time_users$counts=time(NULL);
        XPASS;
    }
    XPASS;

}



void* app_server::users_handler(void *pr)
{
    XTRY;
    app_server*srv=(app_server*)pr;

    string thread_name=srv->server_name()+" app_server::users_handler";
    log_start_thread(thread_name);

    unsigned int n=61;
    unsigned int cs=600;
    unsigned int CS2=60;
    unsigned int avg_cnt=0;
    struct tm New_tm;
    int last_min;
    New_tm=LOCALTIME(time(NULL));
    last_min=New_tm.tm_min;
    bool started=false;
    while (1) {
        {
            if (g_must_terminate) return NULL;
            sleep(1);
        }


        try {
            MUTEX_INSPECTOR;

            {
                MUTEX_INSPECTOR;
                XTRY;
                irc_iterate();
                XPASS;
            }





            chat_channels_backup_messages();
            if (cs>=600) {	//calc statistic & save messages
                cs=0;
            }

            if (CS2>20)
            {
                MUTEX_INSPECTOR;
                XTRY;
                {
                    MUTEX_INSPECTOR;
                    local_users_iterate();
                }
                CS2=0;
                {
                    MUTEX_INSPECTOR;
                    update_db_cache();
                }

                XPASS;
            }

            {
                MUTEX_INSPECTOR;

                int pc=local_users.size$();
                users$counts.set(time(NULL),pc);
                local_users$counts.set(time(NULL),global_state.get_users$count());
                {
                    MUTEX_INSPECTOR;
                    users_handler__update_users$counts();
                }
                try {
                    MUTEX_INSPECTOR;
                    iterate_ext_events();
                } catch (...) {}
                n++;
                cs++;
                CS2++;
            }
        }
        STDCATCHS("users_handler");
    }
    XPASS;

}
void app_server::load_config(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m)
{

}


/*time_t app_server::get_socket_inactive_kill_timeout()
{
    return 5000;
}*/
