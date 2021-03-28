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

#include "user.h"
#include "cutils.h"
#include "str_lib.h"

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
#include "update_cmd.h"
#include "user_profile.h"
#include "app_con.h"

#include "chat_globals.h"
#include "web_tools.h"

#ifdef __WITH_LICENSE
#include "license.h"
#endif
void remove_moderator(const DB_id& uid);
void add_moderator(const DB_id& uid,const CH_id& channel);
map<DB_id,vector<CH_id> > get_moderators();

string is_checked(bool b)
{
    if (b) return "checked";
    return "";
}

void url_admin_privs(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    LICENSE_CK(9);
    MUTEX_INSPECTOR;
    st_update stu;
    if (!__UU) {
        cc->html_out=cc->redirect();
        return;
    }
    user_pair __U(__UU);

    if (!user__privileges(__U,"$privs")) return;
    LICENSE_CK(10);

    map<string,string> m;
    m["~msg~"]="";
    m["~level~"]=itostring(user$level(__U)/sub_levels);
    string us;
    string cmd=cc->params["cmd"];
    st_dbh d;
    m["~nick~"]=cc->params["nick"];
    if (cmd=="add" || cmd=="remove") {
        MUTEX_INSPECTOR;
        string nick=cc->params["nick"];


        DB_id uid;
        CONTAINER(uid)=atoi(d.dbh->select_1Q((QUERY)"select uid from nicks where str_nick='?'"<<str_nick(nick)));
        logErr2("uid %d nick %s",CONTAINER(uid),nick.c_str());

        if (CONTAINER(uid)!=0) {
            MUTEX_INSPECTOR;
            int p;

            PTR_getter<user_profile> __Z(user$profiles.FindByID(CONTAINER(uid)));
            if (__Z)
            {
                PGLW(user_profile, z,__Z);
                if (cmd=="add")
                {
                    z->__privileges.insert(cc->params["priv"]);
                    logErr2("__privileges.insert %s",cc->params["priv"].c_str());
                }
                else
                {
                    z->__privileges.erase(cc->params["priv"]);
                    logErr2("__privileges.erase %s",cc->params["priv"].c_str());
                }
                __send_update_d3(user_profile$privs$3,uid,z->__privileges);
            }
            else
            {
                m["~msg~"]=_MSG("usernotfound");
            }
            if (cmd=="add")
                d.dbh->real_queryQ((QUERY)"insert into tbl_privs (uid,priv) values ('?','?')"<<CONTAINER(uid)<<cc->params["priv"]);
            else
                d.dbh->real_queryQ((QUERY)"delete from tbl_privs where uid=? and priv='?'"<<CONTAINER(uid)<<cc->params["priv"]);

            logErr2("%s (%s) %s privileges for user id=%d [%s] priv %s",get_cnick(__U).c_str(),user$login(__U).c_str(),cmd.c_str(),CONTAINER(uid),cc->peer_ipaddress.c_str(),cc->params["priv"].c_str());
        } else {
            m["~msg~"]=_MSG("usernotfound");
        }
    }
    vector< vector<string> > v=d.dbh->exec("select id,priv,uid from tbl_privs");

    map<DB_id,set<string> > mpriv;
    for (unsigned int i=0; i<v.size(); i++)
    {

        if (v[i].size()!=3) throw cError("select failed. size different."+_DMI());
        DB_id uid;
        CONTAINER(uid)=atoi(v[i][2]);
        if (v[i][1].size())
            mpriv[uid].insert(v[i][1].substr(1,v[i][1].size()-1));
    }

    for ( map<DB_id,set<string> >::iterator i=mpriv.begin(); i!=mpriv.end(); i++)
    {

        MUTEX_INSPECTOR;
        string lpriv=join(", ",i->second);
        map<string,string> q;

        q["~id~"]=itostring(CONTAINER(i->first));
        {
            MUTEX_INSPECTOR;

            q["~login~"]=d.dbh->select_1Q((QUERY)"select login from tbl_users where id=?"<<CONTAINER(i->first));
            q["~cnick~"]=d.dbh->select_1Q((QUERY)"select last_nick from tbl_users where id=?"<<CONTAINER(i->first));
        }

        q["~privs~"]=lpriv;
        us+=RVfiles(q,"admin/privileges/privileges_user");
    }
    m["~users~"]=us;

    cc->html_out=cc->rvfiles(m,"admin/privileges/privileges");
}



void url_admin_moders(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    MUTEX_INSPECTOR;
    if (!__UU) {
        cc->html_out=cc->redirect();
        return;
    }
    user_pair __U(__UU);
    st_dbh d;
    if (!user__privileges(__U,"$moders")) return;
    map<string,string> m;
    m["~msg~"]="";
    m["~level~"]=itostring(user$level(__U));

    if (cc->params.count("remove"))
    {
        DB_id uid;
        CONTAINER(uid)=0;
        CONTAINER(uid)=atoi(cc->params["remove"]);
        remove_moderator(uid);

    }
    if (cc->params.count("nick_add"))
    {
        MUTEX_INSPECTOR;
        DB_id uid;
        CONTAINER(uid)=0;
        CONTAINER(uid)=atoi(d.dbh->select_1Q((QUERY)"select uid from nicks where str_nick='?'"<<str_nick(cc->params["nick_add"])));
        CH_id channel;
        CONTAINER(channel)=0;
        CONTAINER(channel)=atoi(cc->params["channel"]);
        add_moderator(uid,channel);
    }
    if (cc->params.count("login_add"))
    {
        MUTEX_INSPECTOR;
        DB_id uid;
        CONTAINER(uid)=0;
        CONTAINER(uid)=atoi(d.dbh->select_1Q((QUERY)"select id from tbl_users where login='?'"<<cc->params["login_add"]));
        CH_id channel;
        CONTAINER(channel)=0;
        CONTAINER(channel)=atoi(cc->params["channel"]);
        add_moderator(uid,channel);
    }
    map<DB_id,vector<CH_id> > mu=get_moderators();

    string out;
    for (map<DB_id,vector<CH_id> >::const_iterator i=mu.begin(); i!=mu.end(); i++)
    {
        MUTEX_INSPECTOR;
        string login;
        vector<string> chlist;
        if (CONTAINER(i->first)==0) continue;
        {
            PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(i->first)));
            login=u->login;
        }
        for (unsigned k=0; k<i->second.size(); k++)
        {
            if (chat_channels.count(i->second[k]))
            {
                __CH_LOCK(chat_channels[i->second[k]]);
                chlist.push_back(CH->cd.name2);
            }
        }
        string chans=join(",",chlist);
        map<string,string> q;
        q["~uid~"]=itostring(CONTAINER(i->first));
        q["~login~"]=login;
        q["~chans~"]=chans;
        out+=RVfiles(q,"admin/privileges/moder_row");
    }
    m["~rows~"]=out;

    vector<CH_id>chs=chat_channels._keys();

    if (!chs.size())
    {
        logErr2("if (!chs.size())");
        return;
    }
    CH_id ch=*chs.begin();
    map<CH_id,string>mc;
    for (unsigned int i=0; i<chs.size(); i++) {
        if (chat_channels.count(chs[i]))
        {
            __CH_LOCK(chat_channels[chs[i]]);
            mc[CH->cd.chid]=CH->cd.name2;

        }
        else
        {
            logErr2("C!if(chat_channels.count(chs[i]))");
        }

    }
    string sel=make_select("channel",mc,ch,"class=bsel");

    m["~channels~"]=sel;

    cc->html_out=cc->rvfiles(m,"admin/privileges/moder");

}
