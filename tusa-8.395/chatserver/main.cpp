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


void url_change_contact_mode(const PTR_getter<chat_user> &__UU,app_connection*cc)
{
    MUTEX_INSPECTOR;
    st_update stu;
    user_pair __U(__UU);
    LICENSE_CK(12);

    bool is_on;
    {
        PGLW(user_profile,u,__U.up);
        u->ul_mode_contacts_only=!u->ul_mode_contacts_only;
        is_on=u->ul_mode_contacts_only;
    }
    DB_id uid=user$id(__UU);
    out_oscar_buffer o;
    o<<'C'<<is_on;
    __set_user_state(user_state::CHANGE_CONTACT_MODE,uid,o);
    chat_event_UPDATE_CONTACT_LIST *e=new chat_event_UPDATE_CONTACT_LIST(__UU,T_WWW);
    LEAKCTL_ADD(e);
    send_chat_event_push_back(e);

    __send_update_d3(user_profile$ul_mode_contacts_only$3,CONTAINER(uid),is_on);

    if (is_on)
    {
        send_syspriv_msg(__U,"Режим контакт листа включен");
        data_push_back_www(__U,"<SCRIPT>parent.set_state_contact_mode(1);</SCRIPT>\n");

    }
    else
    {
        send_syspriv_msg(__U,"Режим контакт листа выключен");
        data_push_back_www(__U,"<SCRIPT>parent.set_state_contact_mode(0);</SCRIPT>\n");
    }

    cc->html_out="POSRALL";
}

void url_change_female_mode(const PTR_getter<chat_user> &__UU,app_connection*cc)
{
    MUTEX_INSPECTOR;
    st_update stu;
    LICENSE_CK(12);

    user_pair __U(__UU);
    bool is_on;
    {
        PGLW(user_profile,u,__U.up);
        u->ul_mode_hide_female=!u->ul_mode_hide_female;
        is_on=u->ul_mode_hide_female;
    }
    DB_id uid=user$id(__UU);
    out_oscar_buffer o;
    o<<'F'<<is_on;
    __set_user_state(user_state::CHANGE_CONTACT_MODE,uid,o);
    chat_event_UPDATE_CONTACT_LIST *e=new chat_event_UPDATE_CONTACT_LIST(__UU,T_WWW);
    LEAKCTL_ADD(e);
    send_chat_event_push_back(e);

    __send_update_d3(user_profile$ul_mode_hide_female$3,CONTAINER(user$id(__UU)),is_on);

    if (is_on)
    {
        send_syspriv_msg(__U,_MSG("hide_female_enabled"));
        data_push_back_www(__U,"<SCRIPT>parent.set_state_hide_female(1);</SCRIPT>\n");

    }
    else
    {
        send_syspriv_msg(__U,_MSG("hide_female_disabled"));
        data_push_back_www(__U,"<SCRIPT>parent.set_state_hide_female(0);</SCRIPT>\n");
    }

    cc->html_out="POSRALL";
}
void url_change_male_mode(const PTR_getter<chat_user> &__UU,app_connection*cc)
{
    MUTEX_INSPECTOR;
    st_update stu;
    LICENSE_CK(12);

    user_pair __U(__UU);
    bool is_on;
    {
        PGLW(user_profile,u,__U.up);
        u->ul_mode_hide_male=!u->ul_mode_hide_male;
        is_on=u->ul_mode_hide_male;
    }
    DB_id uid=user$id(__UU);
    out_oscar_buffer o;
    o<<'M'<<is_on;
    __set_user_state(user_state::CHANGE_CONTACT_MODE,uid,o);
    chat_event_UPDATE_CONTACT_LIST *e=new chat_event_UPDATE_CONTACT_LIST(__UU,T_WWW);
    LEAKCTL_ADD(e);
    send_chat_event_push_back(e);

    __send_update_d3(user_profile$ul_mode_hide_male$3,CONTAINER(user$id(__UU)),is_on);

    if (is_on)
    {
        send_syspriv_msg(__U,_MSG("hide_male_enabled"));
        data_push_back_www(__U,"<SCRIPT>parent.set_state_hide_male(1);</SCRIPT>\n");

    }
    else
    {
        send_syspriv_msg(__U,_MSG("hide_male_disabled"));
        data_push_back_www(__U,"<SCRIPT>parent.set_state_hide_male(0);</SCRIPT>\n");
    }

    cc->html_out="POSRALL";
}





