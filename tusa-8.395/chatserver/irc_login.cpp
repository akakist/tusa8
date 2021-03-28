
#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include "_mutex.h"
#include <list>
#include "mutex_inspector.h"
#include <stdio.h>
#include <map>
#include "user.h"
#include "str_lib.h"
#include "st_stuff.h"
#include "errs.h"
#include "message.h"
#include "chat_def.h"
#include "cutils.h"
#include "irc_server.h"
#include "file_set.h"
#include "web_server_data.h"
#include "channels.h"
#include "levels.h"
#include "irc_server_config.h"

#include "irc_utils.h"
#include "update_cmd.h"
#include "contact_vec.h"
#include "chat_thread_event.h"
#include "global_state.h"
#include "chat_config.h"


void reload(const PTR_getter<nick_vec> &N);



void irc_change_nick(const PTR_getter<chat_user> &__UU,const string &nick)
{
    user_pair __U(__UU);

    bool too_fast=false;
    DB_id uid=user$id(__UU);
    {
        __UC_LOCK;
        too_fast=(time(NULL)-u->last_change_nick)<chat_config.change_nick_timeout_sec.get();
    }

    if (too_fast) {
        irc_send_reply(__U,401,"Too fast nick change. Wait some seconds...");
        return;
    }
    if (!NICK_EXISTS(uid,nick)) {
        irc_send_reply(__U,432,nick+": You dont have this nick. To use other nick, register it from WEB.");
        return;
    }
    DB_id zzzz;
    CONTAINER(zzzz)=0;
    user_nick nn("",user_state::UNDEF,zzzz);
    {
        PGLR(nick_vec, u,nick_vecs.find_by_id_create(CONTAINER(uid)));
        for (map<unsigned int,user_nick> ::const_iterator i=u->u_nicks.begin(); i!=u->u_nicks.end(); i++) {
            if (str_nick(nick)==str_nick(i->second.name)) {
                nn=i->second;
                break;
            }
        }
    }
    if (nn.id==user_state::UNDEF) {
        irc_send_reply(__U,432,nick+": You dont have this nick. To use other nick, register it from WEB.");
        return;
    }
    if (nn.id==get_cnick_id(__U)) {
        irc_send_reply(__U,432,nick+": You already use this nick.");
        return;
    }
    if (!global_state.inchat$irc(uid)) {

        string cnick=get_cnick(__U);
        string s=":"+cnick+"!"+irc_nick$host(cnick)+" NICK :"+nick+"\r\n";
        data_push_back_irc(__U.cu,s);
    }
    {
        __UC_LOCKW;
        u->last_change_nick=time(NULL);
    }
    global_state.set_nick(uid,nn);

    if (nn.id!=user_state::UNDEF)
    {
        PGLW(user_profile, u,__U.up);
        u->last_nick=nn.name;
        u->last_nick_id=nn.id;
    }


    us_SET_NICK(uid,nn);


    return;
}
void disconnect_irc_user(const PTR_getter<chat_user> &__UU)
{
    MUTEX_INSPECTOR;
    PTR_getter<c_socket>__S(NULL);
    {
        PGLW(chat_user, u,__UU);
        __S=u->persistent_socket;
        u->persistent_socket=NULL;
    }
    if (__S)
    {
        disconnect(__S);
    }

}
