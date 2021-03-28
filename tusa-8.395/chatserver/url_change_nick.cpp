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

void url_change_nick(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    MUTEX_INSPECTOR;
    if (!__UU) return;
    user_pair __U(__UU);
    unsigned int nn;
    LICENSE_CK(15);

    bool tofast=false;
    {
        __UC_LOCK;
        tofast=time(NULL)-u->last_change_nick<chat_config.change_nick_timeout_sec.get();
    }
    if (tofast) {
        send_syspriv_msg(__U,"Слишком частая смена ника. Ник можно менять не чаще чем раз в 10 сек.");
        return;
    }
    {
        __UC_LOCK;
        u->last_change_nick=time(NULL);
    }
    nn=atoi(cc->params["id"]);
    if (nn==get_cnick_id(__U)) {
        return;
    }
    DB_id zzzz;
    CONTAINER(zzzz)=0;
    user_nick nnn("undef",user_state::UNDEF,zzzz);
    DB_id uid=user$id(__UU);
    {
        PGLR(nick_vec, u,nick_vecs.find_by_id_create(CONTAINER(uid)));
        map<unsigned int,user_nick>::const_iterator ii=u->u_nicks.find(nn);
        if (ii!=u->u_nicks.end()) {
            nnn=ii->second;
        }
        else
        {
            if (u->u_nicks.size())
                nnn=u->u_nicks.begin()->second;
        }
    }
    us_SET_NICK(uid,nnn);


}