bool do_change_mode(PTR_getter<chat_channel> &ML,const PTR_getter<chat_user> &__UU,int mode)
{
    MUTEX_INSPECTOR;
    if (!__UU) return false;
    user_pair __U(__UU);

    CH_id uch;
    CONTAINER(uch)=0;
    bool ext;
    int ulvl;
    {
        __CH_LOCK(ML);
        uch=CH->cd.chid;
        ext=CH->cd.extended;
    }
    DB_id uid=user$id(__UU);
    ulvl=user$level(__U);
    if (ulvl/sub_levels<am_admin) {
        send_syspriv_msg(__U,"У Вас слишком маленький уровень!");
        return false;
    }
    if (!ext) {
        send_syspriv_msg(__U,_MSG("cantchangechannelmode"));
        return false;
    }
    unsigned int nid=get_cnick_id(__U);
    chat_event_CHMODE *ev=new chat_event_CHMODE(get_nick(user$id(__U),nid),uch,uid,mode);
    LEAKCTL_ADD(ev);
    send_chat_event_push_back(ev);
    return true;
}

bool do_change_voice(const CH_id& channel,const DB_id& uid,const DB_id& zid, int nvoice)
{
    MUTEX_INSPECTOR;
    chat_event_CHVOICE *ev=new chat_event_CHVOICE(zid,channel,uid,nvoice);
    LEAKCTL_ADD(ev);
    send_chat_event_push_back(ev);
    return true;
}

void url_invite_user(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    MUTEX_INSPECTOR;
    if (!__UU) {
        return;
    }
    LICENSE_CK(9);

    user_pair __U(__UU);
    DB_id zid;
    CONTAINER(zid)=0;
    CONTAINER(zid)=atoi(cc->params["id"]);
    logErr2("zid %d",CONTAINER(zid));
    chat_event_INVITE *event=new chat_event_INVITE(user$id(__UU),zid,unescape_rus(cc->params["msg"]));
    LEAKCTL_ADD(event);
    send_chat_event_push_back(event);
}




void kick_user(const DB_id& zid,const string &msgs);
int do_nickban(const string &nick_orig,const DB_id& adm_uid,unsigned int adm_level);
int do_old_loginban(const DB_id &zuid,const string &descr,time_t t,const DB_id& adm_uid,unsigned int adm_level);
int do_old_ipban(const string &ip_orig,time_t t,const string& za,const DB_id& adm_uid);

void url_klap_user(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    MUTEX_INSPECTOR;
    LICENSE_CK(11);
    user_pair __U(__UU);
    int alevel=user$level(__U);
    DB_id zid;
    CONTAINER(zid)=atoi(cc->params["zid"]);
    string za=unescape_rus(cc->params["za"]);
    int t=atoi(cc->params["time"]);
    CH_id channel;
    CONTAINER(channel)=atoi(cc->params["channel"]);
    st_dbh d;
    vector<PTR_getter<chat_user> > vu=local_users.get$(zid);
    if (vu.size()==0)
    {
        send_syspriv_msg(__U,"Пользователь не найден");
        return;
    }
    DB_id uid=user$id(__U);
    string chname;
    bool moder=false;
    {
        __CH_LOCK(chat_channels[channel]);
        chname=CH->cd.name2;
        if (CH->cd.moderators.count(uid))
            moder=true;

    }
    if (!moder)
    {
        send_syspriv_msg(__U,"Вы не являетесь модератором канала "+chname);
        return;
    }
    int ulevel;
    {
        PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(zid)));
        ulevel=u->level;
    }
    int kick_abl=__LEVELS(alevel).kick_ability;
    int kick_sh=__LEVELS(ulevel).kick_shield;
    if (kick_abl<=kick_sh)
    {
        send_syspriv_msg(__U,"Недостаточно прав на бан");
        return;
    }
    klap_item bi;
    bi.id=get_next_klap_id();
    bi.adm_uid=uid;
    bi.channel=channel;
    bi.setcurtime(t);
    bi.uid=zid;
    bi.descr=za;
    bi.alevel=get_klap_ulevel(bi.adm_uid);
    bi.ulevel=get_klap_ulevel(bi.uid);
    bi.adm_name=get_klap_uname(bi.adm_uid);
    bi.user_name=get_klap_uname(bi.uid);

    add_klap$$(bi);

    send_syspriv_msg(__U,"Пользователю "+bi.user_name+" наложен кляп на канал "+chname);

}

