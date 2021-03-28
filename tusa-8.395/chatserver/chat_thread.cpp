#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <string>
#include <set>
using namespace std;
#include "mutex_inspector.h"
#include "oscar_buffer.h"
#include "PTR.h"
#include "DB_id.h"
#include "CH_id.h"
#include "S_id.h"
#include "user_nick.h"
#include "chat_msg.h"
#include "M_DEQUE.h"
#include "user_status.h"
#include "user_nick.h"
#include "user_state.h"
#include "levels.h"
#include "chat_stuff.h"
#include "message.h"
#include "channels.h"
#include "update_cmd.h"

#include "cutils.h"
#include "chat_def.h"
#include "server_config.h"

#include "chat_thread_event.h"
#include "irc_utils.h"
#include "irc_server_config.h"
#include "chat_globals.h"
#include "user_profile.h"
#include "html_out.h"
#include "chat_config.h"
#include "global_state.h"
#include "contact_vec.h"
#include "ignore_vec.h"
#include "admin.h"
#include "chat_colors.h"
extern m_var<string> SERVERNAME;
M_DEQUE<PTR_getter_nm<chat_event> > __chat_events;

M_DEQUE<pair<string,string> > messages_to_backup;
M_DEQUE<PTR_getter_nm<chat_msg> > external_$messages;

void local_free_handle_chatthread()
{
    __chat_events.clear();

    messages_to_backup.clear();

}
size_t __chat_events_size()
{
    return __chat_events.size();
}


void pack_backup_messages(chat_thread_stuff& stuff);
size_t messages_to_backup_size()
{
    return messages_to_backup.size();
}



void get_irc_list(chat_thread_stuff &stuff);


void send_chat_event_push_back(chat_event*e)
{
    __chat_events.push_back(e);
}



void chat_processor::start(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m)
{
    load_config(pn,prefix,m);
    create_thread(PTHR_handler," handler");
}
void *chat_processor::PTHR_handler(void *pr)
{
    MUTEX_INSPECTOR;

    chat_processor *bcid=(chat_processor*)pr;
    if (!bcid) {
        logErr2("KALLsrall %s %d",__FILE__,__LINE__);
        return NULL;
    }
    while (1)
    {


        try {
            bcid->handler();
            return NULL;

        }
        STDCATCHS("chat_processor::PTHR_receiver");
    }


    return NULL;
}

