#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif


#include "_mutex.h"
#include <list>
#include "mutex_inspector.h"
#include <stdio.h>
#include <map>
#include <vector>
#include "web_server.h"
#include "chat_def.h"
#include "cutils.h"
#include "user.h"
#include "message.h"
#include "str_lib.h"
#include "st_stuff.h"
#include "web_server_data.h"
#include "web_tools.h"
#include "file_set.h"
#include "user_credits.h"

#include "levels.h"
#include "channels.h"
#include "server_config.h"

#include "chat_config.h"
#include "app_def.h"
#include "update_cmd.h"
#include "contact_vec.h"
#include "chat_thread_event.h"
#include "des_sid.h"
#include "html_out.h"
#include "time_limit.h"
#include "chat_globals.h"

#ifdef __WITH_LICENSE
#include "license.h"
#endif
#include "app_con.h"
bool is_user_banned(app_connection * cc, const string & ip, const string & login, const string & conf);

void make_login_out(const PTR_getter<chat_user> &__U,app_connection*cc);
void reload(const PTR_getter<nick_vec> &N);
bool check_for_ipban(const string &ip)
{
    MUTEX_INSPECTOR;
    st_dbh d;
    vector<string> q=d.dbh->select_1_rowQ((QUERY)"select ip,proxy from banned_ip where ip='?'"<<ip);
    if (q.size()) return false;

    return true;
}






bool check_for_max_users()
{
#ifdef __WITH_LICENSE
    if (local_users.size$()>7 &&  local_users.size$()>=max_users_in_license()) return false;
#endif
    return true;
}

void flush_user_data(const DB_id&uid);

PTR_getter<chat_user> create_reg_user(const DB_id&uid,const string& cookie, app_connection *cc)
{
    MUTEX_INSPECTOR;
    PTR_getter<chat_user> __UU(NULL);

    flush_user_data(uid);

    chat_user*__u=new chat_user(T_WWW,uid);
    LEAKCTL_ADD(__u);
    __UU=__u;
    PTR_getter<user_profile> __P=user$profiles.FindByID(CONTAINER(uid));
    if (!__P) throw cError("if(!__P) %s %s %s",__FILE__,__LINE__,_DMI().c_str());
    {
        PGLW(chat_user, u,__UU);
        u->__user_profile=__P;
    }
    user_pair __U(__UU);
    init_internal_data(__U);


    {
        __UC_LOCKW;
        u->db_id=uid;
    }

    string login$,cnick$;
    int level$=0;
    vector<string> nicks$;
    {
        PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(uid)));
        login$=u->login;
        cnick$=u->last_nick;
        level$=u->level;
    }
    {
        PGLR(nick_vec,u,nick_vecs.find_by_id_create(CONTAINER(uid)));
        for (map<unsigned int,user_nick>::const_iterator i=u->u_nicks.begin(); i!=u->u_nicks.end(); i++)
        {
            nicks$.push_back(i->second.name);
        }
    }


    {
        __UC_LOCK;

        u->login_date=time(NULL);
        u->last_access_time=time(NULL);
        u->erased=false;
        u->php_session_id=cookie;


    }
    user_register_all_nicks(uid);
    local_users.insert$(__UU);

    st_dbh d;
    string ip=d.dbh->select_1Q((QUERY)"select stat_last_ip from tbl_users where id='?'"<<CONTAINER(user$id(__U)));

    logErrN("users","%s (%s) logged in from WWW [%s].",get_cnick(__U).c_str(),user$login(__U).c_str(),ip.c_str());

    return __UU;
}