void url_ip_ban(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    MUTEX_INSPECTOR;
    LICENSE_CK(11);
    user_pair __U(__UU);
    int alevel=user$level(__U);
    DB_id zid;
    CONTAINER(zid)=atoi(cc->params["zid"]);
    string za=unescape_rus(cc->params["za"]);
    int t=atoi(cc->params["time"]);
    st_dbh d;
    vector<PTR_getter<chat_user> > vu=local_users.get$(zid);
    if (vu.size()==0)
    {
        send_syspriv_msg(__U,"Пользователь не найден");
        return;
    }
    string ip=d.dbh->select_1Q((QUERY)"select stat_last_ip from tbl_users where id='?'"<<CONTAINER(zid));


    if (alevel<800)
    {
        send_syspriv_msg(__U,"У вас слишком маленький уровень");
        return;
    }
    DB_id uid=user$id(__U);
    int ulevel;
    {
        PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(zid)));
        ulevel=u->level;
    }
    int kick_abl=__LEVELS(alevel).kick_ability;
    int kick_sh=__LEVELS(ulevel).kick_shield;
    if (kick_abl<=kick_sh)
    {
        send_syspriv_msg(__U,"Недостаточно прав на бан");
        return;
    }
    int r=do_old_ipban(ip,t,za,uid);
    if (r==0)
        send_syspriv_msg(__U,"IP "+ip+" забанен");
    else
        send_syspriv_msg(__U,"IP "+ip+" не забанен, причина непонятна");
    kick_user(zid,za);
}

void url_login_ban(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    MUTEX_INSPECTOR;
    LICENSE_CK(11);
    user_pair __U(__UU);
    int alevel=user$level(__U);
    DB_id zid;
    CONTAINER(zid)=atoi(cc->params["zid"]);
    string za=unescape_rus(cc->params["za"]);
    int t=atoi(cc->params["time"]);
    logErr2("ban t %d",t);
    st_dbh d;
    //int lastnickid=atoi(d.dbh->select_1Q((QUERY)"select last_nick_id from tbl_users where id='?'"<<CONTAINER(zid)));
    string login=d.dbh->select_1Q((QUERY)"select login from tbl_users where id='?'"<<CONTAINER(zid));
    if (alevel<700)
    {
        send_syspriv_msg(__U,"У вас слишком маленький уровень");
        return;
    }
    DB_id uid=user$id(__U);
    int ulevel;
    {
        PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(zid)));
        ulevel=u->level;
    }
    int kick_abl=__LEVELS(alevel).kick_ability;
    int kick_sh=__LEVELS(ulevel).kick_shield;
    if (kick_abl<=kick_sh)
    {
        send_syspriv_msg(__U,"Недостаточно прав на бан");
        return;
    }
    int r=do_old_loginban(zid,za,t,uid,alevel);
    if (r==0)
        send_syspriv_msg(__U,"Логин "+login+" забанен");
    else
        send_syspriv_msg(__U,"Ник "+login+" не забанен, причина непонятна");
    kick_user(zid,za);
}

