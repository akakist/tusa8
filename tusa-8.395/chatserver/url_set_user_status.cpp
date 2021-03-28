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

void url_set_user_status(const PTR_getter<chat_user> &__UU,app_connection*cc)
{
    MUTEX_INSPECTOR;

    LICENSE_CK(12);
    if (!__UU) {
        return;
    }
    user_pair __U(__UU);
    map<string,string> m;
    unsigned int st=atoi(cc->params["status"]);

    us_SET_STATUS(user$id(__UU),get_user_status_by_id(__U,st));


}
