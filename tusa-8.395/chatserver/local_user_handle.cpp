#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

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
#include <errno.h>
#include <sys/stat.h>
#include "_mutex.h"
#include <time.h>

#include "cutils.h"
#include <algorithm>
#include "user.h"
#include "message.h"
#include "web_server_data.h"
#include "file_set.h"
#include "tcp_stats.h"
#include "server_config.h"
#include "app_def.h"
#include "tcp_sender.h"
#include "web_tools.h"
#include "version.h"
#include "user_credits.h"
#include "chat_def.h"
#include "irc_server_config.h"

#include "chat_config.h"
#include "channels.h"
#include "irc_utils.h"
#include "update_cmd.h"
#include "ignore_vec.h"
#include "user_profile.h"
#include "contact_vec.h"
#include "notes.h"
#include "global_state.h"
#include "chat_thread_event.h"
class ignore_vec;
class contact_vec;


void www_local_user_handle_rn(const PTR_getter<chat_user> &__ZZ)
{
    MUTEX_INSPECTOR;
    bool need_rn=false;

    {
        MUTEX_INSPECTOR;
        PGLW(chat_user,   z,__ZZ);
        if (z->_ct!=T_WWW)return;

        if (time(NULL) - z->iframe_last_send_time > 10) {
            z->iframe_last_send_time = time(NULL);
            z->iframe_buffer_size++;
            need_rn=true;
        }

    }
    if (need_rn)
    {
        MUTEX_INSPECTOR;

        data_push_back_www(__ZZ,"\r\n");
    }
}
void www_local_user_handle_max_inactive_time(const PTR_getter<chat_user> &__ZZ)
{
    MUTEX_INSPECTOR;
    set<DB_id> s_erased;
    bool psock_connected = false;
    bool psock_connected_irc = false;
    time_t last_access_time = 0;
    PTR_getter<c_socket > pusock(NULL);
    {
        PGLW(chat_user,   z,__ZZ);
        last_access_time = z->last_access_time;
        pusock 		=z->persistent_socket;
    }

    if (pusock) {
        PGLR(c_socket, ps,pusock);
        psock_connected = ps->connected();
    }
    else psock_connected=false;

    if (time(NULL) - last_access_time > chat_config.user_max_inactive_time_sec && !psock_connected )
    {
        MUTEX_INSPECTOR;
        {
            st_dbh d;
            user_pair __Z(__ZZ);
            string ip=d.dbh->select_1Q((QUERY)"select stat_last_ip from tbl_users where id='?'"<<CONTAINER(user$id(__Z)));
            logErrN("users","%s (%s) killed due timeout [%s] {%d}",get_cnick(__Z).c_str(),
                    user$login(__Z).c_str(),ip.c_str(),user$get_spent_time(__Z));
        }
        {
            PGLW(chat_user, z,__ZZ);
            z->erased=true;
        }
        DB_id zid=user$id(__ZZ);
        local_users.erase$(zid,T_WWW);

    }

}
void www_local_user_handle_offline_status(const PTR_getter<chat_user> &__ZZ)
{
    MUTEX_INSPECTOR;
    if (!__ZZ) throw cError("!ZZ"+_DMI());
    bool conn_active=false;
    PTR_getter<c_socket> __S(NULL);
    unsigned int zstatus;
    DB_id db_id;
    CONTAINER(db_id)=0;
    {
        MUTEX_INSPECTOR;
        PGLR(chat_user,z,__ZZ);
        __S=z->persistent_socket;
        db_id=z->db_id;
    }
    zstatus=global_state.get_status(db_id).id;
    if (__S)
    {
        MUTEX_INSPECTOR;
        PGLW(c_socket,s,__S);
        conn_active=s->connected();
    }

    bool cond;

    {
        MUTEX_INSPECTOR;
        PGLW(chat_user,z,__ZZ);

        if (conn_active) z->www_offline_time=0;

        cond=!z->www_offline_status_sent &&
             z->www_offline_time&&
             time(NULL)-z->www_offline_time>10 &&
             z->www_inchat &&
             !conn_active;
        if (!conn_active && z->www_offline_time==0)
        {
            MUTEX_INSPECTOR;
            z->www_offline_time=time(NULL);
        }
        if (cond)
        {
            MUTEX_INSPECTOR;
            z->www_offline_status_sent=true;
            z->last_access_time=time(NULL);

        }
    }

    user_pair __Z(__ZZ);
    DB_id zid=user$id(__Z);
    bool isinv=is_inv(zid);

    if (cond) {
        MUTEX_INSPECTOR;

        if (isinv)
            us_SET_STATUS(zid,get_user_status_by_id(__Z, st_offinvisible));
        else
            us_SET_STATUS(zid,get_user_status_by_id(__Z, st_offline));

    }
    if (conn_active && (zstatus==st_offinvisible || zstatus==st_offline))
    {
        MUTEX_INSPECTOR;
        unsigned int lstatus;
        {
            MUTEX_INSPECTOR;
            PGLR(user_profile,z,__Z.up);
            lstatus=z->last_status_id;
        }
        us_SET_STATUS(zid,get_user_status_by_id(__Z, lstatus));
    }

}


