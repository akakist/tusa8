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
#include "chat_thread_event.h"
#include "user_profile.h"
#include "user_credits.h"
#include "update_cmd.h"
#include "web_tools.h"
#include "app_con.h"
#include "chat_globals.h"

#ifdef __WITH_LICENSE
#include "license.h"
#endif

void send_system_note(const DB_id& uid,const string &subj,const string& msg);
map<CH_id,chat_channel_data> load_channels();
bool add_channel(const string&); // true if success
bool remove_channel(const CH_id &cid); // true if success
void update_channel(const chat_channel_data& cd, const CH_id & );

void url_admin_channels_edit(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    LICENSE_CK(7);
    logErr2("url_admin_channels_edit");
    MUTEX_INSPECTOR;
    if (!__UU) {
        cc->html_out=cc->redirect();
        return;
    }
    user_pair __U(__UU);
    if (!user__privileges(__U,"$settings") )
    {
        return;
    }

    map<string,string>m;
    m["~msg~"]="";
    CH_id cid;
    CONTAINER(cid)=0;
    CONTAINER(cid)=atoi(cc->params["id"]);

    map<CH_id,chat_channel_data>v=load_channels();
    if (!v.count(cid)) throw cError((string)"!v.count(cid)"+_DMI());
    chat_channel_data &c=v[cid];
    if (cc->params["cmd"]=="update")
    {
        c.name=cc->params["name"];
        c.name2=cc->params["name2"];
        c.censored=strupper(cc->params["censored"])=="ON";
        c.move=strupper(cc->params["move"])=="ON";
        CONTAINER(c.move_to_channel)=atoi(cc->params["move_to_channel"]);
        c.extended=strupper(cc->params["extended"])=="ON";
        c.read_level=atoi(cc->params["rl"]);
        c.write_level=atoi(cc->params["wl"]);
        c.smiles_allowed=strupper(cc->params["sa"])=="ON";
        update_channel(c,cid);
        m["~msg~"]=_MSG("channel_updated");
    }
    m["~id~"]=itostring(CONTAINER(c.chid));
    m["~name~"]=c.name;
    m["~name2~"]=c.name2;
    m["~censored~"]=checked(c.censored);
    m["~move~"]=checked(c.move);
    m["~move_to_channel~"]=itostring(CONTAINER(c.move_to_channel));
    m["~extended~"]=checked(c.extended);
    m["~smiles_allowed~"]=checked(c.smiles_allowed);

    map<unsigned int, c_level> lvl;
    lvl=LEVELS_GET();
    string sr="<SELECT name=rl>",sw="<SELECT name=wl>";
    for (map<unsigned int,c_level>::const_iterator j=lvl.begin(); j!=lvl.end(); j++) {
        sr+="<option value="+itostring(j->first);
        sw+="<option value="+itostring(j->first);
        if (j->first==c.read_level) sr+=" SELECTED";
        if (j->first==c.write_level) sw+=" SELECTED";
        sr+=">"+j->second.name(0)+"\r\n";
        sw+=">"+j->second.name(0)+"\r\n";
    }
    sr+="</SELECT>";
    sw+="</SELECT>";
    m["~read_level~"]=sr;
    m["~write_level~"]=sw;

    cc->html_out=cc->rvfiles(m,"admin/channels/edit");
}
void url_admin_channels(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    MUTEX_INSPECTOR;
    if (!__UU) {
        cc->html_out=cc->redirect();
        return;
    }
    user_pair __U(__UU);
    if (!user__privileges(__U,"$settings"))
        return;


    if (cc->vurl.size())
    {
        if (cc->vurl[0]=="edit.tse")
        {
//            cc->vurl.pop_front();
            url_admin_channels_edit(__UU,cc);
            return;

        }
    }


    map<string,string> m;
    m["~msg~"]="";

    if (cc->params["cmd"]=="addchannel")
    {
        logErr2("addchannel");
        string s=cc->params["add_id"];
        if (add_channel(s))
            m["~msg~"]="Комната добавлена успешно";
        else
            m["~msg~"]=_MSG("cant_add_channel");

    }
    if (cc->params["cmd"]=="del")
    {
        CH_id id;
        CONTAINER(id)=atoi(cc->params["id"]);
        if (remove_channel(id))
            m["~msg~"]=_MSG("channelremoved");
        else
            m["~msg~"]=_MSG("cantremovechannel");

    }
    LICENSE_CK(13);
    map<CH_id,chat_channel_data> vc=load_channels();
    string namelist;
    for (map<CH_id,chat_channel_data> ::iterator i=vc.begin(); i!=vc.end(); i++)
    {
        map<string,string>q;
        q["~id~"]=itostring(CONTAINER(i->second.chid));
        q["~irc_name~"]=i->second.irc_name;
        q["~topic~"]=i->second.topic;
        namelist+=RVfiles(q,"admin/channels/list_row_ref");
    }
    m["~namelist~"]=namelist;





    string data;

    cc->html_out=cc->rvfiles(m,"admin/channels/channels");
}
