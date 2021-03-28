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

#include "_mutex.h"
#include "user.h"
#include "cutils.h"
#include "str_lib.h"
#include "message.h"
#include "app_con.h"
#include "chat_def.h"
#include "file_set.h"
#include "web_tools.h"
#include "admin.h"

#include "channels.h"
#include "levels.h"
#include "server_config.h"
#include "app_def.h"
#include "chat_config.h"
#include "update_cmd.h"
#include "oscar_buffer.h"
#include "CH_id.h"
#include "DB_id.h"
#include "chat_thread_event.h"
#include "chat_globals.h"
#include "ignore_vec.h"
#ifdef __WITH_LICENSE
#include "license.h"
#endif
#include "global_state.h"

void url_chat_messages2(const PTR_getter<chat_user> &__UU,app_connection*cc)
{
    MUTEX_INSPECTOR;
    LICENSE_CK(12);
    st_update stu;
    if (!__UU || !cc) {
        logErr2("!__UU ||!cc ");
        return;
    }
    user_pair __U(__UU);
    DB_id uid(user$id(__U));
    st_dbh d;
    vector<string> v=d.dbh->select_1_rowQ((QUERY)"select last_channel,last_nick_id,last_nick,setting_show_eye, setting_neye, last_invis_state, last_status_id from tbl_users where id=?"<<CONTAINER(uid));
    if (v.size()!=7) throw cError("EEEEEEEEEEEEEeerror if(!v.size()) %s %d",__FILE__,__LINE__);

    CH_id  last_channel;
    CONTAINER(last_channel)=atoi(v[0]);
    unsigned int last_nick_id	=atoi(v[1]);
    string last_nick=		      v[2];
    unsigned int setting_show_eye=atoi(v[3]);
    unsigned int setting_neye	=atoi(v[4]);
    unsigned int last_invis_state=atoi(v[5]);
    unsigned int last_status_id=atoi(v[6]);

    user_state sta=global_state.get_user_state(uid);



    unsigned int new_nick_id=sta.nick.id;

    if (user$nicks_size(uid) <= 0) {
        cc->redirect(RVfiles("url_login_redirect"));
        logErr2("user$nicks_size(uid) <= 0 %s %d",__FILE__,__LINE__);
        return;
    }
    sta.nick=get_nick(uid,last_nick);

    if (!chat_channels.count(last_channel))
    {
        vector<CH_id>chs=chat_channels._keys();

        if (chs.size());
        last_channel=chs[0];
        d.dbh->real_queryQ((QUERY)"update tbl_users set last_channel='?' where id='?'"<<CONTAINER(uid));
    }

    {
        __UC_LOCKW;
        u->www_offline_time=0;
    }
    if (sta.__channel_us.size()==1)
    {
        if (!sta.__channel_us.count(last_channel) && sta.__channel_us.begin()->second.size()==1 && sta.status.id!=st_invisible && sta.status.id!=st_offinvisible )
        {
            CH_id ochan=sta.__channel_us.begin()->first;
            chat_event_CHROOM *e=new chat_event_CHROOM(uid,ochan, last_channel);
            LEAKCTL_ADD(e);
            send_chat_event_push_back(e);

        }
    }

    sta.www_channel_replace(last_channel);

    if (has_inv(__U))
    {
        if (last_invis_state) {

            sta.status=get_user_status_by_id(__U,st_invisible);
        }
        else
        {
            sta.status=get_user_status_by_id(__U,last_status_id);
        }
    }
    if (sta.status.id==user_state::UNDEF) sta.status=get_user_status_by_id(__U,st_online);
    if (sta.status.id==st_offline) sta.status=get_user_status_by_id(__U,st_online);
    if (sta.status.id==st_offinvisible) sta.status=get_user_status_by_id(__U,st_invisible);
    if (sta.status.id==st_away) sta.status=get_user_status_by_id(__U,st_online);
    if (sta.status.id==st_na) sta.status=get_user_status_by_id(__U,st_online);
    {

        PGLW(user_profile,u,__U.up);
        if (sta.status.id!=u->last_status_id)
        {
            __send_update_d3(chat_thread$set_status$3,CONTAINER(uid),sta.status);
            u->last_status_id=sta.status.id;

        }

    }

    if (setting_show_eye)
    {
        PGLW(user_profile, u,__U.up);
        u->setting_show_eye=setting_show_eye;
        __send_update_d3(user_profile$setting_show_eye$3,CONTAINER(uid),u->setting_show_eye);
        if (!u->setting_show_eye)
        {
            if (u->setting_show_eye!=true)
            {
                u->setting_show_eye=true;
                __send_update_d3(user_profile$setting_show_eye$3,CONTAINER(uid),u->setting_show_eye);
            }
        }

    }
    else {

        PGLW(user_profile, u,__U.up);
        if (u->setting_show_eye)
        {
            if (u->setting_show_eye!=false)
            {
                u->setting_show_eye=false;
                __send_update_d3(user_profile$setting_show_eye$3,CONTAINER(uid),u->setting_show_eye);
            }
        }
    }


    bool nick_update=false;
    {
        PGLW(user_profile, u,__U.up);
        if (u->last_nick_id!=sta.nick.id)
        {
            nick_update=true;
            u->last_nick_id=sta.nick.id;
            u->last_nick=sta.nick.name;
            __send_update_d3(user_profile$set_nick$3,CONTAINER(uid),sta.nick);
        }
    }
    if (nick_update)
    {
        d.dbh->real_queryQ((QUERY)"update tbl_users set last_nick='?',last_nick_id='?' where id=?"<<sta.nick.name<<sta.nick.id<<CONTAINER(uid));
        d.dbh->real_queryQ((QUERY)"update nicks set last_usage_date=NOW() where uid=? and id=?"<<CONTAINER(uid)<<sta.nick.id);
    }

    global_state.set_user_state(uid,sta);

    {
        chat_event_common_JOIN *e=new chat_event_common_JOIN(__UU,last_channel,sta.status,sta.nick,T_WWW);
        LEAKCTL_ADD(e);
        send_chat_event_push_back(e);
    }

    CH_id c(last_channel);
    chat_event_CHAT_MESSAGES2 *e=new chat_event_CHAT_MESSAGES2(c,__UU);
    LEAKCTL_ADD(e);
    send_chat_event_push_back(e);
    cc->allow_build_response=false;
    {
        __UC_LOCKW;
        u->persistent_socket=cc->socket;
        u->www_offline_time=0;
        u->www_inchat=true;
    }
    string out;
    out = make_http_header();
    out += "Content-type: text/html\r\n\r\n";
    cc->write(out);

}
