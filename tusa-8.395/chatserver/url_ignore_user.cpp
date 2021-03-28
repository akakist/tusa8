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

void url_ignore_user(const PTR_getter<chat_user> &__UU,app_connection*cc)
{
    MUTEX_INSPECTOR;
    st_update stu;
    user_pair __U(__UU);

    LICENSE_CK(14);


    map<string,string> m;
    DB_id zid;
    CONTAINER(zid)=0;
    CONTAINER(zid)=atoi(cc->params["id"]);
    DB_id uid=user$id(__UU);
    if (zid==uid) return;
    if (CONTAINER(zid)==0) return;//throw cError("--Error: on_IGNORE zid 0");
    bool cantignore=false;
    unsigned int zlevel=0,ulevel=user$level(__U);
    int zgender=0;
    {
        PGLR(user_profile,z,user$profiles.FindByID(CONTAINER(zid)));
        zlevel=z->level;
        zgender=z->get_gender();
    }
    if (zlevel/sub_levels>=am_killer) {
        if (ulevel/sub_levels < zlevel/sub_levels) {
            send_syspriv_msg(__U,_MSG("cantignore"));
            return;
        }
    }
    bool need_add=false;
    PTR_getter<ignore_vec> P_uid_ignore_vec=ignore_vecs.find_by_id_create(CONTAINER(uid));
    {
        PGLR(ignore_vec,u,P_uid_ignore_vec);
        if (u->ignore.count(zid)) need_add=false;
        else need_add=true;
    }

    if (need_add) {
        __send_update_d3(ignore$add$3,CONTAINER(uid),CONTAINER(zid));
        send_syspriv_msg(__U,_MSG("user_ignored"));
        {
            PGLW(ignore_vec,u,P_uid_ignore_vec);
            u->ignore.insert(zid);
        }

        data_push_back_www(__UU,"<script>parent.ChangeIgnore("+itostring(CONTAINER(zid))+",1,"+itostring(zgender)+");</script>\n");

    }
    else
    {
        __send_update_d3(ignore$remove$3,CONTAINER(uid),CONTAINER(zid));
        send_syspriv_msg(__U,_MSG("user_unignored"));
        {
            PGLW(ignore_vec,u,P_uid_ignore_vec);
            u->ignore.erase(zid);
        }

        data_push_back_www(__UU,"<script>parent.ChangeIgnore("+itostring(CONTAINER(zid))+",0,"+itostring(zgender)+");</script>\n");
    }


}
