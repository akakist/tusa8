#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif
#include <cerrno>

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
#include "chat_globals.h"
#include "web_tools.h"
#include "chat_def.h"
#include "user_profile.h"
#include "copy_file.h"
#include "chat_config.h"
#ifdef __WITH_LICENSE
#include "license.h"
void license_add_host(const string &);
#endif
#include "global_state.h"
#include "channels.h"


void  app_connection::redirect(const string &url)
{
    http_code=302;
    http_header_out["Location"]=url;
    return;
}
app_connection::app_connection(app_server *cs,const PTR_getter<c_socket>&s): web_connection(cs,s)
{
    handle_max_recv_limit=true;
    server=cs;

}




string app_connection::redirect()
{
    MUTEX_INSPECTOR;
    return RVfiles("scripts/relogin");
}


#define switch_ac_ret(a,b) if(vurl[0]==a){vurl.pop_front();b;return;}


void url_notes(const PTR_getter<chat_user>&__U, app_connection*);
bool stop_threads=false;
string error_http(app_connection *cc,const string &e);
void url_admin(const PTR_getter<chat_user>&__U, app_connection*);
void url_chat(const PTR_getter<chat_user>&__U, app_connection* ac);
void url_New_nick(const PTR_getter<chat_user>&__U, app_connection*);
void url_remove_nick(const PTR_getter<chat_user>&__U, app_connection*);
void url_notes_attach(const PTR_getter<chat_user>&__U, app_connection* cc);
void url_change_nick(const PTR_getter<chat_user> &__UU,app_connection *cc);
void url_set_topic(const PTR_getter<chat_user> &__UU,app_connection *cc);
void url_change_contact_mode(const PTR_getter<chat_user> &__UU,app_connection*cc);
void url_change_female_mode(const PTR_getter<chat_user> &__UU,app_connection*cc);
void url_change_male_mode(const PTR_getter<chat_user> &__UU,app_connection*cc);
void url_invite_user(const PTR_getter<chat_user> &__U,app_connection *cc);
void url_ignore_user(const PTR_getter<chat_user> &__UU,app_connection*cc);
void url_kick_user(const PTR_getter<chat_user> &__UU,app_connection *cc);
void url_invite_user(const PTR_getter<chat_user> &__UU,app_connection *cc);
void url_chat_messages2(const PTR_getter<chat_user>&__U, app_connection*);
void url_set_user_status(const PTR_getter<chat_user>&__U, app_connection*);
void url_chat_sendmessage(const PTR_getter<chat_user> &__UU,app_connection*cc);
void url_chat_exit(const PTR_getter<chat_user>&__U, app_connection*);
void url_nick_ban(const PTR_getter<chat_user> &__UU,app_connection *cc);
void url_login_ban(const PTR_getter<chat_user> &__UU,app_connection *cc);
void url_ip_ban(const PTR_getter<chat_user> &__UU,app_connection *cc);
void url_klap_user(const PTR_getter<chat_user> &__UU,app_connection *cc);



void url_ainf_zzz(const PTR_getter<chat_user> &__UU,app_connection*cc);


extern M_MAP<pthread_t,string> pthread_2_url_map;
struct st_pthread_2_url_map
{
    st_pthread_2_url_map(const string &s)
    {
        pthread_2_url_map.set(pthread_self(),s);

    }
    ~st_pthread_2_url_map()
    {
        pthread_2_url_map.erase_key(pthread_self());
    }
};




void do_adminfo(app_connection * cc, const string & title);
string thread_info_html(const map<pthread_t, string> &add_map);
string error_http(app_connection *cc,const string &e)
{
    map<string,string> m;
    m["~error~"]=e;
    return cc->rvfiles(m,"500");
}
void free_memory();
bool check_state(const string &cookie,DB_id& uid);
PTR_getter<chat_user> create_reg_user(const DB_id&uid,const string& cookie, app_connection*);

