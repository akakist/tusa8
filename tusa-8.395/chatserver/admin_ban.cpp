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
#include "message.h"
#include "chat_def.h"

#include <algorithm>
#include "web_server_data.h"
#include "file_set.h"
#include "copy_file.h"

#include "channels.h"
#include "server_config.h"
#include "app_def.h"
#include "admin.h"
#include "app_con.h"

#include "chat_globals.h"
#include "update_cmd.h"

#ifdef __WITH_LICENSE
#include "license.h"
#endif
#include "web_tools.h"
#include "global_state.h"
#include "chat_config.h"
string get_name_of_ban_time(time_t _tt);
int get_klap_ulevel(const DB_id&uid)
{
    PTR_getter<user_profile> __P=user$profiles.FindByID(CONTAINER(uid));
    if (__P)
    {
        PGLR(user_profile,u,__P);
        return u->level;
    }
    return 0;
}
string get_klap_uname(const DB_id&uid)
{
    string r;
    PTR_getter<user_profile> __P=user$profiles.FindByID(CONTAINER(uid));
    if (__P) {
        PGLR(user_profile,u,__P);
        r+=u->last_nick+" ";
    }
    if (__P) {
        PGLR(user_profile,u,__P);
        r+="("+u->login+")";
    }
    return r;
}
out_oscar_buffer & operator<< (out_oscar_buffer& b,const klap_item& s)
{



    b<<s.id;

    b<<s.uid;
    b<<s.descr;
    b<<s.time_ban_start;
    b<<s.time_ban_end;
    b<<s.time_cnt_str;
    b<<s.channel;
    b<<s.adm_uid;
    b<<s.adm_name;
    b<<s.user_name;
    b<<s.alevel;
    b<<s.ulevel;

    return b;
}
oscar_buffer & operator>> (oscar_buffer& b,  klap_item& s)
{


    b>>s.id;
    b>>s.uid;
    b>>s.descr;
    b>>s.time_ban_start;
    b>>s.time_ban_end;
    b>>s.time_cnt_str;
    b>>s.channel;
    b>>s.adm_uid;
    b>>s.adm_name;
    b>>s.user_name;
    b>>s.alevel;
    b>>s.ulevel;
    return b;

}

klap_stuff _klap_stuff;

void klap_stuff::clear()
{

    M_LOCK(Mutex);
    by_uid.clear();

    items.clear();

}

bool klap_stuff::get_item(const B_id &id, klap_item & ret)
{

    M_LOCK(Mutex);
    if (items.count(id)) {
        ret=items.find(id)->second;
        return true;
    }
    return false;

}
string /*success result*/ klap_stuff::remove_klap(const B_id &id)
{

    M_LOCK(Mutex);
    return remove_klap_NOLOCK(id);
}
string /*success result*/ klap_stuff::remove_klap_NOLOCK( const B_id &id)
{
    MUTEX_INSPECTOR;
    st_update stu;

    klap_item bi;
    {
        map<B_id,klap_item> ::const_iterator i=items.find(id);
        if (i==items.end())return "not_found";
        bi=i->second;
        items.erase(id);
        rebuild_NM();
    }
    {
        __send_update_d2(ban$remove$2,bi);
    }
    return "success";
}
string /*success result*/ remove_klap(const DB_id& adm_remover,  const B_id& id)
{
    MUTEX_INSPECTOR;
    klap_item it;
    st_dbh d;
    if (!_klap_stuff.get_item(id,it)) return "not_found";
    unsigned int a_lv=0,r_lv=0;
    if (CONTAINER(it.adm_uid)) {
        a_lv=atoi(d.dbh->select_1Q((QUERY)"select level from tbl_users where id='?'"<<CONTAINER(it.adm_uid)))/sub_levels;
    }
    if (CONTAINER(adm_remover))
    {
        r_lv=atoi(d.dbh->select_1Q((QUERY)"select level from tbl_users where id='?'"<<CONTAINER(adm_remover)))/sub_levels;
    }
    if (adm_remover!=it.adm_uid)
    {
        logErr2("uid %d adm_uid %d",CONTAINER(adm_remover),CONTAINER(it.adm_uid));
        if (r_lv<a_lv) return "too_low_level";
    }
    string ret=_klap_stuff.remove_klap(id);
    string ss=
        " id="+itostring(CONTAINER(it.id))+
        " uid="+itostring(CONTAINER(it.uid))+
        " descr="+it.descr+
        " time_ban_start="+date2mysql(it.time_ban_start)+
        " time_ban_end="+date2mysql(it.time_ban_end)+
        " time_cnt_str="+it.time_cnt_str+
        " channel="+itostring(CONTAINER(it.channel))+
        " adm_uid="+itostring(CONTAINER(it.adm_uid))+
        " adm_name="+it.adm_name+
        " user_name="+it.user_name+
        " alevel="+itostring(it.alevel)+
        " ulevel="+itostring(it.ulevel);
    logErrN("users","REMOVE_BAN %s",ss.c_str());

    return ret;
}

