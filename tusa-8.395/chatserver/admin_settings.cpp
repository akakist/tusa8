#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include "_mutex.h"
#include <list>
#include "mutex_inspector.h"
#include <time.h>
#include <map>
#include "mysql_extens.h"
#include "_mutex.h"
#include "user.h"
#include "cutils.h"
#include "str_lib.h"
#include "web_server.h"
#include "message.h"

#include "chat_def.h"

#include <algorithm>
#include "web_server_data.h"
#include "file_set.h"
#include "copy_file.h"

#include "channels.h"
#include "levels.h"
#include "server_config.h"

#include "chat_config.h"
#include "app_def.h"
#include "oscar_buffer.h"
#include "CH_id.h"
#include "DB_id.h"
#include "chat_msg.h"
#include "chat_thread_event.h"
#include "user_profile.h"
#include "app_con.h"

#include "chat_globals.h"
#include "global_state.h"
#include "web_tools.h"
#ifdef __WITH_LICENSE
#include "license.h"
#endif

void load_app_configuration();
void url_admin_settings(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    LICENSE_CK(4);
    MUTEX_INSPECTOR;
    if (!__UU) {
        cc->html_out=cc->redirect();
        return;
    }
    user_pair __U(__UU);
    if (!user__privileges(__U,"$settings")) return;


    map<string,string> m;
    m["~msg~"]="";
    string srt=cc->params["sort"];
    if (srt=="") srt="nick";
    m["~sort~"]=srt;
    string cmd=cc->params["cmd"];

    if (cmd=="sendsysmsgtoall") {	//send system message to all rooms
        MUTEX_INSPECTOR;
        string msg=cc->params["smsg"];
        if (msg.size()>1025) msg=msg.substr(0,1025);
        if (!user__privileges(__U,"$tags"))  msg=remove_tags(msg);
        msg=replace_links(msg);
        m["~msg~"]=_MSG("sysmsgsent");
    }
    if (cmd=="senddatatoall") {	//send data to all
        MUTEX_INSPECTOR;
        string msg=cc->params["datatoall"];
        if (msg.size()>1024) msg=msg.substr(0,1024);

        chat_event_SEND_DATA_TO_WWW_USERS *e=new chat_event_SEND_DATA_TO_WWW_USERS;
        LEAKCTL_ADD(e);
        e->msg=msg;
        send_chat_event_push_back(e);


        m["~msg~"]=_MSG("datasenttoall");
        logErr2("%s (%s) sended data to all '%s' [%s]",get_cnick(__U).c_str(),user$login(__U).c_str(),msg.c_str(),cc->peer_ipaddress.c_str());
    }
    if (cmd=="refresh") {		//refresh chat
        MUTEX_INSPECTOR;
        chat_event_SEND_DATA_TO_WWW_USERS *e=new chat_event_SEND_DATA_TO_WWW_USERS;
        LEAKCTL_ADD(e);
        e->msg="<script>window.top.location='/chat/';</script>\n";
        send_chat_event_push_back(e);

        m["~msg~"]=_MSG("chatrefreshed");
        logErr2("%s (%s) refreshed chat. [%s]",get_cnick(__U).c_str(),user$login(__U).c_str(),cc->peer_ipaddress.c_str());
    }
    if (cmd=="reload") {		//reload files
        logErr2("%s (%s) reloaded all files. [%s]",get_cnick(__U).c_str(),user$login(__U).c_str(),cc->peer_ipaddress.c_str());
#ifndef WIN32
        FILE *f=popen("./update_files","r");
        string svn_out;
        if (f)
        {
            MUTEX_INSPECTOR;
            while (!feof(f))
            {
                MUTEX_INSPECTOR;
                char s[1000];
                if (fgets(s,1000,f))
                {
                    svn_out+=s;
                }
            }
            fclose(f);
        }
#endif
        {
            MUTEX_INSPECTOR;
            load_app_configuration();
        }

#ifndef WIN32
        m["~msg~"]=_MSG("filesreloaded")+"<p>"+svn_out;
#else
        m["~msg~"]=_MSG("filesreloaded")+"<p>";
#endif
    }
    if (cmd=="killsession") {		//kill session
        MUTEX_INSPECTOR;
        DB_id id;
        CONTAINER(id)=0;
        CONTAINER(id)=atoi(cc->params["id"]);
        vector<PTR_getter<chat_user> >__ZZ=local_users.get$(id);
        for (unsigned i=0; i<__ZZ.size(); i++)
            if (__ZZ[i]) {
                user_destroy(__ZZ[i]);
                m["~msg~"]=_MSG("sessionkilled");
            }
    }
    if (cmd=="senddatatouser") {	//send data to user
        MUTEX_INSPECTOR;
        DB_id id;
        CONTAINER(id)=0;
        CONTAINER(id)=atoi(cc->params["id"]);
        string msg=cc->params["msg"];
        if (msg.size()>1024) msg=msg.substr(0,1024);
        PTR_getter<chat_user> __ZZ =local_users.get$(id,T_WWW);

        if (__ZZ) {
            MUTEX_INSPECTOR;
            user_pair __Z(__ZZ);
            string cnick=get_cnick(__Z);
            string login=user$login(__Z);
            {
                logErr2("%s (%s) sended data to user %s (%s) '%s' [%s]",cnick.c_str(),login.c_str(),get_cnick(__Z).c_str(),user$login(__Z).c_str(),msg.c_str(),cc->peer_ipaddress.c_str());

                m["~msg~"]=_MSG("datasent");
            }
            data_push_back_www(__Z,msg);
        }
    }

    vector<PTR_getter<chat_user> > v=local_users.values$();

    string us;
    for (unsigned int i=0; i<v.size(); i++) {
        MUTEX_INSPECTOR;
        bool zinchat=false;
        user_pair __Z(v[i]);
        map<string,string> q;
        DB_id zid=user$id(__Z);
        zinchat=global_state.inchat$www(zid)||global_state.inchat$irc(zid);
        q["~nick~"]=get_cnick(__Z);
        q["~login~"]=user$login(__Z);

        if (zinchat) {
            q["~inchat~"]=_MSG("yes");
        } else {
            q["~inchat~"]="";
        }

        string ip,zip;
        time_t ld;

        {
            MUTEX_INSPECTOR;
            __ZC_LOCK;
            ld=z->login_date;
        }

        q["~ip~"]=ip;
        q["~id~"]=itostring(CONTAINER(zid));
        q["~time~"]=itostring(time(NULL)-ld);
        us+=RVfiles(q,"admin/settings/settings_user");
    }
    m["~users~"]=us;
    m["~cnt~"]=itostring(v.size());
    cc->html_out=cc->rvfiles(m,"admin/settings/settings");
}
