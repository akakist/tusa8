#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include "_mutex.h"
#include <list>
#include "mutex_inspector.h"
#include <time.h>
#include <list>
#include <stdio.h>
#include "errs.h"
#include <map>
#include "cutils.h"
#include "web_server.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "user.h"
#include "message.h"
#include "web_server.h"
#include "copy_file.h"
#include "wrappers.h"
#include "file_set.h"
#include "channels.h"
#include "levels.h"
#include "server_config.h"

#include "chat_config.h"

#include "irc_utils.h"
#include "irc_server_config.h"
#include "DB_id.h"
#include "CH_id.h"
#include "update_cmd.h"
#include "chat_msg.h"
#include "cm_container.h"
#include "M_DEQUE.h"
#include "mysql_extens.h"
#include "chat_colors.h"
#include "global_state.h"
extern M_DEQUE<pair<string,string> > messages_to_backup;
extern M_DEQUE<PTR_getter_nm<chat_msg> > external_$messages;


cm_container msgs_defstatus("defstatus");

//cm_container msgs_defstatus("defstatus");
cm_container msgs_greeting("greetings");
cm_container msgs_bye("bye");
cm_container msgs_kicking("kicking");
cm_container msgs_killing("killing");
cm_container msgs_changes("changes");
cm_container msgs_nickchanges("nick_changes");
cm_container msgs_topicchanges("topic_changes");
cm_container msgs_modechanges("mode_changes");
cm_container msgs_maty("cenmess");

void local_free_handler_message()
{
    msgs_defstatus.clear();
}
chat_channel::chat_channel():base_container("chat_channel")
{

}

chat_channel::~chat_channel()
{
}



void chat_channels_backup_messages()
{
    MUTEX_INSPECTOR;
    string fn__=(string)global_config.var_dir.get()+split_char+"dmp"+ split_char+ "URL."+date4file(time(NULL));
    deque<pair<string,string> > d=messages_to_backup.extract_all();
    string out_bin;
    string out_txt;
    for (deque<pair<string,string> >::const_iterator i=d.begin(); i!=d.end(); i++)
    {
        out_bin+=i->first;
        out_txt+=i->second;
    }
    try {
        if (out_bin.size())
        {
            string fn=fn__+".bin";
            check_path_wfn(fn.c_str());
            st_FILE f(fn,"ab");
            f.write(out_bin);
        }
        if (out_txt.size())
        {
            string fn2=fn__+".txt";
            check_path_wfn(fn2.c_str());
            st_FILE f(fn2,"ab");
            f.write(out_txt);

        }
    }
    STDCATCHS("chat_channels_backup_messages");
}

string cm_container::get(unsigned int g,unsigned int n)
{
    M_LOCK(Mutex);
    if (g >= container.size()) {
        logErr2("--Error: n>container.size() - in %s.%d",_name.get().c_str(),g);
        return "";
    }
    if (n >= container[g].size()) {
        logErr2("--Error: n>container.size() - in %s.%d - n %d",_name.get().c_str(),g,n);
        return "";
    }
    return container[g][n];
}

size_t cm_container::size()
{
    M_LOCK(Mutex);
    if (container.size())
        return container[0].size();
    return 0;
}

string cm_container::get_random(unsigned int g)
{
    unsigned int n=get_random_n(g);
    M_LOCK(Mutex);
    if (g >= container.size()) {
        logErr2("--Error: n>container.size() - in %s.%d",_name.get().c_str(),g);
        return "";
    }
    if (n >= container[g].size()) {
        logErr2("--Error: n>container.size() - in %s.%d n %d",_name.get().c_str(),g,n);
        return "";
    }
    return container[g][n];
}

unsigned int cm_container::get_random_n(unsigned int g)
{

    M_LOCK(Mutex);
    size_t k=container[g].size();
    unsigned int n=0;
    if (k)n=rand()%k;
    if (n>=container[g].size()) {
        n=0;
    }
    return n;
}

void cm_container::assign(const vector< vector<string> >  &v)
{
    M_LOCK(Mutex);
    container=v;
}
void cm_container::assign_ck(const vector< vector<string> > &v)
{
    {
        M_LOCK(Mutex);
        container=v;
    }
    check();
}

void cm_container::clear()
{
    M_LOCK(Mutex);
    container.clear();
}

void cm_container::check()
{
    M_LOCK(Mutex);
    size_t max=0;
    for (unsigned int j=0; j<container.size(); j++) {
        if (max<container[j].size()) max=container[j].size();
    }

    for (unsigned int j=0; j<container.size(); j++) {
        if (container[j].size()!=max) logErr2("--Error: size different in %s.%d: normal size=%d current size=%d",_name.get().c_str(),j,max,container[j].size());
    }

}


void send_syspriv_msg(const DB_id& uid,const string &msg)
{
    vector<PTR_getter<chat_user> > __UU=local_users.get$(uid);
    for (unsigned i=0; i<__UU.size(); i++)
    {
        if (__UU[i])
        {
            send_syspriv_msg(__UU[i],msg);
        }
    }
}

void send_syspriv_msg(const PTR_getter<chat_user>&__U,const string &msg)
{


    user_pair __UU(__U);
    send_syspriv_msg(__UU,msg);
}

void send_syspriv_msg(const user_pair&__U,const string &msg)
{
    MUTEX_INSPECTOR;
    CT ct;
    {
        PGLR(chat_user,u,__U.cu);
        ct=u->_ct;
    }
    if (ct==T_WWW)
    {
        MUTEX_INSPECTOR;
        DB_id uid=user$id(__U);
        user_state ustate=global_state.get_user_state(uid);
        set<CH_id> c=ustate.get_channel(T_WWW);
        if (c.size())
        {
            DB_id from;
            CONTAINER(from)=0;
            chat_msg_SYSPRIV *p1=new chat_msg_SYSPRIV(*c.begin(),from,uid,get_cnick(__U),msg);
            LEAKCTL_ADD(p1);
            chat_msg*p2=new chat_msg(p1);
            LEAKCTL_ADD(p2);
            PTR_getter_nm<chat_msg> e=p2;
            data_push_back_www(__U,e.___ptr->data->make(*c.begin(),uid)+SCROLL);
            external_$messages.push_back(p2);
        }
    }
    if (ct==T_IRC)
    {
        MUTEX_INSPECTOR;
        string from_nick=get_cnick(__U);
        irc_send_notice(__U,msg,from_nick);
    }
}

