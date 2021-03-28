#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <list>
#include "mutex_inspector.h"
#include <time.h>
#include <map>
#include "mysql_extens.h"
#include "user.h"
#include "cutils.h"
#include "str_lib.h"
#include "chat_def.h"


#include "channels.h"
#include "server_config.h"
#include "user.h"
#include "app_def.h"
#include "user_profile.h"

#include "app_con.h"
#include "web_tools.h"
#include "update_cmd.h"

#ifdef __WITH_LICENSE
#include "license.h"
#endif



string log_name(const string& prefix);

void url_admin_logs(const PTR_getter<chat_user> &__UU,app_connection *cc, const char * logname$)
{
    LICENSE_CK(16);

    MUTEX_INSPECTOR;
    if (!__UU) {
        cc->html_out=cc->redirect();
        return;
    }
    user_pair __U(__UU);

    if (!(user__privileges(__U,"$logs")) ) {
        return;
    }
    int levelsub_levels=user$level(__U)/sub_levels;

    LICENSE_CK(13);
    string logname = log_name(logname$);


    st_FILE f(logname,"r");
    deque<string> v;
    try {
        MUTEX_INSPECTOR;

        while (1) {
            st_malloc buf(700);
            f.gets((char*)buf.buf,690);
            string z((char*) buf.buf,strlen((char*) buf.buf));
            v.push_front(z);
        }
    }
    STDCATCHS("admin");

    map<string,string> K;
    cc->html_out=cc->rvfiles(K,"admin/logs/logs");
    for (size_t i=0; i<v.size(); i++) {
        MUTEX_INSPECTOR;

        string q=remove_tags(v[i]);
        if (q.find("logged in",0)!=-1) {
            cc->html_out+="<font class=lg>"+q+"</font><br>";
            continue;
        }
        if (q.find("--Error",0)!=-1) {
            MUTEX_INSPECTOR;

            if (levelsub_levels==am_sadmin) cc->html_out+="<font class=er>"+q+"</font><br>";
            continue;
        }
        if (q.find("kicked",0)!=-1) {
            MUTEX_INSPECTOR;

            cc->html_out+="<font class=kick>"+q+"</font><br>";
            continue;
        }
        if (q.find("banned",0)!=-1) {
            MUTEX_INSPECTOR;

            cc->html_out+="<font class=ban>"+q+"</font><br>";
            continue;
        }
        if (levelsub_levels!=am_sadmin) continue;
        cc->html_out+=remove_tags(q)+"<br>";
    }
}






void url_admin_settings(const PTR_getter<chat_user> &__U,app_connection*cc);
void url_admin_privs(const PTR_getter<chat_user> &__U,app_connection*cc);

void url_admin_credgen(const PTR_getter<chat_user> &__U,app_connection*cc);
void url_admin_ipban(const PTR_getter<chat_user> &__UU,app_connection*cc);
void url_admin_loginban(const PTR_getter<chat_user> &__UU,app_connection*cc);
void url_admin_nickban(const PTR_getter<chat_user> &__UU,app_connection*cc);
void url_admin_channels(const PTR_getter<chat_user> &__UU,app_connection*cc);
void url_admin_ban(const PTR_getter<chat_user> &__UU,app_connection *cc);
void url_admin_moders(const PTR_getter<chat_user> &__UU,app_connection *cc);
void url_admin_ban_uid(const PTR_getter<chat_user> &__UU,app_connection *cc);
void url_admin_old_loginban(const PTR_getter<chat_user> &__UU,app_connection *cc);
void url_admin_old_nickban(const PTR_getter<chat_user> &__UU,app_connection *cc);
void url_admin_old_ipban(const PTR_getter<chat_user> &__UU,app_connection *cc);

string pread(const string &pn)
{
    string svn_out;
#ifndef _WIN32
    FILE *f=popen(pn.c_str(),"r");
    if (f)
    {
        MUTEX_INSPECTOR;
        while (!feof(f))
        {
            char s[1000];
            if (fgets(s,1000,f))
            {
                svn_out+=s;
            }
        }
        fclose(f);
    }
#endif
    return svn_out;

}
void url_admin_reload(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    if (!__UU) {
        cc->html_out=cc->redirect();
        return;
    }
    user_pair __U(__UU);
    if (cc->vurl.size())
    {
        if (cc->vurl[0]=="mafia")
        {
            if (user__privileges(__U,"$bot_reload"))
                cc->html_out=pread("./__reload_mafia");
            return;
        }
        if (cc->vurl[0]=="girl")
        {
            if (user__privileges(__U,"$bot_reload"))
                cc->html_out=pread("./__reload_girl");
            return;
        }
        if (cc->vurl[0]=="mat")
        {
            if (user__privileges(__U,"$bot_reload"))
                cc->html_out=pread("./__reload_mat");
            return;
        }
        if (cc->vurl[0]=="quiz")
        {
            if (user__privileges(__U,"$bot_reload"))
                cc->html_out=pread("./__reload_quiz");
            return;
        }
        if (cc->vurl[0]=="roulette")
        {
            if (user__privileges(__U,"$bot_reload"))
                cc->html_out=pread("./__reload_roulette");
            return;
        }
        if (cc->vurl[0]=="apache")
        {
            if (user__privileges(__U,"$apache_reload"))
                cc->html_out=pread("./__reload_apache");
            return;
        }
    }
    cc->html_out="Not enough privileges";
}

void url_admin(const PTR_getter<chat_user> &__U,app_connection* cc)
{
    MUTEX_INSPECTOR;
    LICENSE_CK(17);
    if (!__U) {
        cc->html_out=cc->redirect();
        return;
    }
    user_pair __UU(__U);
    switch_cc_ret("ban.tse",url_admin_ban(__U,cc));
    switch_cc_ret("ban_uid.tse",url_admin_ban_uid(__U,cc));

    switch_cc_ret("logs.tse",url_admin_logs(__U,cc,"users"));
    switch_cc_ret("logcredit.tse",url_admin_logs(__U,cc,"credit"));
    switch_cc_ret("logcommon.tse",url_admin_logs(__U,cc,"log"));
    switch_cc_ret("settings.tse",url_admin_settings(__U,cc));
    switch_cc_ret("privileges.tse",url_admin_privs(__U,cc));
    switch_cc_ret("moders.tse",url_admin_moders(__U,cc));
    switch_cc_ret("reload.tse",url_admin_reload(__U,cc));
    switch_cc_ret("channels.tse",url_admin_channels(__U,cc));

    if (user__privileges(__UU,"$oldban"))
    {
        switch_cc_ret("loginban.tse",url_admin_old_loginban(__U,cc));
        switch_cc_ret("nickban.tse",url_admin_old_nickban(__U,cc));
        switch_cc_ret("ipban.tse",url_admin_old_ipban(__U,cc));
    }
}