void handle_chat_events(chat_thread_stuff &stuff)
{
    MUTEX_INSPECTOR;
    deque<PTR_getter_nm<chat_event> > de=__chat_events.extract_all();
    for (unsigned int i=0; i<de.size(); i++)
    {
        if (g_must_terminate)return ;
        try {
            switch (de[i].___ptr->type)
            {
            case chat_event::INVITE:
            {

                chat_event_INVITE *e=(chat_event_INVITE*)de[i].___ptr;
                stuff.on_INVITE(e);
                break;
            }


            case chat_event::UPDATE_CONTACT_LIST:
            {

                chat_event_UPDATE_CONTACT_LIST *e=(chat_event_UPDATE_CONTACT_LIST*)de[i].___ptr;
                stuff.on_UPDATE_CONTACT_LIST(e);
            }
            break;

            case chat_event::CHAT_MESSAGES2:
            {

                chat_event_CHAT_MESSAGES2* e=(chat_event_CHAT_MESSAGES2*)de[i].___ptr;
                stuff.on_CHAT_MESSAGES2(e);
            }
            break;

            case chat_event::SEND_MESSAGE:
            {

                chat_event_SEND_MESSAGE *e=(chat_event_SEND_MESSAGE*)de[i].___ptr;
                stuff.on_SEND_MESSAGE(e);
            }
            break;

            case chat_event::SET_user_state:
            {

                chat_event_SET_user_state *e=(chat_event_SET_user_state*)de[i].___ptr;
                stuff.on_SET_user_state(e);
            }
            break;

            case chat_event::SEND_DATA_TO_WWW_USERS:
            {

                stuff.on_SEND_DATA_TO_WWW_USERS((chat_event_SEND_DATA_TO_WWW_USERS*)de[i].___ptr);
            }
            break;

            case chat_event::CHMODE:
            {

                chat_event_CHMODE *e=(chat_event_CHMODE*)de[i].___ptr;
                stuff.on_CHMODE(e);
            }
            break;

            case chat_event::CHVOICE:
            {

                chat_event_CHVOICE *e=(chat_event_CHVOICE*)de[i].___ptr;
                stuff.on_CHVOICE(e);
            }
            break;

            case chat_event::CHTOPIC:
            {

                stuff.on_CHTOPIC((chat_event_CHTOPIC*)de[i].___ptr);
            }
            break;

            case chat_event::UPDATE_DATA:
            {

                stuff.on_UPDATE_DATA((chat_event_UPDATE_DATA*)de[i].___ptr);
            }
            break;
            case chat_event::FLUSH_USER_DATA:
            {
                chat_event_FLUSH_USER_DATA *e=(chat_event_FLUSH_USER_DATA*) de[i].___ptr;
                stuff.unload_user(e->uid);
            }
            break;
            case chat_event::CHROOM:
            {
                chat_event_CHROOM *e=(chat_event_CHROOM*) de[i].___ptr;
                map<CH_id,chat_channel_data> ::const_iterator  i=stuff.__channels$.find(e->newchan);
                if (i==stuff.__channels$.end()) {
                    throw cError("if(i==__channels$.end())" +_DMI());
                }
                const chat_channel_data &cs=i->second;
                PTR_getter_nm<chat_stuff_user_p> __pu=stuff.get_user_p(e->uid);
                stuff.chroom_msgs_channels.push_back(make_pair(e->oldchan,make_pair(__pu,cs.name)));

            }
            break;

            case chat_event::irc_cmd_ISON:
            {

                chat_event_irc_cmd_ISON* e=(chat_event_irc_cmd_ISON*)de[i].___ptr;
                stuff.on_irc_cmd_ISON(e);
            }
            break;

            case chat_event::irc_cmd_WHOIS:
            {

                chat_event_irc_cmd_WHOIS *e=(chat_event_irc_cmd_WHOIS*)de[i].___ptr;
                stuff.on_irc_cmd_WHOIS(e);
            }
            break;

            case chat_event::irc_cmd_JOIN:
            {

                chat_event_common_JOIN *e=(chat_event_common_JOIN*)de[i].___ptr;
                stuff.on_common_JOIN(e);
            }
            break;

            case chat_event::AINF:
            {

                chat_event_ainf *e=(chat_event_ainf *)de[i].___ptr;
                stuff.on_ainf(e);
            }
            break;
            default:
                logErr2("Undefined chat_event %d",de[i].___ptr->type)    ;
            }
        }
        catch (cError e)
        {
            logErr2("catched in chat_processor %s",e.what());

        }
        catch (...)
        {
            logErr2("catched in chat_processor");
        }

    }

}
void chat_processor::handler()
{
    MUTEX_INSPECTOR;
    log_start_thread("chat processor");
    chat_thread_stuff stuff(this);
    time_t last_t_cnt_recalc=time(NULL);
    time_t user_message_count_clear=time(NULL);
    stuff.__channels_keys$=chat_channels._keys();

    map<unsigned int, c_level> ml=LEVELS_GET();
    for (map<unsigned int, c_level>::iterator i=ml.begin(); i!=ml.end(); i++)
    {
        MUTEX_INSPECTOR;
        stuff.level$[i->first]=new c_level(i->second);
    }

    for (unsigned i=0; i<stuff.__channels_keys$.size(); i++)
    {
        MUTEX_INSPECTOR;
        CH_id ch=stuff.__channels_keys$[i];
        __CH_LOCK(chat_channels[ch]);
        chat_channel_data cd=CH->cd;
        stuff.__channels$[cd.chid]=cd;
    }
    try {
        MUTEX_INSPECTOR;
        stuff.to_all=chat_config.to_all;
        stuff.make_url_users_full_to_file();
        stuff.get_irc_list();
        stuff.last_send_rc=time(NULL);
        stuff.last_sync_time=time(NULL);
        stuff.last_make_url_users_content=time(NULL);
        stuff.last_clean_local_clients=time(NULL);
        stuff.update_contact_list_data_create_time=0;
    }
    STDCATCHS("chat_processor::handler__1");

    while (1) {
        MUTEX_INSPECTOR;
        if (g_must_terminate)return;
        try {
            MUTEX_INSPECTOR;
            if (external_$messages.size())
            {
                deque<PTR_getter_nm<chat_msg> > d=external_$messages.get_container();
                external_$messages.clear();
                for (unsigned i=0; i<d.size(); i++)
                {
                    stuff._$messages.push_back(d[i]);
                }

            }

            handle_chat_events(stuff);
            try {
                MUTEX_INSPECTOR;
                if (__chat_events.size()==0)
                {
                    MUTEX_INSPECTOR;

                    stuff.handle_room_event_messages();
                }

                if (__chat_events.size()==0 && stuff.need_send_rc && time(NULL)-stuff.last_send_rc>5)
                {
                    MUTEX_INSPECTOR;
                    stuff.need_make_url_users_content =true;
                    stuff.last_send_rc=time(NULL);
                    stuff.need_send_rc=false;
                    string rc=stuff.LK_get_users$count_for_web();

                    for (map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> >::const_iterator i=stuff.local_clients.begin(); i!=stuff.local_clients.end(); i++)
                    {
                        MUTEX_INSPECTOR;
                        client_instance &ci=i->second.___ptr->ci;
                        if (i->first.second==T_WWW )
                        {
                            data_push_back_www(ci.__U,rc);
                        }

                    }
                }
                if (__chat_events.size()==0 && stuff.need_make_url_users_content && time(NULL)-stuff.last_make_url_users_content>30)
                {
                    MUTEX_INSPECTOR;
                    stuff.last_make_url_users_content=time(NULL);
                    stuff.need_make_url_users_content =false;
                    stuff.make_url_users_full_to_file();
                    stuff.get_irc_list();
                }
                if (__chat_events.size()==0 && time(NULL)-stuff.last_clean_local_clients>200)
                {
                    MUTEX_INSPECTOR;
                    stuff.last_clean_local_clients=time(NULL);

                    set<pair<DB_id,CT> > inchat,notinchat;
                    for (map<CH_id,map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> >  >	::const_iterator i=stuff.__local_users_in_channels$.begin(); i!=stuff.__local_users_in_channels$.end(); i++)
                    {
                        MUTEX_INSPECTOR;
                        for (map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> >::const_iterator j=i->second.begin(); j!=i->second.end(); j++)
                        {
                            MUTEX_INSPECTOR;
                            inchat.insert(j->first);
                        }
                    }
                    for (map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> >::const_iterator i=stuff.local_clients.begin(); i!=stuff.local_clients.end(); i++)
                    {
                        MUTEX_INSPECTOR;
                        if (!inchat.count(i->first)) notinchat.insert(i->first);
                        if (!local_users.get$(i->first.first,i->first.second))
                            notinchat.insert(i->first);

                    }
                    for (set<pair<DB_id,CT> >::const_iterator i=notinchat.begin(); i!=notinchat.end(); i++)
                    {
                        MUTEX_INSPECTOR;
                        stuff.local_clients.erase(*i);
                    }


                }

                if (__chat_events.size()==0 || (stuff.update_contact_list_data_create_time&&time(NULL)-stuff.update_contact_list_data_create_time>10))
                {
                    MUTEX_INSPECTOR;
                    stuff.__update_contact_list_all();
                    stuff.update_contact_list_data.clear();

                    stuff.update_contact_list_data_create_time=0;

                }
                if (time(NULL)-user_message_count_clear>3600)
                {
                    user_message_count_clear=time(NULL);
                    st_update stu;
                    for (map<DB_id,int>::iterator i=stuff.user_message_count.begin(); i!=stuff.user_message_count.end(); i++)
                    {
                        __send_update_d3(user_profile$inc_stat_m_count$3,CONTAINER(i->first),i->second);
                    }
                    stuff.user_message_count.clear();
                }


            }
            STDCATCHS("chat_processor::handler 2");


            pack_backup_messages(stuff);

            if (stuff._$messages.size()>15000)
            {

                while (stuff._$messages.size()>10000)
                {

                    stuff._$messages.pop_front();
                }
            }
            if (time(NULL)-last_t_cnt_recalc>600)
            {

                st_update stu;
                time_t tt=time(NULL);
                last_t_cnt_recalc=tt;
                for (map<DB_id,time_t>::iterator i=stuff.join_time.begin(); i!=stuff.join_time.end(); i++)
                {
                    if (tt-i->second>100)
                    {
                        __send_update_d3(user_profile$add_stat_t_count$3,CONTAINER(i->first),tt-i->second);
                        i->second=tt;
                    }
                }

                vector<DB_id> todel;
                for (map<DB_id,time_t> ::const_iterator i=stuff.trash.begin(); i!=stuff.trash.end(); i++)
                {
                    if (tt-i->second>3600*3) todel.push_back(i->first);
                }
                for (unsigned i=0; i<todel.size(); i++)
                {
                    stuff.trash.erase(todel[i]);
                    stuff.unload_user(todel[i]);
                }
            }

            {
                usleep(11111);
            }
        }
        catch (cError e)
        {
            logErr2("--Error: cha_processor: error! %s %d %s",__FILE__,__LINE__,e.what());

        }
        catch (...)
        {
            logErr2("--Error: cha_processor: unknow error! %s %d",__FILE__,__LINE__);
        }
    }
    return ;
}