void url_nick_ban(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    MUTEX_INSPECTOR;
    LICENSE_CK(11);
    user_pair __U(__UU);
    int alevel=user$level(__U);
    DB_id zid;
    CONTAINER(zid)=atoi(cc->params["zid"]);
    st_dbh d;
    string lastnick=d.dbh->select_1Q((QUERY)"select last_nick from tbl_users where id='?'"<<CONTAINER(zid));
    if (alevel<700)
    {
        send_syspriv_msg(__U,"У вас слишком маленький уровень");
        return;
    }
    DB_id uid=user$id(__U);
    int ulevel;
    {
        PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(zid)));
        ulevel=u->level;
    }
    int kick_abl=__LEVELS(alevel).kick_ability;
    int kick_sh=__LEVELS(ulevel).kick_shield;
    if (kick_abl<=kick_sh)
    {
        send_syspriv_msg(__U,"Недостаточно прав на бан");
        return;
    }
    int r=do_nickban(lastnick,uid,alevel);
    if (r==0)
        send_syspriv_msg(__U,"Ник "+lastnick+" забанен");
    else
        send_syspriv_msg(__U,"Ник "+lastnick+" не забанен, причина непонятна");


}
void url_kick_user(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    MUTEX_INSPECTOR;
    LICENSE_CK(11);
    user_pair __U(__UU);
    int alevel=user$level(__U);
    DB_id zid;
    CONTAINER(zid)=atoi(cc->params["zid"]);
    if (alevel<600)
    {
        send_syspriv_msg(__U,"У вас слишком маленький уровень");
        return;
    }
    DB_id uid=user$id(__U);
    int ulevel;
    {
        PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(zid)));
        ulevel=u->level;
    }
    int kick_abl=__LEVELS(alevel).kick_ability;
    int kick_sh=__LEVELS(ulevel).kick_shield;
    if (kick_abl<=kick_sh)
    {
        send_syspriv_msg(__U,"Недостаточно прав на кик");
        return;
    }
    kick_user(zid,unescape_rus(cc->params["msg"]));

    return;
}


void kick_user(const DB_id& uid,const string &msgs)
{

    out_oscar_buffer o;
    o<<msgs;
    __set_user_state(user_state::KICK_USER,uid,o);
}
PTR_getter<chat_user> get_chat_user(const DB_id &uid);


int cnt=0;



void url_ainf_zzz(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    chat_event_ainf *e=new chat_event_ainf(cc->socket) ;
    LEAKCTL_ADD(e);
    send_chat_event_push_back(e);
    cc->allow_build_response=false;
}
void url_set_topic(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    user_pair __U(__UU);
    st_update stu;
    if (!user__privileges(__U,"$topic")&& user$level(__U)/sub_levels<am_vip)
    {
        cc->html_out="\r\n\r\n Topic not changed.";
        return;
    }
    string top=unescape_rus(cc->params["msg"]);
    if (top.size()>128) top=top.substr(0,128);
    top=str_replace("'","",top);
    top=str_replace("\\\\","",top);


    DB_id cc_user_id;
    CONTAINER(cc_user_id)=0;
    if (__UU)cc_user_id=user$id(__UU);

    CH_id channel;
    CONTAINER(channel)=0;
    set<CH_id> channels;
    channels=global_state.get_user_channel(T_WWW,cc_user_id);
    if (channels.size()!=1)throw cError("channels.size()>1 "+_DMI());
    channel=*channels.begin();
    string cnick;
    {
        PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(cc_user_id)));
        cnick=u->last_nick;
    }
    __send_update_d5(channel$topic$5,channel,top,cnick,time(NULL));
    chat_event_CHTOPIC *e=new chat_event_CHTOPIC(channel,cc_user_id,top);
    LEAKCTL_ADD(e);
    send_chat_event_push_back(e);

}
void url_remove_nick(const PTR_getter<chat_user> &__UU,app_connection*cc)
{
    MUTEX_INSPECTOR;

    LICENSE_CK(9);
    st_update stu;
    if (!__UU) return;
    user_pair __U(__UU);
    DB_id id=user$id(__UU);
    string login=user$login(__U);
    unsigned int nick_id=atoi(cc->params["nick"]);
//	logErr2("rem nick %d",nick_id);
    if (user$nicks_size(user$id(__UU))>1) {
        string unregnick;
        {
            PGLW(nick_vec,n,nick_vecs.find_by_id_create(CONTAINER(id)));
            if (n->u_nicks.count(nick_id))
            {
                unregnick=n->u_nicks[nick_id].name;
                n->u_nicks.erase(nick_id);
                __send_update_d3(nick$remove$3,CONTAINER(id),nick_id);
            }
        }
    }
    cc->redirect(RVfiles("url_login_redirect"));
}
void url_chat_exit(const PTR_getter<chat_user> &__U,app_connection *cc)
{
    MUTEX_INSPECTOR;

    if (!__U) {
        return;
    }
    logErr2("us_PART_CHANNEL(T_WWW,%d) %s %d",CONTAINER(user$id(__U)),__FILE__,__LINE__);
    us_PART_CHANNEL(T_WWW,user$id(__U));
    cc->redirect(RVfiles("url_login_redirect"));
}