void url_toggle_moderated(const PTR_getter<chat_user>&__UU, app_connection*)
{
    MUTEX_INSPECTOR;
    user_pair __U(__UU);
    set<CH_id> chs=global_state.get_user_channel(T_WWW,user$id(__UU));
    if (chs.size())
    {
        PTR_getter<chat_channel> ML(chat_channels[*chs.begin()]);
        do_change_mode(ML,__UU,-1);
    }
}
void url_change_voice(const PTR_getter<chat_user>&__UU,app_connection *cc)
{
    MUTEX_INSPECTOR;
    if (!__UU) return;
    user_pair __U(__UU);
    set<CH_id> uchan=global_state.get_user_channel(T_WWW,user$id(__UU));
    if (uchan.size())
    {
        DB_id zid;
        CONTAINER(zid)=atoi(cc->params["zid"]);
        do_change_voice(*uchan.begin(),user$id(__UU),zid,-1);
    }
}
void app_connection::process_web()
{
    MUTEX_INSPECTOR;
    try {



#ifdef __WITH_LICENSE
        license_add_host(headers["HOST"]);
#endif

        string chat_url=strlower(url);
#ifdef LEAKCTLDEBUG
        if (chat_url=="/report_leaks") {
            html_out+=report_leaks();
            return;
        }
#endif
#ifdef _WIN32
        if (chat_url=="/exit_xxx") {
            free_memory();
            PostQuitMessage(0);
            exit(0);
            return;
        }
#endif



        if (params.count("PHPSESSID")) in_cookies["PHPSESSID"]=params["PHPSESSID"];
        vurl = splitTOKd("/", url);
        st_pthread_2_url_map kalllll(url+" ("+peer_ipaddress+")");
//	logErr2("url %s",url.c_str());



        if (url=="/ainf") {
            do_adminfo(this,"Server Info");
            return;
        }

        PTR_getter<chat_user>	__USS(NULL);



        if (!__USS &&  in_cookies["PHPSESSID"].size())
        {
            DB_id uid;
            CONTAINER(uid)=0;
            CONTAINER(uid)=0;
            string cid=repl_percent(in_cookies["PHPSESSID"]);
            if (!__USS && cid.size())
            {
                string cname=chat_config.php_session_prefix.get()+in_cookies["PHPSESSID"];
                struct stat st;
                if (!stat(cname.c_str(),&st))
                {
                    string s;
                    if (load_1_file(s,cname)!=-1)
                    {
                        vector<string> v=splitTOK(";",s);
                        for (unsigned i=0; i<v.size(); i++)
                        {
                            vector<string> w=splitTOK("|",v[i]);
                            if (w.size()==2)
                            {
                                if (w[0]=="uid")
                                {
                                    vector<string> vv=splitTOK(":",w[1]);
                                    if (vv.size())
                                    {
                                        string suid=vv[vv.size()-1];
                                        suid=str_replace("\"","",suid);
                                        CONTAINER(uid)=atoi(suid);
                                        __USS=local_users.get$(uid,T_WWW);
                                        if (!__USS)
                                        {
                                            __USS=create_reg_user(uid,in_cookies["PHPSESSID"],this);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }


            }

        }



        //URls without authorize

        if (__USS) {

            PGLW(chat_user, u,__USS);
            u->last_access_time=time(NULL);
        }
        if (chat_url=="/ainf_zzz.tse") {
            url_ainf_zzz(__USS,this);
            return;
        }

        app_connection *cc=this;
        LICENSE_CK(5);

        if (__USS)
        {
            switch_cc_ret("chat_sendmessage.tse",url_chat_sendmessage(__USS,this));
            switch_cc_ret("chat_messages2.tse",url_chat_messages2(__USS,this));
            switch_ac_ret("toggle_moderated.tse",url_toggle_moderated(__USS,this));
            switch_ac_ret("change_voice.tse",url_change_voice(__USS,this));
            switch_ac_ret("change_nick.tse",url_change_nick(__USS,this));
            switch_ac_ret("remove_nick.tse",url_remove_nick(__USS,this));
            switch_ac_ret("change_contact_mode.tse",url_change_contact_mode(__USS,this));
            switch_ac_ret("change_female_mode.tse",url_change_female_mode(__USS,this));
            switch_ac_ret("change_male_mode.tse",url_change_male_mode(__USS,this));
            switch_ac_ret("set_topic.tse",url_set_topic(__USS,this));
            switch_ac_ret("ignore_user.tse",url_ignore_user(__USS,this));
            switch_ac_ret("kick_user.tse",url_kick_user(__USS,this));
            switch_ac_ret("invite_user.tse",url_invite_user(__USS,this));
            switch_ac_ret("set_user_status.tse",url_set_user_status(__USS,this));
            switch_ac_ret("chat_exit.tse",url_chat_exit(__USS,this));
            switch_ac_ret("nick_ban.tse",url_nick_ban(__USS,this));
            switch_ac_ret("login_ban.tse",url_login_ban(__USS,this));
            switch_ac_ret("ip_ban.tse",url_ip_ban(__USS,this));
            switch_ac_ret("klap_user.tse",url_klap_user(__USS,this));
            if (chat_url.substr(0,7)=="/attach.tse") {
                url_notes_attach(__USS,this);
                return;
            }
        }
        if (__USS)
        {
            switch_ac_ret("notes.tse",url_notes(__USS,this));
            switch_ac_ret("admin.tse",url_admin(__USS,this));
        }
        {
            if (__USS) {

                bool erased;
                {
                    PGLR(chat_user, u,__USS);
                    erased=u->erased;
                }
                if (erased) {	//kicked user
                    html_out=redirect();
                    return;
                }

                if (chat_url=="/newnick.tse") {
                    url_New_nick(__USS,this);
                }
                switch_ac_ret("admin.tse",url_admin(__USS,this));
            }
            //authorized URLs
        }

        if (chat_url!="/chat_sendmessage.tse"&&  chat_url!="/chat.tse/messages.tse" &&chat_url!="/chat.tse/messages2.tse"&&chat_url!="/chat_messages2.tse") {		//live connect
            if (html_out.size()==0) {		//All others URL
                logErr2("unhandled url %s",chat_url.c_str());
                MUTEX_INSPECTOR;
                html_out=redirect();
            }
        }
    }
    catch (socketError e)
    {
    }
    catch (cError e) {
        logErr2("--Error: %s URL=%s [%s]",e.error_string.c_str(),url.c_str(),peer_ipaddress.c_str());
        html_out=error_http(this,e.error_string);
    }
    catch (...) {
        logErr2("--Error: UNKNOW!!! URL=%s [%s]",url.c_str(),peer_ipaddress.c_str());
        html_out=error_http(this,"unknow error "+_DMI());
    }
    if (html_out.size()==0) {		//All others URL

        MUTEX_INSPECTOR;
        html_out=redirect();
    }
}
void url_empty_frame(app_connection * cc)
{
    cc->html_out=global_config.files_get("chat/users");
}