void pack_backup_messages(chat_thread_stuff& stuff)
{
    MUTEX_INSPECTOR;
    if (stuff.backup_messages.size())
    {
        out_oscar_buffer o;
        string o2;
        for (deque<PTR_getter_nm<chat_msg> > ::const_iterator i=stuff.backup_messages.begin(); i!=stuff.backup_messages.end(); i++)
        {
            chat_msg$ *m=i->___ptr->data;
            o<<m->msgtype;
            m->pack(o);
            o2+=m->make_backup(stuff);
        }
        messages_to_backup.push_back(make_pair(o.as_string(),o2));
        stuff.backup_messages.clear();
    }
}


bool chat_stuff_user::I_can_see_contact_list(const chat_stuff_user&__z) const
{
    MUTEX_INSPECTOR;
    if (dbid==__z.dbid)
    {
        return true;
    }
    bool i_can_see=true;



    if (__z.cstate.status.id==st_offinvisible ||__z.cstate.status.id==st_invisible)
    {
        if (__z._invisibility>_invisibility)
        {
            return false;
        }
    }
    if (ul_mode_contacts_only)
    {
        if (contacts.count(__z.dbid)) return true;
    }
    if (ul_mode_hide_male)
    {
        if (__z.gender!=2)return false;
    }
    if (ul_mode_hide_female)
    {
        if (__z.gender!=1)return false;
    }

    return true;
}