void	klap_stuff::rebuild_NM()
{
    MUTEX_INSPECTOR;

    by_uid.clear();
    set<B_id> rm;
    time_t t=time(NULL);
    for (map<B_id,klap_item>::const_iterator i=items.begin(); i!=items.end(); i++)
    {
        if (i->second.time_ban_end<t)
        {
            rm.insert(i->first);
            continue;
        }
        by_uid[i->second.channel][i->second.uid]=i->second.id;

    }
    for (set<B_id>::const_iterator i=rm.begin(); i!=rm.end(); i++)
    {
    }
}
bool ck_ban(const klap_item& item)
{
    if (time(NULL)>item.time_ban_end)
    {
        return false;
    }
    if (item.time_ban_start>=item.time_ban_end)
    {
        logErr2("item.time_ban>=item.time_ban_end %s %d",__FILE__,__LINE__);
        return false;
    }
    int uid=CONTAINER(item.uid);
    int adm_uid=CONTAINER(item.adm_uid);
    if (adm_uid<=0)
    {
        logErr2("adm_uid<=0 %s %d",__FILE__,__LINE__);
        return false;
    }
    if (uid<=0)
    {
        return false;
    }
    if (item.adm_name.size()==0)
    {
        logErr2("item.adm_name.size()==0 %s %d",__FILE__,__LINE__);
        return false;
    }
    if (item.user_name.size()==0)
    {
        return false;
    }
    if (item.alevel==0)
    {
        return false;
    }
    return true;
}

string klap_stuff::add__klap(const klap_item& item)
{
    MUTEX_INSPECTOR;
    if (!ck_ban(item)) return "error";
    {
        M_LOCK(Mutex);
        items[item.id]=item;
        rebuild_NM();
    }
    return "success";
}
string add_klap$$(const klap_item& item)
{
    MUTEX_INSPECTOR;
    st_update stu;
    if (!ck_ban(item)) return "error";

    {
        unsigned int a_lv=0,u_lv=0;
        PTR_getter<user_profile> AP=user$profiles.FindByID(CONTAINER(item.adm_uid));
        if (AP) {

            PGLR(user_profile,u,AP);
            a_lv=u->level/sub_levels;
        }

        if (CONTAINER(item.uid))
        {
            PTR_getter<user_profile> AU=user$profiles.FindByID(CONTAINER(item.uid));
            if (AU)
            {
                PGLR(user_profile,u,AU);
                {
                    u_lv=u->level/sub_levels;
                }
            }
        }
        if (a_lv<u_lv)
        {
            return "too low admin level";
        }
    }
    {
        __CH_LOCK(chat_channels[item.channel]);
        if (!CH->cd.moderators.count(item.adm_uid))
            return "you are not operator on channel "+itostring(CONTAINER(item.channel));
    }
    string ret=_klap_stuff.add__klap(item);

    string ss=
        " id="+itostring(CONTAINER(item.id))+
        " uid="+itostring(CONTAINER(item.uid))+
        " descr="+item.descr+
        " time_ban_start="+date2mysql(item.time_ban_start)+
        " time_ban_end="+date2mysql(item.time_ban_end)+
        " time_cnt_str="+item.time_cnt_str+
        " channel="+itostring(CONTAINER(item.channel))+
        " adm_uid="+itostring(CONTAINER(item.adm_uid))+
        " adm_name="+item.adm_name+
        " user_name="+item.user_name+
        " alevel="+itostring(item.alevel)+
        " ulevel="+itostring(item.ulevel);
    logErrN("users","ADD_BAN %s",ss.c_str());
    if (ret=="success")
    {
        __send_update_d2(ban$add$2,item);
    }
    return ret;
}

map<B_id ,klap_item>  klap_stuff::get_items()
{

    M_LOCK(Mutex);
    return items;
}



