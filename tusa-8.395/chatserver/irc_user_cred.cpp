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
#include "server_config.h"
#include "irc_server_config.h"

#include "user_credits.h"
#include "thread_descr.h"
#include "irc_utils.h"
#include "tcp_stats.h"
#include "chat_thread_event.h"
#include "irc_session.h"
#include "chat_globals.h"
void irc_cmd_UTTC(const PTR_getter<chat_user> &__UU,irc_command &cmd)
{
    MUTEX_INSPECTOR;
    user_pair __U(__UU);
    if (!user__privileges(__U,"$credits"))
    {
        irc_send_reply(__UU,421,cmd.command+": Unauthorized command" + _SFL);
        return;

    }
    vector<string> vV=splitTOK(" ",cmd.data);
    if (vV.size()==4) {

        string nick=vV[0];
        string summa=vV[1];
        string type=vV[2];
        string cookie=vV[3];

        DB_id to;
        CONTAINER(to)=0;
        DB_id from=all_user_nick_2_DB_id(nick);

        if (CONTAINER(from)==0) {
            st_dbh d;
            CONTAINER(from)=atoi(d.dbh->select_1Q((QUERY)"select uid from nicks where str_nick='?'"<<str_nick(nick)));
        }
        if (CONTAINER(from)==0) {
            irc_send_reply(__U,401,"No such nick/channel." +_SFL);
            return;
        }
        if (!user__privileges(__U,"$tags")) {
            irc_send_reply(__U,401,"Bad command" +_SFL);
            return;
        }

        to=user$id(__UU);

        Double tot_cost;
        tot_cost.container=atof(summa.c_str());
        bool OK = false;
        if (tot_cost.container>0 )
        {
            Double rem_f,rem_t;
            if (buy_thing(from,to,tot_cost, type,rem_f,rem_t)) // true if success
            {
                data_push_back_irc(__U.cu,
                                   (string)"UTTC_REPLY "
                                   "NICK:"+nick+";"
                                   "STATUS:OK;"
                                   "SUMMA:"+ftostring(tot_cost.container)+";"
                                   "COOKIE:"+cookie+";"
                                   "REMINDER_FROM:"":"+ftostring(rem_f.container)+";"
                                   "REMINDER_TO:"+ftostring(rem_t.container)+";"
                                   "\n");
            } else {
                data_push_back_irc(__U.cu,
                                   "UTTC_REPLY "
                                   "NICK:"+nick+";"
                                   "STATUS:FAIL;"
                                   "ERROR:Not_enough_credits;"
                                   "COOKIE:"+cookie+";"
                                   +"\n");
            }
        }
        else
        {
            data_push_back_irc(__U.cu,
                               "UTTC_REPLY "
                               "NICK:"+nick+";"
                               "STATUS:FAIL;"
                               "ERROR:Not_enough_credits;"
                               "COOKIE:"+cookie+";"
                               +"\n");
        }
        return;
    }
    else
    {
        data_push_back_irc(__U.cu,"UTTC_REPLY invalid_format\n");
    }
}

void irc_cmd_CTTU(const PTR_getter<chat_user> &__UU,irc_command &cmd)
{
    MUTEX_INSPECTOR;
    user_pair __U(__UU);
    if (!user__privileges(__U,"$credits"))
    {
        irc_send_reply(__UU,421,cmd.command+": Unauthorized command" +_SFL);
        return;

    }
    vector<string> vV=splitTOK(" ",cmd.data);

    if (vV.size()==4) {
        string nick=vV[0];
        string summa=vV[1];
        string type=vV[2];
        string cookie=vV[3];
        DB_id to, from;
        CONTAINER(to)=0;
        CONTAINER(from)=0;
        to=all_user_nick_2_DB_id(nick);

        if (CONTAINER(to)==0)
        {
            st_dbh d;
            CONTAINER(to)=atoi(d.dbh->select_1Q((QUERY)"select uid from nicks where str_nick='?'"<<str_nick(nick)));
        }
        if (CONTAINER(to)==0) {
            irc_send_reply(__U,401,"No such nick/channel."+_SFL  );
            return;
        }
        from=user$id(__UU);

        Double tot_cost;
        tot_cost.container=atof(summa.c_str());

        if (tot_cost.container>0)
        {
            Double rem_f,rem_t;
            if (buy_thing(from,to,tot_cost, type,rem_f,rem_t)) // true if success
            {

                data_push_back_irc(__U.cu,
                                   (string)"CTTU_REPLY "
                                   "NICK:"+nick+";"
                                   "STATUS:OK;"
                                   "SUMMA:"+ftostring(tot_cost.container)+";"
                                   "COOKIE:"+cookie+";"
                                   "REMINDER_FROM:"":"+ftostring(rem_f.container)+";"
                                   "REMINDER_TO:"+ftostring(rem_t.container)+";"
                                   "\n");

            }
            else
            {
                data_push_back_irc(__U.cu,
                                   "CTTU_REPLY "
                                   "NICK:"+nick+";"
                                   "STATUS:FAIL;"
                                   "ERROR:Not_enough_credits;"
                                   "COOKIE:"+cookie+";"
                                   +"\n");
            }
        } else
        {
            data_push_back_irc(__U.cu,
                               "CTTU_REPLY "
                               "NICK:"+nick+";"
                               "STATUS:FAIL;"
                               "ERROR:Not_enough_credits;"
                               "COOKIE:"+cookie+";"
                               +"\n");
        }

        return;
    }
    else
    {
        data_push_back_irc(__U.cu,"CTTU_REPLY invalid_format\n");
    }

}