bool chat_stuff_user::I_can_see_other(const chat_stuff_user&__z) const
{

    if (dbid==__z.dbid) return true;
    bool i_can_see=true;


    if (__z.cstate.status.id==st_offinvisible ||__z.cstate.status.id==st_invisible)
    {
        if (__z._invisibility>_invisibility) return false;
    }
    return true;
}



#define I_UID     DB_id uid;CONTAINER(uid)=0;    o>>uid;\
	map<DB_id,PTR_getter_nm<chat_stuff_user_p> >::const_iterator it=__users$.find(uid);\
	if(it==__users$.end()) return;\
	chat_stuff_user &__u=it->second.___ptr->ud;

void chat_thread_stuff::on_UPDATE_DATA(chat_event_UPDATE_DATA *e)
{

    oscar_buffer o(e->buf.data(),e->buf.size());
    int cmd;
    o>>cmd;



    switch (cmd)
    {


    case ban$remove$2: {
        klap_item bit;
        o>>bit;
        {
            map<CH_id,chat_channel_data>::iterator it=__channels$.find(bit.channel);
            if (it!=__channels$.end())
            {
//                if (bit.type==klap_item::BAN_BOTH || bit.type==klap_item::BAN_IP)
//                    it->second.banned_ips.erase(bit.ip);
//                if (bit.type==klap_item::BAN_BOTH || bit.type==klap_item::BAN_UID)
                it->second.klaped_logins_stuff.erase(bit.uid);
            }

        }


    }
    break;
    case ban$add$2: {
        klap_item bit;
        o>>bit;
        {
            map<CH_id,chat_channel_data>::iterator it=__channels$.find(bit.channel);
            if (it!=__channels$.end())
            {
                chat_channel_data::klap k;
                k.time_ban_end=bit.time_ban_end;
                k.time_cnt_str=bit.time_cnt_str;
                k.za=bit.descr;
                it->second.klaped_logins_stuff[bit.uid]=k;
            }

        }

    }
    break;

    case channel$update$3:
    {
        chat_channel_data cd;
        CH_id channel;
        CONTAINER(channel)=0;
        out_oscar_buffer b;
        o>>cd>>channel;
        map<CH_id,chat_channel_data>::iterator it=__channels$.find(channel);
        if (it!=__channels$.end())
        {
            it->second=cd;
        }

    }
    break;
    case moderator$add$3:
    {

        DB_id uid;
        CONTAINER(uid)=0;
        CH_id channel;
        CONTAINER(channel)=0;
        o>>uid>>channel;
        map<CH_id,chat_channel_data>::iterator it=__channels$.find(channel);
        if (it!=__channels$.end())
        {
            it->second.moderators.insert(uid);
        }

    }
    break;
    case moderator$remove$2:
    {
        DB_id uid;
        CONTAINER(uid)=0;
        o>>uid;
        for (map<CH_id,chat_channel_data>::iterator it=__channels$.begin(); it!=__channels$.end(); it++)
        {
            it->second.moderators.erase(uid);
        }

    }
    break;
    case channel$topic$5:    {
        string top,set_by;
        time_t t;
        CH_id ch;
        CONTAINER(ch)=0;
        o>>ch>>top>>set_by>>t;
        map<CH_id,chat_channel_data>::iterator it=__channels$.find(ch);
        if (it!=__channels$.end())
        {
            it->second.topic=top;
            it->second.set_by=set_by;
            it->second.topic_time=t;
        }

    }
    break;
    case chat_thread$set_status$3: {
        I_UID;
        o>>__u.cstate.status;
    }
    break;


    case user_profile$stat_last_ip$3: {
        I_UID;
        o>>__u.cstate.ip;
        global_state.set_ip(__u.dbid,__u.cstate.ip);
    }
    break;
    case user_profile$ul_mode_contacts_only$3: {
        I_UID;
        o>>__u.ul_mode_contacts_only;
    }
    break;
    case user_profile$ul_mode_hide_male$3: {
        I_UID;
        o>>__u.ul_mode_hide_male;
    }
    break;
    case user_profile$ul_mode_hide_female$3: {
        I_UID;
        o>>__u.ul_mode_hide_female;
    }
    break;

    case user_profile$contact_options$3: {
        I_UID;
        o>>__u.contact_options;
    }
    break;
    case ignore$add$3: {
        I_UID;
        DB_id zid;
        CONTAINER(zid)=0;
        o>>zid;
        __u.ignores.insert(zid);

    }
    break;
    case ignore$remove$3: {
        I_UID;
        DB_id zid;
        CONTAINER(zid)=0;
        o>>zid;
        __u.ignores.erase(zid);


    }
    break;
    case user_profile$set_nick$3:
    {
        I_UID;
        o>>__u.cstate.nick;
    }
    break;
    case contact$add$3: {
        I_UID;
        DB_id zid;
        CONTAINER(zid)=0;
        o>>zid;
        __u.contacts.insert(zid);
    }
    break;
    case contact$remove$3: {
        I_UID;
        DB_id zid;
        CONTAINER(zid)=0;
        o>>zid;
        __u.contacts.erase(zid);
    }
    break;
    case user_profile$setting_show_eye$3: {
        I_UID;
        o>>__u.setting_show_eye;
    }
    break;
    case user_profile$setting_neye$3: {
        I_UID;
        o>>__u.setting_neye;
    }
    break;
    case user_profile$privs$3:
    {
        I_UID;
        o>>__u.__privileges;
    }
    break;
    case nick$add$3:
    {
        I_UID;

        user_nick n;
        o>>n;

        string sn=str_nick(n.name);
        if (it!=__users$.end())
        {

            strnick2__u.insert(make_pair(sn,it->second));
            __u.str_nicks[n.id]=sn;
        }
        user_register_1_nick(it->first,sn);
    }
    case nick$remove$3:
    {
        I_UID;
        unsigned int nid;
        o>>nid;
        if (__u.str_nicks.count(nid))
        {
            string sn=__u.str_nicks[nid];

            strnick2__u.erase(sn);
            __u.str_nicks.erase(nid);
            user_unregister_1_nick(it->first,sn);
        }
    }
    case nick$update_nick$4:
    {
        I_UID;
        unsigned int nid;
        string name;
        o>>nid>>name;
        __u.str_nicks[nid]=name;

    }
    break;

    }

}