void www_local_user_check_for_channel_cleanup(const PTR_getter<chat_user>&__ZZ )
{
    MUTEX_INSPECTOR;

    if (!__ZZ)  return;
    const user_pair __Z(__ZZ);
    bool conn_active=false;

    PTR_getter<c_socket>sok(NULL);
    {
        MUTEX_INSPECTOR;
        __ZC_LOCK;
        if (z->_ct!=T_WWW) return;
        sok=z->persistent_socket;

    }
    if (sok)
    {
        MUTEX_INSPECTOR;
        PGLW(c_socket,s,sok);
        conn_active=s->connected();
    }


    bool cond=false;
    size_t channel_size=global_state.channel_size(user$id(__Z));
    {
        MUTEX_INSPECTOR;
        __ZC_LOCK;
        cond=channel_size && !conn_active && time(NULL)-z->www_offline_time > chat_config.chat_offline_timeout_sec && z->www_offline_time!=0;
        if (!cond)
        {
            if (z->erased && channel_size)
                cond=true;
        }
    }
    if (cond) {
        MUTEX_INSPECTOR;
//		logErr2("us_PART_CHANNEL %d %s %d",CONTAINER(user$id(__Z)),__FILE__,__LINE__);
        us_PART_CHANNEL(T_WWW,user$id(__Z));
        {
            __ZC_LOCK;
            z->last_access_time=time(NULL);
        }
    }


}
void local_users_iterate()
{
    MUTEX_INSPECTOR;

    vector < PTR_getter<chat_user> > v=local_users.values$();
    time_t t=time(NULL);
    for (unsigned i=0; i<v.size(); i++)
    {
        MUTEX_INSPECTOR;
        try {
            MUTEX_INSPECTOR;
            if (!v[i])continue;
            www_local_user_handle_offline_status(v[i]);
            www_local_user_handle_rn(v[i]);
            www_local_user_handle_max_inactive_time(v[i]);
            www_local_user_check_for_channel_cleanup(v[i]);



        }
        STDCATCHS("local_users_iterate");

    }
    local_users.__clean$();


}
void update_db_cache()
{
    MUTEX_INSPECTOR;
    time_t drop_timeout=chat_config.data_drop_timeout_sec;
    try {
        __notes.purge(drop_timeout);
    }
    STDCATCHS("purge");
    try {
        contact_vecs.purge(drop_timeout);
    }
    STDCATCHS("purge");
    try {
        dump_credits_log();
    }
    STDCATCHS("purge");
    try {
        ignore_vecs.purge(drop_timeout);
    }
    STDCATCHS("purge");
    try {
        nick_vecs.purge(drop_timeout);
    }
    STDCATCHS("purge");
    try {
        note_texts.purge(drop_timeout);
    }
    STDCATCHS("purge");
    try {
        user$profiles.purge(drop_timeout);
    }
    STDCATCHS("purge");
}

