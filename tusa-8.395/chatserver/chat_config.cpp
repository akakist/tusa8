#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <signal.h>
#include <errno.h>
#include "file_set.h"
#include "mutex_inspector.h"
#include "oscar_buffer.h"
#include "chat_config.h"
#include "server_config.h"

#include "str_lib.h"
#include "message.h"

#include "message.h"
#include "cm_container.h"
#include "time_limit.h"

string str_nick(const string &n);
void load_bans();

bool load_channels$$$();

void load_color_packs(const string &scpack,const string& conf_name);
bool load_messages();
bool load_levels(const string &pn);
vector< vector<string> > load_st_file(const string &pn);
vector<string> load_l_file(const string &fn);

vector< vector<string> > load_st_file(const string &fname)
{
    vector< vector<string> > v;
    for (int i=0; i<3; i++) {
        string fn=fname+"."+itostring(i)+".msg";
        v.push_back(load_l_file(fn));
    }
    return v;
}


__chat_config chat_config;

__chat_config::__chat_config()
{
}

void __chat_config::load_config(const string&pn)
{
    MUTEX_INSPECTOR;
    map<string,string>	m=load_m_file(pn);
    set_cfg_str(m,"to_all", pn,to_all,"ВСЕМ");
    set_cfg_str(m,"php_session_prefix", pn,php_session_prefix,"/tmp/sess_");

#ifdef WIN32
    set_cfg_str(m,"time_format_win32", pn,time_format,"%k:%M");
#else

    set_cfg_str(m,"time_format", pn,time_format,"%k:%M");
#endif
    set_cfg_i(m,"chat_offline_timeout_sec", pn,chat_offline_timeout_sec,300);
    set_cfg_i(m,"data_drop_timeout_sec", pn,data_drop_timeout_sec,1000);
    set_cfg_i(m,"user_max_inactive_time_sec", pn,user_max_inactive_time_sec,1);
    set_cfg_i(m,"iframe_buffer_max_size", pn,iframe_buffer_max_size,100000);
    set_cfg_i(m,"max_msg_per_period", pn,max_msg_per_period,20);
    set_cfg_i(m,"max_msg_period_sec", pn,max_msg_period_sec,20);


    load_messages();
    load_levels(get_cfg_str(m,"levels_config",pn, "./conf/levels.conf"));

    msgs_greeting.assign_ck(load_st_file(get_cfg_str(m,		"greeting",pn, "./www/messages/greetings")));
    msgs_bye.assign_ck(load_st_file( get_cfg_str(m,		"bye",pn, "./www/messages/byes")));
    msgs_kicking.assign_ck(load_st_file(get_cfg_str(m,		"kicking",pn, "./www/messages/kickings")));
    msgs_killing.assign_ck(load_st_file(get_cfg_str(m,		"killing",pn, "./www/messages/killing")));
    msgs_changes.assign_ck(load_st_file(get_cfg_str(m,		"changes",pn, "./www/messages/channels")));
    msgs_nickchanges.assign_ck(load_st_file(get_cfg_str(m,	"nickchanges",pn, "./www/messages/nickchanges")));
    msgs_topicchanges.assign_ck(load_st_file(get_cfg_str(m,	"topicchanges",pn, "./www/messages/topicchanges")));
    msgs_modechanges.assign_ck(load_st_file(get_cfg_str(m,	"modechanges",pn, "./www/messages/modechanges")));
    msgs_maty.assign_ck(load_st_file(get_cfg_str(m,		"censmess",pn, "./www/messages/censored")));

    msgs_defstatus.assign(load_st_file(get_cfg_str(m,		"status",pn, "./www/messages/status")));
    vector<string> v=load_l_file(get_cfg_str(m,			"bad_words",pn, "./www/messages/bad_words")+".msg");
    if (v.size()>0) {
        string b=v[0];
        for (unsigned int i=1; i<v.size(); i++) {
            if (v[i]!="") {
                b+="|"+v[i];
            }
        }
        bad_words=str_nick(b);
    }
    load_color_packs(get_cfg_str(m,"color_packs", pn,"0:./conf/colors/std.conf,1:./conf/colors/1blue.conf,2:./conf/colors/2green.conf,3:./conf/colors/3red.conf,4:./conf/colors/4pink.conf"),pn);

    load_bans();
    load_channels$$$();
    {

        set_cfg_i(m,"change_nick_timeout_sec", pn,change_nick_timeout_sec,2);
        set_cfg_i(m,"system_note_uid", pn,system_note_uid,26);

    }

    print_rest_cfg(m,pn);
}