bool klap_stuff::check_ban_by_uid(const CH_id& ch,const DB_id& uid, klap_item& bi)
{
    MUTEX_INSPECTOR;
    M_LOCK(Mutex);
    map<CH_id,map<DB_id,B_id> >::const_iterator it=by_uid.find(ch);
    if (it==by_uid.end()) return false;

    map<DB_id,B_id>::const_iterator j=it->second.find(uid);
    if (j==it->second.end()) return false;
    B_id bid=j->second;
    bi=items[bid];

    if (bi.time_ban_end<time(NULL))
    {
        remove_klap_NOLOCK(bid);
        return false;
    }
    return true;
}


void url_admin_ban(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    LICENSE_CK(27);
    MUTEX_INSPECTOR;
    if (!__UU) {
        cc->html_out=cc->redirect();
        return;
    }
    st_dbh d;
    DB_id cc_user_id=user$id(__UU);
    user_pair __U(__UU);
    if (user$level(__U)/sub_levels<am_guard) {
        return;
    }
    map<string,string> m;
    m["~msg~"]="";

    if (cc->params["cmd"]=="remove")
    {

        B_id id;
        CONTAINER(id)=atoi(cc->params["id"]);
        {

            string ret=remove_klap(cc_user_id,  id);
            m["~msg~"]=ret;
        }
    }
    set<CH_id> channels;
    for (map<string,string>::const_iterator i=cc->params.begin(); i!=cc->params.end(); i++)
    {

        if (i->first.substr(0,5)=="chan_")
        {
            if (strupper(i->second)=="ON")
            {
                CH_id chan;
                CONTAINER(chan)=atoi(i->first.substr(5,i->first.size()-5));
                channels.insert(chan);

            }
        }
    }
    if (cc->params["addnick"].size()||cc->params["addlogin"].size())
    {
        DB_id uid;
        CONTAINER(uid)=0;
        if (cc->params["addnick"].size())
            CONTAINER(uid)=atoi(d.dbh->select_1Q((QUERY)"select uid from nicks where str_nick='?'"<<str_nick(cc->params["addnick"])));
        else
            CONTAINER(uid)=atoi(d.dbh->select_1Q((QUERY)"select id from tbl_users where login='?'"<<cc->params["addlogin"]));
        if (CONTAINER(uid))
        {
            int ulevel=0,alevel=user$level(__U);
            {
                PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(uid)));
                ulevel=u->level;
            }
            if (alevel/sub_levels>ulevel/sub_levels)
            {
                int tcnt=atoi(cc->params["how"]);
                string descr=remove_tags(cc->params["descr"]);
                for (set<CH_id>::const_iterator i=channels.begin(); i!=channels.end(); i++)
                {
                    klap_item bi;
                    bi.id=get_next_klap_id();
                    bi.uid=uid;
                    bi.descr=remove_tags(descr);
                    bi.setcurtime(tcnt);
                    bi.channel=*i;
                    bi.adm_uid=user$id(__U);
                    bi.user_name=get_klap_uname(uid);
                    bi.adm_name=get_klap_uname(user$id(__U));
                    bi.alevel=get_klap_ulevel(user$id(__U));
                    bi.ulevel=get_klap_ulevel(uid);
                    m["~msg~"]="Пользователь забанен";
                    add_klap$$(bi);
                }
            }
        }

    }
    else if (cc->params["addip"].size())
    {
        int timeban=atoi(cc->params["how"]);
        string descr=cc->params["descr"];
        for (set<CH_id>::const_iterator i=channels.begin(); i!=channels.end(); i++)
        {
            klap_item bi;
            bi.id=get_next_klap_id();
            CONTAINER(bi.uid)=0;
            bi.descr=remove_tags(descr);
            bi.setcurtime(timeban);
            bi.channel=*i;
            bi.adm_uid=user$id(__U);


            bi.adm_name=get_klap_uname(user$id(__U));
            bi.alevel=get_klap_ulevel(user$id(__U));


            m["~msg~"]="Пользователь забанен";
            add_klap$$(bi);
        }
    }

    map<B_id,klap_item> c=_klap_stuff.get_items();
    string data;
    int my_level=0;
    {
        PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(cc_user_id)));
        my_level=u->level;
    }
    for (map<B_id,klap_item>::const_iterator i=c.begin(); i!=c.end(); i++)
    {


        const klap_item &bi=i->second;
        if (!ck_ban(bi)) continue;
        map<string,string>q;
        q["~channel~"]=itostring(CONTAINER(bi.channel));
        q["~descr~"]=bi.descr;
        q["~time_ban_start~"]=date2mysql(bi.time_ban_start);
        q["~time_ban_end~"]=date2mysql(bi.time_ban_end);
        q["~time_cnt_str~"]=bi.time_cnt_str;

        if (CONTAINER(bi.adm_uid)==0)
        {
            logErr2("CONTAINER(bi.adm_uid)==0 || CONTAINER(bi.$uid)==0 %s",_DMI().c_str());
            continue;

        }
        q["~adm~"]=bi.adm_name;
        q["~login~"]=bi.user_name;
        q["~uid~"]=itostring(CONTAINER(bi.uid));

        if (my_level/sub_levels>=bi.alevel/sub_levels || bi.adm_uid==cc_user_id)
            q["~remove~"]="<input type=button value=\""+_MSG("removeban")+"\" onclick=\"b_remove('"+itostring(CONTAINER(bi.id))+"')\">";
        else
            continue;


        data+=RVfiles(q,"admin/ban/row");
    }
    m["~data~"]=data;
    vector<CH_id>chs=chat_channels._keys();//splitTOK(" ,",chat_config.channel_list.get());

    if (!chs.size())return;
    CH_id ch=*chs.begin();
    map<CH_id,string>mc;
    for (unsigned int i=0; i<chs.size(); i++) {

        if (!chat_channels.count(chs[i]))continue;
        __CH_LOCK(chat_channels[chs[i]]);
        if (CH->cd.moderators.count(cc_user_id))
            mc[CH->cd.chid]=CH->cd.name;

    }
    m["~cnt~"]=itostring(c.size());
    m["~channels~"]="";
    for (map<CH_id,string>::const_iterator i=mc.begin(); i!=mc.end(); i++)
    {
        m["~channels~"]+=(string)"<input type=checkbox name='chan_"+itostring(CONTAINER(i->first))+"' class=c_b >"+i->second+"<br>\n";
    }
    cc->html_out=cc->rvfiles(m,"admin/ban/ban");
}
void url_admin_ban_uid(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    LICENSE_CK(7);
    MUTEX_INSPECTOR;
    DB_id cc_user_id=user$id(__UU);
    DB_id zid;
    CONTAINER(zid)=0;
    map<string,string>m;
    m["~msg~"]="";
    for (map<string,string>::iterator i=cc->params.begin(); i!=cc->params.end(); i++)
    {
        logErr2("EE %s %s",i->first.c_str(),i->second.c_str());
    }
    CONTAINER(zid)=atoi(cc->params["id"]);
    logErr2("zid %d",CONTAINER(zid));
    if (cc->params.count("id")) m["~id~"]=cc->params["id"];
    string zlogin;
    {
        MUTEX_INSPECTOR;
        PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(zid)));
        zlogin=u->login;
    }
    bool ok=true;
    if (cc->params.count("timeban"))
    {
        MUTEX_INSPECTOR;
        set<CH_id> channels;
        for (map<string,string>::const_iterator i=cc->params.begin(); i!=cc->params.end(); i++)
        {
            MUTEX_INSPECTOR;
            if (i->first.size()>5 && i->first.substr(0,5)=="chan_")
            {
                MUTEX_INSPECTOR;
                if (strupper(i->second)=="ON")
                {
                    MUTEX_INSPECTOR;
                    CH_id chan;
                    CONTAINER(chan)=atoi(i->first.substr(5,i->first.size()-5));
                    channels.insert(chan);

                }
            }
        }

        time_t tcnt=atoi(cc->params["timeban"]);
        //bool by_ip=strupper(cc->params["by_ip"])=="ON";
        //bool by_login=strupper(cc->params["by_login"])=="ON";
        int ulevel=0,zlevel=0;
        {
            PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(zid)));
            zlevel=u->level;
        }
        {
            PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(cc_user_id)));
            ulevel=u->level;
        }
        if (ulevel/sub_levels<=zlevel/sub_levels)
        {
            m["~msg~"]="too low level";
            ok=false;
        }
        if (ok)
        {
            MUTEX_INSPECTOR;

            for (set<CH_id>::const_iterator ic=channels.begin(); ic!=channels.end(); ic++)
            {
                MUTEX_INSPECTOR;

                CH_id channel=*ic;
                PTR_getter<chat_channel> C=chat_channels.___get(channel);
                if (!C) throw cError("!C"+_DMI());
                {
                    MUTEX_INSPECTOR;

                    PGLR(chat_channel,c,C);
                    if (!c->cd.moderators.count(cc_user_id))
                        throw cError("not moderator for this channel"+_DMI());
                }
                if (ok)
                {
                    MUTEX_INSPECTOR;

                    klap_item bi;
                    bi.id=get_next_klap_id();

                    bi.descr=remove_tags(cc->params["descr"]);
                    bi.setcurtime(tcnt);
                    bi.channel=*ic;
                    bi.adm_uid=cc_user_id;

                    {
                        MUTEX_INSPECTOR;
                        bi.uid=zid;
                        bi.user_name=get_klap_uname(zid);
                        bi.adm_name=get_klap_uname(cc_user_id);
                        bi.alevel=get_klap_ulevel(cc_user_id);
                        bi.ulevel=get_klap_ulevel(zid);
                        add_klap$$(bi);
                        //kick_user(zid,bi.descr);
                        m["~msg~"]="Пользователь заткнут кляпом";
                    }
                }
            }
        }
    }
    m["~login~"]=zlogin;

    map<CH_id,string>mc;
    {
        MUTEX_INSPECTOR;
        vector<CH_id>chs=chat_channels._keys();

        if (!chs.size())return;
        CH_id ch=*chs.begin();

        for (unsigned int i=0; i<chs.size(); i++)
        {
            MUTEX_INSPECTOR;
            if (chat_channels.count(chs[i]))
            {
                MUTEX_INSPECTOR;
                __CH_LOCK(chat_channels[chs[i]]);
                if (CH->cd.moderators.count(cc_user_id))

                    mc[CH->cd.chid]=CH->cd.name2;
            }

        }
    }
    m["~channels~"]="";
    if (mc.size())
    {
        MUTEX_INSPECTOR;
        for (map<CH_id,string>::const_iterator i=mc.begin(); i!=mc.end(); i++)
        {
            MUTEX_INSPECTOR;
            m["~channels~"]+=(string)"<input type=checkbox name='chan_"+itostring(CONTAINER(i->first))+"' class=c_b>"+i->second+"<br>\n";
        }


        m["~sform~"]="";
        m["~eform~"]="";
    }
    else
    {
        MUTEX_INSPECTOR;
        m["~channels~"]="";
        m["~msg~"]=_MSG("not_moder");
        m["~sform~"]="<!--";
        m["~eform~"]="-->";
    }

    cc->html_out=cc->rvfiles(m,"admin/ban/uid");
}