PTR_getter_nm<chat_stuff_user_p>  chat_thread_stuff::find_nick(const string& strn)
{
    map<string,PTR_getter_nm<chat_stuff_user_p> >::const_iterator zit=strnick2__u.find(strn);
    if (zit!=strnick2__u.end()) return zit->second;
    return NULL;

}

void chat_thread_stuff::on_irc_cmd_ISON(chat_event_irc_cmd_ISON *e)
{
    MUTEX_INSPECTOR;
    vector<string> onus;
    DB_id uid=user$id(e->__U);
    PTR_getter_nm<chat_stuff_user_p> __pu=get_user_p(uid);
    chat_stuff_user&__u=__pu.___ptr->ud;

    for (unsigned int i=0; i<e->nicks.size(); i++) {
        string strn=str_nick(e->nicks[i]);
        PTR_getter_nm<chat_stuff_user_p> zit=find_nick(strn);
        if (!zit)
            continue;
        chat_stuff_user &__z=zit.___ptr->ud;

        if (__u.I_can_see_other(__z))
            onus.push_back(e->nicks[i]);

    }
    irc_send_reply(e->__U,303,join(" ",onus));

}

void chat_thread_stuff::on_JOIN_fix_lists(const chat_channel_data &__c,const PTR_getter_nm<chat_stuff_user_p> &__pu, const CH_id& channel,const user_status &status, const user_nick& nick, const CT& _ct)
{
    MUTEX_INSPECTOR;
    chat_stuff_user&__u=__pu.___ptr->ud;
    set<CH_id> s_ch;
    __u.cstate.export_channels(s_ch);

    user_nick oldnick=__u.cstate.nick;
    __u.cstate.nick=nick;
    int New_st=status.id;

    bool New_vis	=	New_st!=st_invisible && New_st!=st_offinvisible;
    int oldst=__u.cstate.status.id;
    bool old_vis	=	oldst!=st_invisible && oldst!=st_offinvisible;
    __u.cstate.status=status;

    bool old_in_chan=__u.cstate.__channel_us.count(channel);

    if (__channels$.count(channel)==0)throw cError("if(__channels$.count(c)==0)"+_DMI());
    {

        if (_ct==T_IRC)
            __u.cstate.__channel_us[channel].insert(_ct);
        else if (_ct==T_WWW)
            __u.cstate.www_channel_replace(channel);
    }
    {

        if (!old_in_chan&& New_vis)
            greeting_msgs_channels.push_back(make_pair(channel,__pu));
        if (old_in_chan&& old_vis&& !New_vis)
            bye_msgs_channels.push_back(make_pair(channel,__pu));
        if (old_in_chan&& !old_vis&& New_vis)
            greeting_msgs_channels.push_back(make_pair(channel,__pu));
    }

    global_state.set_user_state(__u.dbid,__u.cstate);
    __u.cstate.export_channels(s_ch);
    {


        normalize_user_channels(_ct,__pu);
        update_contact_list_all(s_ch,__pu);
        global_state.set_user_state(__u.dbid,__u.cstate);
        need_send_rc=true;
    }
    handle_stat_t_count(__pu);
}

