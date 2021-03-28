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

void url_chat_sendmessage(const PTR_getter<chat_user> &__UU,app_connection*cc)
{
    MUTEX_INSPECTOR;
    LICENSE_CK(17);
    if (!__UU) {
        return;
    }
    user_pair __U(__UU);

    LICENSE_CK(16);
    set<CH_id> channel=global_state.get_user_channel(T_WWW,user$id(__UU));
    CH_id ch;
    CONTAINER(ch)=0;
    if (channel.size()>1)
        throw cError("channel.size()>1 "+_DMI());
    {
        if (channel.size())
            ch=*channel.begin();
        else {
            send_syspriv_msg(__U,"Вы находитесь не на канале");
            return;
        }
    }
    klap_item bit;
    if (_klap_stuff.check_ban_by_uid(ch,user$id(__UU),bit))
    {
        MUTEX_INSPECTOR;
        char ss[300];
        snprintf(ss,sizeof(ss),"У вас кляп на этот канал. Время %s, за %s, до %s",bit.time_cnt_str.c_str(),bit.descr.c_str(),date2mysql(bit.time_ban_end).c_str());
        send_syspriv_msg(__U,ss);
        return;

    }
    string msg=unescape_rus(cc->params["msg"]);
    string fornick=unescape_rus(cc->params["fornick"]);

    chat_event_SEND_MESSAGE *e=new chat_event_SEND_MESSAGE(ch,user$id(__UU),fornick,msg,cc->params["priv"]=="true",cc->params["sys"]=="true",cc->params["makeit"]=="true");
    LEAKCTL_ADD(e);
    send_chat_event_push_back(e);
}