B_id  klap_stuff::get_next_klap_id()
{
    B_id r;
    CONTAINER(r)=0;

    {
        M_LOCK(Mutex);
        for (map<B_id,klap_item>::iterator i=items.begin(); i!=items.end(); i++)
            if (CONTAINER(i->first)>CONTAINER(r)) r=i->first;

        CONTAINER(r)++;
        klap_item it;
        it.id=r;
        items[r]=it;
    }
    return r;
}
B_id get_next_klap_id()
{
    return _klap_stuff.get_next_klap_id();
}

void klap_item::pack(out_oscar_buffer& o)
{
    o<<*this;
}
void klap_item::unpack(oscar_buffer&o)
{
    o>>*this;
}

string get_name_of_ban_time(time_t _tt)
{
    time_t t=_tt;
    string r;
    while (1)
    {
        if (t>=24*7*3600)
        {
            time_t w=t/(24*7*3600);
            r+=itostring(w)+" ";

            if (w==1) r+=_MSG("weekS");
            else r+=_MSG("weekS");
            t-=w*(24*7*3600);
            r+=" ";
        }
        else if (t>=24*3600)
        {
            time_t d=t/(24*3600);
            r+=itostring(d)+" ";
            if (d==1) r+=_MSG("dayS");
            else r+=_MSG("dayS");
            t-=d*(24*3600);
            r+=" ";
        }
        else if (t>=3600)
        {
            time_t h=t/(3600);
            r+=itostring(h)+" ";
            if (h==1) r+=_MSG("hourS");
            else r+=_MSG("hourS");
            t-=h*(3600);
            r+=" ";

        }
        else if (t>=60)
        {
            time_t h=t/(60);
            r+=itostring(h)+" ";
            r+=_MSG("minS");
            t-=h*(60);
            r+=" ";
        }
        else break;
    }
    return r;
}