void chat_thread_stuff::on_common_JOIN(chat_event_common_JOIN *e)
{


    if (e->nick.name=="")
    {
        logErr2("if(e->nick.name==\"\") %s %d",__FILE__,__LINE__);
        return;
    }

    MUTEX_INSPECTOR;
    DB_id uid=user$id(e->__U);

    PTR_getter_nm<chat_stuff_user_p> __pu=get_user_p(uid);
    chat_stuff_user&__u=__pu.___ptr->ud;
    pair<DB_id,CT> key=make_pair(uid,e->_ct);
    if (!local_clients.count(key))
    {
        MUTEX_INSPECTOR;
        client_instance_p *__p=new client_instance_p(e->__U,__pu,e->_ct);
        LEAKCTL_ADD(__p);
        local_clients.insert(make_pair(key,__p));
    }
    map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> >::const_iterator uci=local_clients.find(key);
    if (uci==local_clients.end())
    {
        MUTEX_INSPECTOR;
        throw cError("if(uci==local_clients.end())  "+_DMI());
    }
    client_instance &ci=uci->second.___ptr->ci;
    ci.__U=e->__U;



    map<CH_id,chat_channel_data>::const_iterator cit=__channels$.find(e->channel);
    if (cit==__channels$.end()  )
    {
        MUTEX_INSPECTOR;
        if (e->_ct==T_IRC)
        {
            irc_send_reply(e->__U,401,"No such nick/channel");
        }
        return;

    }
    const chat_channel_data &__c=cit->second;


    on_JOIN_fix_lists(__c,__pu,e->channel,e->status, e->nick, e->_ct);

    update_my_contact_list_nofix_www_list(uci->second,e->channel,e->_ct);

    out_oscar_buffer o;
    o<<e->nick<<e->status<<e->channel<<e->_ct;
    __set_user_state(user_state::JOIN_CHANNEL,uid,o);




    if (e->_ct==T_IRC)
    {
        MUTEX_INSPECTOR;
        string s=":"+__u.cstate.nick.name+"!"+__u.cstate.nick.name+"@"+__u.cstate.ip+" JOIN :#"+__c.irc_name+"\r\n";
        data_push_back_irc(e->__U,s);

        if (__c.topic.size()) irc_send_reply(e->__U,332,"#"+__c.irc_name,__c.topic);
        s=":"+string(irc_server_config.host)+" 333 "+__u.cstate.nick.name+" #"+__c.irc_name+" "+__c.set_by+" "+itostring(__c.topic_time)+"\r\n";
        data_push_back_irc(e->__U,s);
        map<int, vector<string> > nks;
        int idx=0;
        map<DB_id,client_instance::vis_item > & vu=ci.visible_users[e->channel];
        for ( map<DB_id,PTR_getter_nm<chat_stuff_user_p > > ::const_iterator i=__users$.begin(); i!=__users$.end(); i++)
        {
            if (i->second.___ptr->ud.cstate.__channel_us.count(e->channel))
            {
                chat_stuff_user &__z=i->second.___ptr->ud;
                if (vu.count(__z.dbid))
                {
                    idx++;
                    string prefix;
                    if (__z.level>=am_admin && __z.setting_show_eye) prefix="@";
                    nks[idx/20].push_back(prefix+__z.cstate.nick.name);
                }
            }
        }
        for (map<int, vector<string> >::const_iterator i_nks=nks.begin(); i_nks!=nks.end(); i_nks++)
        {
            irc_send_reply(e->__U,353,"= #"+__c.irc_name,join(" ",i_nks->second));
        }
        irc_send_reply(e->__U,366,"#"+__c.irc_name,"End of NAMES list.");
    }

    return;
}




