#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include "_mutex.h"
#include <list>
#include "mutex_inspector.h"
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <sys/stat.h>
#include <map>
#include "mysql_extens.h"
#include "_mutex.h"
#include "user.h"
#include "cutils.h"
#include "str_lib.h"
#include "web_server.h"
#include "message.h"
#include "chat_def.h"
#include "file_set.h"
#include "web_server_data.h"
#include "web_tools.h"

#include "channels.h"
#include "levels.h"
#include "server_config.h"

#include "app_def.h"

#include "chat_config.h"
#include "irc_server_config.h"

#include "update_cmd.h"
#include "chat_globals.h"
#include "user_credits.h"
#include <math.h>
#ifdef __WITH_LICENSE
#include "license.h"
#endif
#include "app_con.h"

void reload(const PTR_getter<nick_vec> &N);
void send_system_note(const DB_id& uid,const string &subj,const string& msg);


void url_New_nick(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    LICENSE_CK(16);
    MUTEX_INSPECTOR;
    st_dbh d;
    if (!__UU) return;
    if (!__UU) {
        cc->html_out=cc->redirect();
        return;
    }
    DB_id cc_user_id=user$id(__UU);


    user_pair __U(__UU);

    map<string,string> m;
    if (cc->params["nick"]=="") {
        m["~msg~"]="";
        m["~nick~"]="";
        cc->html_out=cc->rvfiles(m,"new_nick");
        return;
    }
    LICENSE_CK(18);
    st_update stu;
    string nick=cc->params["nick"];
    try {

        if (!check_maty(" "+nick)) {
            m["~nick~"]=nick;
            m["~msg~"]=_MSG("incorrectwordsinnick");
            cc->html_out=cc->rvfiles(m,"new_nick");
            return;
        }
        if (!check_nick(nick)) {		//bad nick
            m["~nick~"]=nick;
            m["~msg~"]=_MSG("incorrectnick");
            cc->html_out=cc->rvfiles(m,"new_nick");
            return;
        }
        if (nick.size()>16) {		//Nick to long
            m["~nick~"]=nick;
            m["~msg~"]=_MSG("nickistoolong");
            cc->html_out=cc->rvfiles(m,"new_nick");
            return;
        }
        DB_id zid;
        CONTAINER(zid)=atoi(d.dbh->select_1Q((QUERY)"select uid from nicks where str_nick='?'"<<str_nick(nick)));
        {
            if (CONTAINER(zid)!=0)
            {
                m["~nick~"]=nick;
                m["~msg~"]=_MSG("nickexists");
                cc->html_out=cc->rvfiles(m,"new_nick");
                return;
            }
        }
        string uinfo_login;
        DB_id cc_user_id=user$id(__UU);
        uinfo_login=user$login(__U);
        if (!can_add_nick(__U))
        {
            m["~nick~"]=nick;
            m["~msg~"]=_MSG("toomanynicks");
            cc->html_out=cc->rvfiles(m,"new_nick");
            return;
        }
        unsigned int newid=0;


        st_dbh d;
        d.dbh->real_queryQ((QUERY)"insert into nicks (nick,str_nick,uid,banned,bby,last_usage_date) values ('?','?','?','0','0',NOW())"<<nick<<str_nick(nick)<<CONTAINER(cc_user_id));
        newid=atoi(d.dbh->select_1("select LAST_INSERT_ID()"));
        user_nick nnn(nick,newid,cc_user_id);

        {
            PGLW(nick_vec,n,nick_vecs.find_by_id_create(CONTAINER(cc_user_id)));
            n->u_nicks.insert(make_pair(newid,nnn));
        }

        __send_update_d3(nick$add$3,cc_user_id,nnn);

        logErrN("users","%s registered neu nick '%s'.",user$login(__U).c_str(),nick.c_str());

        user_nick n=user_nick(nick,newid,cc_user_id);
        us_SET_NICK(cc_user_id,n);



        cc->html_out+="<html><script>window.opener.location='/c2/url_login.php?r="+itostring(rand())+"'; window.close();</script>\n";
    }
    catch (cError e) {
        m["~nick~"]=nick;
        m["~msg~"]=e.error_string;
        cc->html_out=cc->rvfiles(m,"new_nick");
    }
}