const chat_channel_data & chat_thread_stuff::channel_cref(const CH_id &ch) const
{
    MUTEX_INSPECTOR;

    map<CH_id,chat_channel_data> ::const_iterator  i=__channels$.find(ch);
    if (i!=__channels$.end()) return i->second;
    throw cError("channel_cref not exists"+_DMI());
}
chat_channel_data & chat_thread_stuff::channel_ref(const CH_id &ch)
{
    MUTEX_INSPECTOR;

    map<CH_id,chat_channel_data>::iterator i=__channels$.find(ch);
    if (i!=__channels$.end()) return i->second;
    throw cError("channel_ref not exists"+_DMI());
}


string chat_thread_stuff::__local_users_in_channels$_dump()
{
    vector<string> o;
    for (map<CH_id,map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> >  >::const_iterator i=__local_users_in_channels$.begin(); i!=__local_users_in_channels$.end(); i++)
    {
        o.push_back(itostring(CONTAINER(i->first))+"->"+itostring(i->second.size()));
    }
    return join(";",o);


}
string chat_thread_stuff::__users_in_channels$_dump()
{
    vector<string> o;
    for (map<CH_id,set<DB_id> >::const_iterator i=__users_in_channels$.begin(); i!=__users_in_channels$.end(); i++)
    {
        o.push_back(itostring(CONTAINER(i->first))+"->"+itostring(i->second.size()));
    }
    return join(";",o);


}
void chat_processor::load_config(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m)
{

}

void chat_thread_stuff::on_irc_cmd_WHOIS(chat_event_irc_cmd_WHOIS *e)
{
    MUTEX_INSPECTOR;
    string strn=str_nick(e->nick);
    DB_id uid=user$id(e->__U);

    PTR_getter_nm<chat_stuff_user_p> __pu=get_user_p(uid);
    chat_stuff_user&__u=__pu.___ptr->ud;

    PTR_getter_nm<chat_stuff_user_p> zit=find_nick(strn);
    if (!zit)
        return;


    chat_stuff_user &__z=zit.___ptr->ud;

    unsigned int levelsub_levels;
    bool admin;
    bool found=false;
    string cnick;
    string irc_host=string(irc_server_config.host);
    string r;

    PTR_getter<user_profile>
    __UP=user$profiles.FindByID(CONTAINER(uid)),
    __ZP=user$profiles.FindByID(CONTAINER(__z.dbid));
    if (!__UP || !__ZP) {
        irc_send_reply(e->__U,401,"No such nick/channel");
        return;
    }
    {
        PGLR(user_profile,u,__UP);
        cnick=u->last_nick;
        levelsub_levels=u->level/sub_levels;
    }
    admin=levelsub_levels>=am_admin;
    st_dbh d;
    vector<string>	v=d.dbh->select_1_rowQ((QUERY)"select name,login,stat_last_ip,hide_name from tbl_users where id=?"<<CONTAINER(__z.dbid));
    if (v.size()!=4) throw cError("v.size()!=4");
    string zname=v[0];
    string zlogin=v[1];
    string host=levelsub_levels>=am_spy?v[2]:irc_host;
    bool hide_name=atoi(v[3]);


    if (hide_name) {
        if (!admin) {
            zname="";
        } else {
            zname << " " << "(скрыто)";
        }
    }

    string nnk=e->nick;
    if (levelsub_levels>=am_spy) {
        nnk=zlogin;
    }
    r+=irc_gen_reply2(311,e->nick+" "+nnk+" "+host+" *",zname,irc_host,cnick);
    r+=irc_gen_reply2(312,e->nick+" "+irc_host,irc_server_config.location,irc_host,cnick);
    if (admin)r+=irc_gen_reply2(317,e->nick+" "+itostring(time(NULL)-__z.tmp$last_m_time),"seconds idle",irc_host,cnick);
    string out_irc;
    make_user_info(__z.dbid,uid,admin,irc_host,cnick,true,out_irc);
    r+=out_irc;
    r+=irc_gen_reply2(318,e->nick,"End of WHOIS list.",irc_host,cnick);

    if (__u.I_can_see_other(__z))
    {

        data_push_back_irc(e->__U,r);
    }
    else {

        irc_send_reply(e->__U,401,"No such nick/channel");
    }
}
