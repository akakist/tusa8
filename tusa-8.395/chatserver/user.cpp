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
#include "web_tools.h"
#include "file_set.h"
#include "web_server_data.h"
#include "channels.h"
#include "levels.h"

#include "server_config.h"



#include "chat_config.h"
#include "irc_server_config.h"
#include "oscar_buffer.h"
#include "irc_utils.h"
#include "tcp_sender.h"
#include "DB_id.h"
#include "CH_id.h"
#include "update_cmd.h"
#include "levels.h"
#include "chat_msg.h"
#include "cm_container.h"
#include "chat_thread_event.h"
#include "chat_stuff.h"
#include "ignore_vec.h"
#include "contact_vec.h"
#include "user_profile.h"
#include "chat_globals.h"
#include "global_state.h"
#include "chat_stuff.h"
#include <set>
#include "user_credits.h"
#include "admin.h"
#include "M_HASHMAP.h"
#include "chat_colors.h"
#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
#include <unistd.h>
#endif

string replace_smiles(const map<string,string> &p, const string &__src);
int do_old_ipban(const string &ip_orig,time_t t,const string& za,const DB_id& adm_uid);

string fix_www_irccodes(const string& ms);

db_map<unsigned int,user_profile> user$profiles("tbl_users");


db_map<unsigned int,nick_vec> nick_vecs("nick_vecs");
db_map<unsigned int,ignore_vec> ignore_vecs("ignore_vecs");
db_map<unsigned int,contact_vec> contact_vecs("contact_vecs");


chat_users_container local_users;



string homedir(unsigned int uid);
void get_notes_files(set<string> &files, unsigned int uid);

bool intersect_channel(const chat_stuff_user &__u, const chat_stuff_user &__z);
inline set<CH_id> join(const set<CH_id>&a, const set<CH_id>&b)
{
    set<CH_id> r;
    for (set<CH_id>::const_iterator i=a.begin(); i!=a.end(); i++)  r.insert(*i);
    for (set<CH_id>::const_iterator i=b.begin(); i!=b.end(); i++)  r.insert(*i);
    return r;
}
inline  void add(set<CH_id>&a, const set<CH_id>&b)
{
    for (set<CH_id>::const_iterator i=b.begin(); i!=b.end(); i++)  a.insert(*i);
}
inline  void add(set<pair<CH_id,DB_id> > &a, const set<CH_id>&b,const DB_id uid)
{
    for (set<CH_id>::const_iterator i=b.begin(); i!=b.end(); i++)  a.insert(make_pair(*i,uid));
}
inline  void add(deque<pair<CH_id,DB_id> > &a, const set<CH_id>&b,const DB_id uid)
{
    for (set<CH_id>::const_iterator i=b.begin(); i!=b.end(); i++)  a.push_back(make_pair(*i,uid));
}

inline  void add(deque<pair<CH_id,pair<DB_id,string> > > &a, const set<CH_id>&b,const DB_id& uid, const string& s)
{
    for (set<CH_id>::const_iterator i=b.begin(); i!=b.end(); i++)  a.push_back(make_pair(*i,make_pair(uid,s)));
}

inline bool intersect(const set<CH_id>&a, const set<CH_id>&b)
{
    for (set<CH_id>::const_iterator i=a.begin(); i!=a.end(); i++)
    {
        if (b.count(*i)) return true;
    }
    return false;
}

string get_remove_string(const DB_id& uid)
{
    string s="<SCRIPT>parent.DeleteUser("+itostring(CONTAINER(uid))+");</SCRIPT>\r\n";
    return s;
}
string chat_thread_stuff::LK_chat_channel_get_n_messages(const CH_id& ch_id, const chat_stuff_user& __u,unsigned int n)
{
    MUTEX_INSPECTOR;

    int nm=0;

    deque<string> out;
    for (deque<PTR_getter_nm<chat_msg> >::reverse_iterator i=_$messages.rbegin(); i!=_$messages.rend(); i++)
    {
        if (i->___ptr->data->is_ignored(__u.ignores)) continue;
        string s=i->___ptr->data->make(ch_id,__u.dbid);
        if (s.size()) {
            out.push_front(s);
            nm++;
        }
        if (nm>n)break;

    }
    string o;
    for (unsigned i=0; i<out.size(); i++)
        o+=out[i];
    return o;
}


string  user_add_set(const CH_id& channel,const chat_stuff_user &uu,const chat_stuff_user &zz)
{
    string s;
    // nick
    s="{n:'"+zz.cstate.nick.name+"',";
    // nick for sorting
    s+="nu:'"+str_nick(nick_for_sort(zz.cstate.nick.name))+"',";
    //nick id
    s+="nid:"+itostring(zz.cstate.nick.id)+",";
    //name of status
    s+="sn:'"+ES(zz.cstate.status.name)+"',";
    // index of status picture
    s+="spn:"+itostring(zz.cstate.status.pic)+",";
    // user id
    s+="uid:"+itostring(CONTAINER(zz.dbid))+",";
    // voice
    s+="v:"+itostring(zz.cstate.voice.count(channel))+",";
    bool adm=zz.level/sub_levels>=am_admin && zz.setting_show_eye;
    // show admin eye
    s+="a:"+itostring(adm)+",";
    // number of eye
    if (adm)
        s+="ae:"+itostring(zz.setting_neye)+",";

    tm now=LOCALTIME(time(NULL));
    tm bd=LOCALTIME(zz.birth_date);
    bool isbd=now.tm_yday==bd.tm_yday;

    // nick class
    if (isbd)
        s+="ns:'"+zz.nickclass_birthdate+"',";
    else
        s+="ns:'"+zz.nickclass+"',";

    //has info
    bool hi=uu.level>=600 || !zz.unreged;
    s+="hi:"+itostring(hi)+",";

    // kick button
    if (uu.level/sub_levels>zz.level/sub_levels && uu.level>=600)
        s+="kb:1,";
    else
        s+="kb:0,";

    // gender
    s+="g:"+itostring(zz.gender)+",";
    // unreged
    s+="unr:"+itostring(zz.unreged)+",";
    //ignored
    s+="ign:"+itostring(uu.ignores.count(zz.dbid)!=0)+"}";



    return s;
}
string get_add_string(const CH_id& channel,const chat_stuff_user &uu,const chat_stuff_user &zz)
{
    string s;
    s="<SCRIPT>parent.AddUser("+user_add_set(channel,uu,zz)+");</SCRIPT>\r\n";
    return s;
}

string chat_thread_stuff::LK_get_java_users(const CH_id& channel, chat_stuff_user &__u,client_instance& __u_instance)
{
    MUTEX_INSPECTOR;
    XTRY;


    vector<string> v;
    map<DB_id,client_instance::vis_item > &vu=__u_instance.visible_users[channel];

    for (map<DB_id,client_instance::vis_item >::const_iterator i=vu.begin(); i!=vu.end(); i++)
    {
        chat_stuff_user &zz=i->second.__pu.___ptr->ud;
        string ret;
        ret+=user_add_set(channel,__u,zz);
        v.push_back(ret);
    }
    str_sort(v);
    string res=join(", ",v);

    return res;
    XPASS;
}

static inline bool is_inv(const chat_stuff_user& __u)
{
    if (__u.cstate.status.id==st_invisible || __u.cstate.status.id==st_offinvisible ) return true;
    return false;
}

bool chat_thread_stuff::can_preload_user(const DB_id& uid)
{

    if (__users$.count(uid))	return true;
    if (!contact_vecs.get(CONTAINER(uid))) return false;
    if (!ignore_vecs.get(CONTAINER(uid))) return false;
    if (!user$profiles.get(CONTAINER(uid))) return false;
    if (!user$profiles.get(CONTAINER(uid))) return false;
    if (!nick_vecs.get(CONTAINER(uid))) return false;
    return true;
}
void chat_thread_stuff::preload_user(const DB_id& uid)
{


    if (CONTAINER(uid)==0) throw cError("--Error: preload user 0"+_DMI());

    chat_stuff_user_p *p=new chat_stuff_user_p;
    LEAKCTL_ADD(p);
    PTR_getter_nm<chat_stuff_user_p> pu=p;



    chat_stuff_user &uu=pu.___ptr->ud;

    uu.dbid=uid;
    user_register_all_nicks(uid);
    {
        PGLR(contact_vec,c,contact_vecs.find_by_id_create(CONTAINER(uid)));
        uu.contacts=c->contact;

    }
    {
        PGLR(ignore_vec,c,ignore_vecs.find_by_id_create(CONTAINER(uid)));
        uu.ignores=c->ignore;
    }
    int binv;
    PTR_getter<user_profile> __UP=user$profiles.FindByID(CONTAINER(uid));
    if (__UP) {
        PGLR(user_profile,u,__UP);
        uu.gender=u->get_gender();
        uu.login=u->login;
        binv=u->bought_invisibility;

        const map<unsigned int,user_status> &st=u->status;
        if (st.count(st_online))
        {
            uu.cstate.status=st.find(st_online)->second;
            global_state.set_user_state(uu.dbid,uu.cstate);
        }
        uu.level=u->level;
        uu.__privileges=u->__privileges;
        uu.contact_options=u->contact_options;
        uu.setting_show_eye=u->setting_show_eye;
        uu.setting_neye=u->setting_neye;
        uu.n_color_pack=u->setting_n_color_pack;
        uu.cstate.nick.id=u->last_nick_id;
        uu.cstate.nick.name=u->last_nick;
        uu.ul_mode_contacts_only=u->ul_mode_contacts_only;
        uu.ul_mode_hide_male=u->ul_mode_hide_male;
        uu.ul_mode_hide_female=u->ul_mode_hide_female;
        uu.unreged=u->unreged;
        uu.birth_date=u->birth_date;
    }
    map<unsigned int,c_level*>::iterator lit=level$.find(uu.level);
    if (lit==level$.end())
    {
        logErr2("Fatal cannot find level %d in stuff",uu.level);
        if (level$.size())
        {
            uu.c$level=level$.begin()->second;
        }
        else throw cError("CANNOT ASSIGN LEVEL"+_DMI());
    }
    uu.c$level=lit->second;

    uu._invisibility=binv+uu.c$level->invisibility;

    uu._bought_invisibility=binv;



    PTR_getter<__c_pack> cp=c_packs.get(uu.n_color_pack);
    {
        PGLR(__c_pack, c,cp);
        uu.nickclass=c->nickclass;
        uu.nickclass_birthdate=c->nickclass_birthdate;
    }
    {
        PGLR(nick_vec,u,nick_vecs.find_by_id_create(CONTAINER(uid)));
        for (map<unsigned int,user_nick> ::const_iterator i=u->u_nicks.begin(); i!=u->u_nicks.end(); i++)
        {
            string sn=str_nick(i->second.name);
            strnick2__u.insert(make_pair(sn,pu));
            uu.str_nicks[i->first]=sn;
        }
        if (u->u_nicks.size())
        {
            map<unsigned int,user_nick> ::const_iterator it=u->u_nicks.find(uu.cstate.nick.id);


            if (it==u->u_nicks.end())
                uu.cstate.nick=u->u_nicks.begin()->second;
            else  uu.cstate.nick=it->second;
        }
    }

    uu.tmp$last_m_time=0;
    uu.tmp$last_invite_time=0;
    uu.tmp$msg_cnt_per_time=0;
    global_state.set_user_state(uu.dbid,uu.cstate);
    __users$.insert(make_pair(uid,pu));

}
void chat_thread_stuff::unload_user(const DB_id& uid)
{

    map<DB_id,PTR_getter_nm<chat_stuff_user_p> > ::const_iterator it=__users$.find(uid);
    if (it==__users$.end()) return;
    const chat_stuff_user &__uu=it->second.___ptr->ud;
    for (map<unsigned int,string>::const_iterator i=__uu.str_nicks.begin(); i!=__uu.str_nicks.end(); i++)
    {
        strnick2__u.erase(i->second);
    }
    __users$.erase(uid);
    global_state.remove(uid);

}





void chat_thread_stuff::update_my_contact_list_nofix_www_list(const PTR_getter_nm<client_instance_p> &__pci , const CH_id &ch, const CT &_ct)
{
    MUTEX_INSPECTOR;

    client_instance &ci=__pci.___ptr->ci;

    chat_stuff_user &__u=ci.usr.___ptr->ud;
    map<DB_id,client_instance::vis_item>  &vu=ci.visible_users[ch];
    vu.clear();
    vu.insert(make_pair(__u.dbid,client_instance::vis_item(__u.cstate.status.id,__u.cstate.nick.name,ci.usr)));
    for ( map<DB_id,PTR_getter_nm<chat_stuff_user_p > > ::const_iterator i=__users$.begin(); i!=__users$.end(); i++)
    {
        chat_stuff_user &__z=i->second.___ptr->ud;

        map<CH_id,set<CT> >::const_iterator cui=__z.cstate.__channel_us.find(ch);

        if (cui!=__z.cstate.__channel_us.end())
        {
            if (cui->second.size())
            {

                bool can_see=__u.I_can_see_contact_list(__z);
                if (can_see)
                    vu.insert(make_pair(i->first,client_instance::vis_item(__z.cstate.status.id,__z.cstate.nick.name,i->second)));
            }
        }
    }
}


void chat_thread_stuff::update_my_contact_list_fix_www_list(const PTR_getter_nm<client_instance_p> &__pci, const CH_id &ch, const CT &_ct)
{
    MUTEX_INSPECTOR;
    chat_stuff_user &__u=__pci.___ptr->ci.usr.___ptr->ud;
    client_instance &ci=__pci.___ptr->ci;

    map<DB_id,client_instance::vis_item>  &vu=ci.visible_users[ch];
    for ( map<DB_id,PTR_getter_nm<chat_stuff_user_p > > ::const_iterator i=__users$.begin(); i!=__users$.end(); i++)
    {
        chat_stuff_user &__z=i->second.___ptr->ud;
        bool can_see=false;
        map<CH_id,set<CT> >::const_iterator cui=__z.cstate.__channel_us.find(ch);
        if (cui!=__z.cstate.__channel_us.end())
        {
            if (cui->second.size())
            {

                can_see=__u.I_can_see_contact_list(__z);
            }
        }

        bool need_add=false,need_remove=false;
        bool vu_count_i_first=vu.count(i->first);
        if (can_see && !vu_count_i_first)
            need_add=true;
        else if (!can_see && vu_count_i_first)
            need_remove=true;

        if (need_add)	vu.insert(make_pair(i->first,client_instance::vis_item(__z.cstate.status.id,__z.cstate.nick.name,i->second)));
        if (need_remove)	vu.erase(i->first);

        if (need_add && _ct==T_WWW)
        {
            data_push_back_www(ci.__U,get_add_string(ch,__u,__z));
        }
        if (need_add && _ct==T_IRC)
        {
            MUTEX_INSPECTOR;
            XTRY;
            const chat_channel_data &c=channel_cref(ch);
            string dt_join=":"+__z.cstate.nick.name+"!"+irc_nick$host(__z.cstate.nick.name)+" JOIN #"+c.irc_name+"\r\n";
            data_push_back_irc(ci.__U,dt_join);
            XPASS;
        }
        if (need_remove && _ct==T_WWW)
        {

            data_push_back_www(ci.__U,get_remove_string(__z.dbid));
        }
        if (need_remove && _ct==T_IRC)
        {
            MUTEX_INSPECTOR;
            XTRY;
            const chat_channel_data &c=channel_cref(ch);
            string dt_part=":"+__z.cstate.nick.name+"!"+irc_nick$host(__z.cstate.nick.name)+" PART #"+ c.irc_name +" :\r\n";
            data_push_back_irc(ci.__U,dt_part);
            XPASS;
        }
    }


}

void proc_send_msg(chat_thread_stuff* stuff,const map<CH_id,string> & content)
{

    for (map<CH_id,string> ::const_iterator C=content.begin(); C!=content.end(); C++)
    {


        map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> > &ccc=stuff->__local_users_in_channels$[C->first];
        for (map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> >::const_iterator i=ccc.begin(); i!=ccc.end(); i++)
        {
            if (i->first.second!=T_WWW) continue;
            data_push_back_www(i->second.___ptr->ci.__U,C->second+SCROLL);
        }
    }

}

void chat_thread_stuff::handle_room_event_messages()
{
    MUTEX_INSPECTOR;

    map<CH_id,string> content;
    while (greeting_msgs_channels.size())
    {
        pair<CH_id,PTR_getter_nm<chat_stuff_user_p> >  p=*greeting_msgs_channels.begin();

        chat_stuff_user &__u=p.second.___ptr->ud;
        chat_msg_GREETING *_p1=new chat_msg_GREETING(p.first,rand()%msgs_greeting.size(),
                __u.cstate.nick,__u.gender,__u.n_color_pack);
        LEAKCTL_ADD(_p1);

        chat_msg *_p2=new chat_msg(_p1);
        LEAKCTL_ADD(_p2);
        PTR_getter_nm<chat_msg> m=_p2;
        save_chat_msg(m);
        DB_id id;
        CONTAINER(id)=0;
        CONTAINER(id)=0;
        content[p.first]+=m.___ptr->data->make(p.first,id);
        greeting_msgs_channels.pop_front();
    }
    while (bye_msgs_channels.size())
    {
        pair<CH_id,PTR_getter_nm<chat_stuff_user_p> > p=*bye_msgs_channels.begin();

        chat_stuff_user &__u=p.second.___ptr->ud;
        chat_msg_QUIT *_p1=new chat_msg_QUIT(p.first,rand()%msgs_bye.size(),__u.cstate.nick,__u.gender,__u.n_color_pack);
        LEAKCTL_ADD(_p1);
        chat_msg *_p2=new chat_msg(_p1);
        LEAKCTL_ADD(_p2);
        PTR_getter_nm<chat_msg> m=_p2;
        save_chat_msg(m);
        DB_id id;
        CONTAINER(id)=0;
        CONTAINER(id)=0;
        content[p.first]+=m.___ptr->data->make(p.first,id);
        bye_msgs_channels.pop_front();
    }
    while (chroom_msgs_channels.size())
    {
        pair<CH_id, pair<PTR_getter_nm<chat_stuff_user_p> ,string /*name of New channel*/> >  p=*chroom_msgs_channels.begin();

        chat_stuff_user &__u=p.second.first.___ptr->ud;
        chat_msg_CHROOM *_p1=new chat_msg_CHROOM(p.first,__u.gender,rand()%msgs_changes.size(),__u.cstate.nick,p.second.second,__u.n_color_pack);
        LEAKCTL_ADD(_p1);
        chat_msg *_p2=new chat_msg(_p1);
        LEAKCTL_ADD(_p2);
        PTR_getter_nm<chat_msg> m=_p2;
        save_chat_msg(m);
        DB_id id;
        CONTAINER(id)=0;
        CONTAINER(id)=0;
        content[p.first]+=m.___ptr->data->make(p.first,id);
        chroom_msgs_channels.pop_front();
    }
    while (kick_msgs_channels.size())
    {
        pair<CH_id,pair<PTR_getter_nm<chat_stuff_user_p> ,string> > p=*kick_msgs_channels.begin();

        chat_stuff_user &__u=p.second.first.___ptr->ud;
        chat_msg_KICK *_p1=new chat_msg_KICK(p.first,__u.cstate.nick,p.second.second,__u.gender,__u.n_color_pack,__u.n_color_pack);
        LEAKCTL_ADD(_p1);
        chat_msg *_p2=new chat_msg(_p1);
        LEAKCTL_ADD(_p2);
        PTR_getter_nm<chat_msg> m=_p2;
        save_chat_msg(m);
        DB_id id;
        CONTAINER(id)=0;
        CONTAINER(id)=0;
        content[p.first]+=m.___ptr->data->make(p.first,id);
        kick_msgs_channels.pop_front();
    }
    while (chnick_msgs_channels.size())
    {
        pair<CH_id,pair<PTR_getter_nm<chat_stuff_user_p> ,string> > p=*chnick_msgs_channels.begin();

        chat_stuff_user &__u=p.second.first.___ptr->ud;
        {
            chat_msg_CHNICK*_p1=new chat_msg_CHNICK(p.first,__u.gender,rand()%msgs_nickchanges.size(),p.second.second,__u.cstate.nick,__u.n_color_pack);
            LEAKCTL_ADD(_p1);
            chat_msg*_p2=new chat_msg(_p1);
            LEAKCTL_ADD(_p2);
            PTR_getter_nm<chat_msg> m=_p2;

            save_chat_msg(m);
            DB_id id;
            CONTAINER(id)=0;
            CONTAINER(id)=0;
            content[p.first]+=m.___ptr->data->make(p.first,id);
        }
        chnick_msgs_channels.pop_front();
    }
    proc_send_msg(this,content);
}
void chat_thread_stuff::update_contact_list_all_kicked(const set<CH_id> &fluent_channels,const PTR_getter_nm<chat_stuff_user_p>&__pu, const string& _zamsg)
{
    {

        chat_stuff_user &__u=__pu.___ptr->ud;
        for (set<CH_id>::const_iterator C=fluent_channels.begin(); C!=fluent_channels.end(); C++)
        {

            bool u_in_chan=__u.cstate.__channel_us.count(*C);
            map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> > &ccc=__local_users_in_channels$[*C];
            for (map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> >::const_iterator i=ccc.begin(); i!=ccc.end(); i++)
            {
                MUTEX_INSPECTOR;
                XTRY;
                if (i->first.second!=T_IRC) continue;
                chat_stuff_user &__z=i->second.___ptr->ci.usr.___ptr->ud;
                bool z_in_chan=ccc.count(i->first);
                if (!z_in_chan) continue;
                map<DB_id,client_instance::vis_item>  &vu=i->second.___ptr->ci.visible_users[*C];
                const chat_channel_data &c=channel_cref(*C);
                string dt_part;
                dt_part=":System!system@"+string(irc_server_config.host)+" KICK #"+c.irc_name+" "+__u.cstate.nick.name+" :"+_zamsg+"\r\n";
                data_push_back_irc(i->second.___ptr->ci.__U,dt_part);

                vu.erase(__u.dbid);
                XPASS;
            }
        }
    }
}

void chat_thread_stuff::__update_contact_list_all()
{

    __users_in_channels$.clear();
    for (map<DB_id,PTR_getter_nm<chat_stuff_user_p > >::const_iterator i=__users$.begin(); i!=__users$.end(); i++)
    {
        const chat_stuff_user& u=i->second.___ptr->ud;
        for (map<CH_id,set<CT> >::const_iterator k=u.cstate.__channel_us.begin(); k!=u.cstate.__channel_us.end(); k++)
        {
            if (k->second.size())
                __users_in_channels$[k->first].insert(i->first);

        }
    }

    for (map<CH_id,map<DB_id,PTR_getter_nm<chat_stuff_user_p> > > ::const_iterator C=update_contact_list_data.begin(); C!=update_contact_list_data.end(); C++)
    {
        set<DB_id>&_$__users_in_channels$=	__users_in_channels$[C->first];

        map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> > &_$__local_users_in_channels$=__local_users_in_channels$[C->first];
        for (map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> >::const_iterator i=_$__local_users_in_channels$.begin(); i!=_$__local_users_in_channels$.end(); i++)
        {

            chat_stuff_user &__i=i->second.___ptr->ci.usr.___ptr->ud;

            for (map<DB_id,PTR_getter_nm<chat_stuff_user_p> >  ::const_iterator U=C->second.begin(); U!=C->second.end(); U++)
            {
                chat_stuff_user &__u=U->second.___ptr->ud;
                bool U_in_C=_$__users_in_channels$.count(U->first);
                map<DB_id,client_instance::vis_item>  &vu=i->second.___ptr->ci.visible_users[C->first];
                bool I_see_U=vu.count(U->first);
                bool need_add=false;
                bool need_remove=false;
                if (U_in_C)
                {
                    bool cansee=__i.I_can_see_contact_list(__u);
                    if (cansee && !I_see_U)
                        need_add=true;
                    if (!cansee && I_see_U)
                        need_remove=true;

                }
                if (!U_in_C && I_see_U)
                    need_remove=true;

                bool need_cnick=false;
                bool need_cstatus=false;
                string fromnick;
                string tonick;
                if (U_in_C && I_see_U)
                {
                    // analize nick and status;
                    map<DB_id,client_instance::vis_item>::iterator ivu=vu.find(U->first);
                    if (ivu!=vu.end())
                    {
                        if (__u.cstate.nick.name!=ivu->second.nick)
                        {
                            fromnick=ivu->second.nick;
                            tonick=__u.cstate.nick.name;
                            ivu->second.nick=__u.cstate.nick.name;
                            ivu->second.status=__u.cstate.status.id;

                            need_cnick=true;
                        }
                        else if (__u.cstate.status.id!=ivu->second.status)
                        {
                            ivu->second.status=__u.cstate.status.id;
                            need_cstatus=true;
                        }
                    }
                }

                if (i->first.second==T_WWW)
                {
                    PTR_getter<chat_user> &_iU=i->second.___ptr->ci.__U;
                    if (need_add)
                        data_push_back_www(_iU,get_add_string(C->first,__i,__u));
                    if (need_remove)
                        data_push_back_www(_iU,get_remove_string(__u.dbid));
                    if (need_cstatus)
                        data_push_back_www(_iU,"<SCRIPT>parent.set_status("+itostring(CONTAINER(__u.dbid))+","+itostring(__u.cstate.status.pic)+",'"+MES(__u.cstate.status.name)+"');</SCRIPT>\r\n");
                    if (need_cnick)
                        data_push_back_www(_iU,get_remove_string(__u.dbid)+get_add_string(C->first,__i,__u));

                }
                else if (i->first.second==T_IRC)
                {
                    if (need_add)
                    {
                        MUTEX_INSPECTOR;
                        XTRY;

                        const chat_channel_data &c=channel_cref(C->first);
                        string dt_join=":"+__u.cstate.nick.name+"!"+irc_nick$host(__u.cstate.nick.name)+" JOIN #"+c.irc_name+"\r\n";
                        data_push_back_irc(i->second.___ptr->ci.__U,dt_join);
                        XPASS;
                    }
                    if (need_remove)
                    {
                        MUTEX_INSPECTOR;
                        XTRY;

                        const chat_channel_data &c=channel_cref(C->first);
                        string dt_part=":"+__u.cstate.nick.name+"!"+irc_nick$host(__u.cstate.nick.name)+" PART #"+ c.irc_name+" :\r\n";
                        data_push_back_irc(i->second.___ptr->ci.__U,dt_part);
                        XPASS;
                    }
                    if (need_cnick)
                    {

                        string ircd=":"+fromnick+"!"+irc_nick$host(fromnick)+" NICK :"+tonick+"\r\n";
                        data_push_back_irc(i->second.___ptr->ci.__U,ircd);

                    }
                }
                if (need_add)
                    vu.insert(make_pair(__u.dbid,client_instance::vis_item(__u.cstate.status.id,__u.cstate.nick.name,U->second)));
                if (need_remove)
                    vu.erase(__u.dbid);
            }
        }
    }

}

void chat_thread_stuff::update_contact_list_all(const set<CH_id> &fluent_channels,const PTR_getter_nm<chat_stuff_user_p>&__pu)
{
    chat_stuff_user &__u=__pu.___ptr->ud;
    for (set<CH_id>::const_iterator C=fluent_channels.begin(); C!=fluent_channels.end(); C++)
    {
        map<DB_id,PTR_getter_nm<chat_stuff_user_p> > &__uc=update_contact_list_data[*C];
        map<DB_id,PTR_getter_nm<chat_stuff_user_p> > ::iterator it=__uc.find(__u.dbid);
        if (it==__uc.end())
            __uc.insert(make_pair(__u.dbid,__pu));
    }
    if (update_contact_list_data_create_time==0)
        update_contact_list_data_create_time=time(NULL);
}

void chat_thread_stuff::do_set_user_state(int cmd,const PTR_getter_nm<chat_stuff_user_p> &__pu,oscar_buffer& o)
{

    chat_stuff_user &__u=__pu.___ptr->ud;
    try {
        switch (cmd)
        {
        case user_state::JOIN_CHANNEL:
        {

            user_nick nick;
            user_status status;
            CH_id channel;
            CONTAINER(channel)=0;
            CT _ct;
            for (map<CH_id,set<CT> >::iterator i=__u.cstate.__channel_us.begin(); i!=__u.cstate.__channel_us.end(); i++)
            {

            }



            o>>nick>>status>>channel>>_ct;

            if (nick.name.size()==0)
            {
                logErr2("nick.name.size()==0  status %d channel %d ct %d (%s %d)",status.id,CONTAINER(channel),_ct, __FILE__,__LINE__);
                return;
            }
            map<CH_id,chat_channel_data>::iterator cit=__channels$.find(channel);
            if (cit==__channels$.end())
            {
                return;
            }
            chat_channel_data &__c=cit->second;


            on_JOIN_fix_lists(__c,__pu, channel,status, nick, _ct);
        }
        break;
        case user_state::SET_STATUS:
        {

            if (__u.cstate.__channel_us.size()==0)return;
            set<CH_id> s_ch;

            __u.cstate.export_channels(s_ch);
            int oldst=__u.cstate.status.id;
            bool old_vis=__u.cstate.status.id!=st_invisible && __u.cstate.status.id!=st_offinvisible;

            o>>__u.cstate.status;

            bool New_vis=__u.cstate.status.id!=st_invisible && __u.cstate.status.id!=st_offinvisible;
            int New_st=__u.cstate.status.id;
            global_state.set_user_state(__u.dbid,__u.cstate);

            if (New_vis && !old_vis)
            {

                foreach_channel_us(i,__u.cstate)
                {
                    greeting_msgs_channels.push_back(make_pair(i->first,__pu));
                }
            }
            if (!New_vis && old_vis)
            {

                foreach_channel_us(i,__u.cstate)
                {
                    bye_msgs_channels.push_back(make_pair(i->first,__pu));
                }
            }
            global_state.set_user_state(__u.dbid,__u.cstate);

            __u.cstate.export_channels(s_ch);

            {

                update_contact_list_all(s_ch,__pu);
            }
            need_send_rc=true;
            return;
        }
        break;
        case user_state::KICK_USER:
        {


            set<CH_id> s_ch;
            __u.cstate.export_channels(s_ch);
            string kick_msg;
            o>>kick_msg;
            if (__u.cstate.status.id!=st_invisible && __u.cstate.status.id!=st_offinvisible )
            {

                foreach_channel_us(i,__u.cstate)
                {
                    kick_msgs_channels.push_back(make_pair(i->first,make_pair(__pu,kick_msg)));
                }
            }

            map<pair<DB_id,CT>, PTR_getter_nm<client_instance_p> >::const_iterator it=local_clients.find(make_pair(__u.dbid,T_WWW));

            if (it!=local_clients.end())
            {

                if (kick_msg.size()) {
                    {
                        map<string,string>q;
                        q["~kick_msg~"]=kick_msg;
                        data_push_back_www(it->second.___ptr->ci.__U,RVfiles(q,"scripts/kicked2"));
                    }
                } else {
                    data_push_back_www(it->second.___ptr->ci.__U,RVfiles("scripts/kicked"));
                }
            }

            __u.cstate.__channel_us.clear();

            global_state.set_user_state(__u.dbid,__u.cstate);
            __u.cstate.export_channels(s_ch);

            update_contact_list_all_kicked(s_ch, __pu,kick_msg);
            normalize_user_channels_clean(__u.dbid);
            update_contact_list_all(s_ch, __pu);
            local_clients.erase(make_pair(__u.dbid,T_WWW));
            local_clients.erase(make_pair(__u.dbid,T_IRC));
            local_users.erase$(__u.dbid,T_WWW);
            local_users.erase$(__u.dbid,T_IRC);
            need_send_rc=true;
            DB_id uid=__u.dbid;
            __users$.erase(uid);
            global_state.erase(uid);
            st_dbh d;
            string pn=chat_config.php_session_prefix.get()+d.dbh->select_1Q((QUERY)"select php_session_id from tbl_users where id=?"<<CONTAINER(__u.dbid));
            unlink(pn.c_str());
            return;
        }
        break;

        case user_state::PART_CHANNEL:
        {

            set<CH_id> s_ch;
            __u.cstate.export_channels(s_ch);
            CT _ct;
            set<CH_id> chans;
            o>>chans>>_ct;
            set<CH_id> parted=__u.cstate.part_channels(chans,_ct);
            if (__u.cstate.status.id!=st_invisible && __u.cstate.status.id!=st_offinvisible )
            {

                for (set<CH_id>::const_iterator i=parted.begin(); i!=parted.end(); i++)
                {
                    bye_msgs_channels.push_back(make_pair(*i,__pu));
                }
            }
            global_state.set_user_state(__u.dbid,__u.cstate);
            __u.cstate.export_channels(s_ch);


            normalize_user_channels(_ct,__pu);
            update_contact_list_all(s_ch, __pu);
            need_send_rc=true;

        }
        break;
        case user_state::SET_NICK:
        {
            if (__u.cstate.__channel_us.size()==0)return;

            set<CH_id> s_ch;
            __u.cstate.export_channels(s_ch);
            string from_nick=__u.cstate.nick.name;
            user_nick nnn;
            o>>nnn;
            if (nnn.name.size()==0)
            {
                logErr2("nick.name.size()==0 %s %d",__FILE__,__LINE__);
                return;
            }
            __u.cstate.nick=nnn;
            string  New_nick=__u.cstate.nick.name;
            global_state.set_user_state(__u.dbid,__u.cstate);
            if (__u.cstate.status.id!=st_invisible&&__u.cstate.status.id!=st_offinvisible)
            {
                foreach_channel_us(i,__u.cstate)
                {
                    if (i->second.size())
                    {
                        chnick_msgs_channels.push_back(make_pair(i->first,make_pair(__pu,from_nick)));
                    }
                }
            }
            __u.cstate.export_channels(s_ch);

            update_contact_list_all(s_ch,__pu);

        }
        break;
        case user_state::CHANGE_CONTACT_MODE:
        {

            unsigned char c;
            o>>c;
            if (c=='C')  o>>__u.ul_mode_contacts_only;
            else if (c=='F')  o>>__u.ul_mode_hide_female;
            else if (c=='M')  o>>__u.ul_mode_hide_male;

        }
        break;
        case user_state::CONTACT_ADD:
        {

            DB_id zid;
            CONTAINER(zid)=0;
            o>>zid;
            __u.contacts.insert(zid);
        }

        break;
        case user_state::CONTACT_REMOVE:
        {

            DB_id zid;
            CONTAINER(zid)=0;
            o>>zid;
            __u.contacts.erase(zid);

        }
        break;

        default:
            logErr2("undef user_state::%d",cmd);
        }
        handle_stat_t_count(__pu);
    } catch (cError e) {
        logErr2("catched %s (%s %d)",e.what(),__FILE__,__LINE__);
    } catch (...) {
        logErr2("ctched %s %d",__FILE__,__LINE__);
    }

}
void chat_thread_stuff::handle_stat_t_count(const PTR_getter_nm<chat_stuff_user_p> &__pu)
{
    st_update stu;
    chat_stuff_user &__u=__pu.___ptr->ud;
    size_t jtc=join_time.count(__u.dbid);
    if (__u.cstate.channel_us_size() && !jtc)
    {
        join_time[__u.dbid]=time(NULL);
    }
    if (__u.cstate.channel_us_size()==0 && jtc)
    {
        __send_update_d3(user_profile$add_stat_t_count$3,CONTAINER(__u.dbid),time(NULL)-join_time[__u.dbid]);
        join_time.erase(__u.dbid);
    }



    if (__u.cstate.channel_us_size()==0 )
        trash[__u.dbid]=time(NULL);
    else
        trash.erase(__u.dbid);

}
void chat_thread_stuff::on_SET_user_state(chat_event_SET_user_state* e)
{
    MUTEX_INSPECTOR;
    if (e->CONTAINER(uid)==0)
    {
        logErr2("CONTAINER(uid)==0 cmd=%d  %s",e->cmd,_DMI().c_str());
    }
    else
    {
        oscar_buffer o(e->msg.data(),e->msg.size());
        PTR_getter_nm<chat_stuff_user_p> __pu=get_user_p(e->uid);
        do_set_user_state(e->cmd,__pu,o);
    }
}




void chat_thread_stuff::on_CHAT_MESSAGES2(chat_event_CHAT_MESSAGES2*e)
{
    MUTEX_INSPECTOR;
    XTRY;
    st_update stu;
    DB_id uid=user$id(e->__U);
    if (!CONTAINER(uid))
    {
        logErr2("if(!CONTAINER(uid)) %s %d",__FILE__,__LINE__);
        return;
    }
    map<string,string> m;

    //string out;
    PTR_getter_nm<chat_stuff_user_p> __pu=get_user_p(uid);
    chat_stuff_user &__u=__pu.___ptr->ud;

    map<CH_id,set<CT> > old_channel_us=__u.cstate.__channel_us;

    pair<DB_id,CT> lkey=make_pair(uid,T_WWW);
    if (!local_clients.count(lkey))
    {
        client_instance_p *p=new client_instance_p(e->__U,__pu,T_WWW);
        LEAKCTL_ADD(p);
        local_clients.insert(make_pair(lkey,p));
    }

    PTR_getter_nm<client_instance_p> &__u_client_instance_p=local_clients.find(lkey)->second;
    __u_client_instance_p.___ptr->ci.__U=e->__U;


    vector<CH_id>chs=chat_channels._keys();
    set<CH_id> chs_s;
    for (unsigned i=0; i<chs.size(); i++)
        chs_s.insert(chs[i]);

    if (chs_s.count(e->c)) {

        CH_id c=e->c;

        if (!__u.cstate.channel_us_count_usr(c,T_WWW))
        {

            out_oscar_buffer o;
            o<<c<<T_WWW;
            string ooo=o.as_string();
            oscar_buffer oo(ooo.data(),ooo.size());
            do_set_user_state(user_state::CHANGE_CHANNEL,__pu,oo);
            chat_event_SET_user_state ee;
            ee.msg=o.as_string();
            logErr2("CHANGE CHANNEL %s %d",__FILE__,__LINE__);
            ee.cmd=user_state::CHANGE_CHANNEL;
            ee.uid=uid;
            __send_update_d3(user_profile$last_channel$3,CONTAINER(uid),c);
            global_state.set_user_state(__u.dbid,__u.cstate);

        }
    }

    set<CH_id> sc=__u.cstate.get_channel(T_WWW);
    if (sc.size()!=1)
    {
        logErr2("if (sc.size()!=1) sc.size()= %d",sc.size());
        return;
    }
    CH_id c=*sc.begin();
    const chat_channel_data &ch=channel_ref(c);

    if (ch.topic.size())
    {
//		string s="<script>parent.add_messages('<div class=sysmsg>*** Топик комнаты: <b>"+ch.topic+"</b></div>');</script>";
//		s+="<script>parent.add_messages('<div class=sysmsg>*** Поставил(а) <b>"+ch.set_by+"</b>, "+datetime2rus(ch.topic_time)+"</div>');</script>";
//
//		s+=
    }
    if (ch.topic.size()) {
        m["~top~"]="";
        m["~hastopic~"]="true";
        m["~topic~"]=str_replace("\"","\\\"",ch.topic);
        m["~who~"]=str_replace("\"","\\\"",ch.set_by);;
        m["~time~"]=str_replace("\"","\\\"",datetime2rus(ch.topic_time));
    } else {
        m["~hastopic~"]="false";
        m["~topic~"]="";
        m["~who~"]="";
        m["~time~"]="";
    }

    /*
    ~top~
    <TABLE border=0 id=hs><TR><TD><font class=ms>*** Топик комнаты: <b>~topic~</b></TD></TR></TABLE>
    <TABLE border=0 id=hs><TR><TD><font class=ms>*** Поставил(а) <b>~who~</b>, ~time~</b></TD></TR></TABLE>
    ~etop~
    <div class='sysmsg'>~msg~</div>
        const chat_channel_data &ch=channel_ref(c);
        {

            if (ch.topic.size()){
                m["~top~"]="";
                m["~etop~"]="";

                m["~topic~"]=ch.topic;
                m["~topic2~"]=ch.topic;
                m["~who~"]=ch.set_by;
                m["~time~"]=datetime2rus(ch.topic_time);
            }else{
                m["~top~"]="<!--";
                m["~etop~"]="-->";
                m["~topic~"]="";
                m["~topic2~"]="";
                m["~who~"]="";
                m["~time~"]="";
            }
        }
    <TABLE border=0 id=hs><TR><TD><font class=ms>*** Топик комнаты: <b>~topic~</b></TD></TR></TABLE>
    <TABLE border=0 id=hs><TR><TD><font class=ms>*** Поставил(а) <b>~who~</b>, ~time~</b></TD></TR></TABLE>

    */


//	const chat_channel_data &ch=channel_ref(c);

    char s[1000];
    string out="\r\n\r\n"+RVfiles(m,"chat/messages2");
//	if (ch.topic.size()){
    snprintf(s,sizeof(s),"<script>parent.set_topic('%s','%s','%s');</script>\n",MES(ch.topic).c_str(),MES(ch.set_by).c_str(),MES(datetime2rus(ch.topic_time)).c_str());
    out+=s;
//	}


    snprintf(s,sizeof(s),"<script>parent.set_uid(%d);</script>\n",CONTAINER(__u.dbid));
    klap_item bit;
    if (_klap_stuff.check_ban_by_uid(c,uid,bit))
    {

        snprintf(s,sizeof(s),"У вас кляп на канал. Время %s, за %s, до %s",bit.time_cnt_str.c_str(),bit.descr.c_str(),date2mysql(bit.time_ban_end).c_str());
        string msg=s;
        send_syspriv_msg(e->__U,msg);
    }
    out+="<script>parent.set_status("+itostring(CONTAINER(__u.dbid))+","+itostring(__u.cstate.status.pic)+",'"+ES(__u.cstate.status.name)+"');</script>\n";

    int cidx=0;
    for (int i=0; i<chs.size(); i++)
    {
        if (chs[i]==c) cidx=i;
    }
    int status_idx=0;
    {
        PGLW(user_profile,u,user$profiles.FindByID(CONTAINER(uid)));
        for (map<unsigned int,user_status>::iterator i=u->status.begin(); i!=u->status.end(); i++,status_idx++)
        {

            if (i->second.id==__u.cstate.status.id)
            {
                break;
            }
        }
    }
    out+="<script>parent.set_my_status("+itostring(/*__u.cstate.status.id*/status_idx-2)+");</script>\n";
    if (old_channel_us!=__u.cstate.__channel_us)
        update_my_contact_list_nofix_www_list(__u_client_instance_p,c,T_WWW);

    out+=LK_get_users$count_for_web();
    out+="<script>parent.set_channel("+itostring(cidx)+");</script>\n";
    out+="<script>parent.set_moderated("+itostring(ch.moderated)+");</script>\n";
    out+=LK_chat_channel_get_n_messages(c,__u,100);
    string LK="<script>parent.UpdateUsers(["+LK_get_java_users(c,__u,__u_client_instance_p.___ptr->ci)+"]);</script>\n";
    out+=LK;



    data_push_back_www(e->__U,out+SCROLL);
    {
        PGLW(chat_user,u,e->__U);
        u->www_offline_status_sent=false;
        u->www_inchat=true;
        u->www_offline_time=0;

    }

    XPASS;
}

void chat_thread_stuff::on_INVITE(chat_event_INVITE *e)
{
    MUTEX_INSPECTOR;
    map<string,string> m;
    if (e->CONTAINER(zid)==0 ||e->CONTAINER(uid)==0) throw cError("--Error: on_INVITE uid zid 0"+_DMI()); ;

    PTR_getter_nm<chat_stuff_user_p> __pu=get_user_p(e->uid);
    PTR_getter_nm<chat_stuff_user_p> __pz=get_user_p(e->zid);

    chat_stuff_user &__z=__pz.___ptr->ud;
    const chat_stuff_user &__u=__pu.___ptr->ud;

    string msg=e->msg;
    set<CH_id> sc=__u.cstate.get_channel(T_WWW);
    if (sc.size()!=1) return;
    CH_id c=*sc.begin();

    if (msg.size()>1024) msg=msg.substr(0,1024);
    {
        if (!__u.__privileges.count("$tags")) msg=remove_tags(msg);
        msg=replace_links(msg);
    }
    bool may_invite=true;
    bool timeok=true;
    if (__z.ignores.count(e->uid)) {
        may_invite=false;
    }
    if (time(NULL)-__z.tmp$last_invite_time<30) timeok=false;
    __z.tmp$last_invite_time=time(NULL);

    if (may_invite) {
        if (__z.cstate.status.disable_invite || !timeok) {
            if (msg.size()) {


                if (!__z.ignores.count(__u.dbid)) {
                    chat_msg_PRIV*p1=new chat_msg_PRIV(c,e->uid,e->zid,__u.cstate.nick,__z.cstate.nick,msg,__u.n_color_pack);
                    LEAKCTL_ADD(p1);
                    chat_msg*p2=new chat_msg(p1);
                    LEAKCTL_ADD(p2);
                    PTR_getter_nm<chat_msg> cm=p2;
                    save_chat_msg(cm);
                    PTR_getter<chat_user> __ZZ=local_users.get$(e->zid,T_WWW);
                    if (__ZZ)
                        data_push_back_www(__ZZ,cm.___ptr->data->make(c,e->zid)+SCROLL);


                }
            }
            string mmsg;
            if (!timeok) {
                mmsg=_MSG("cantinvitewaitplz");
            } else {
                char ss[200];
                snprintf(ss,sizeof(ss)-1,_MSG("invitedisabled").c_str(),__z.cstate.nick.name.c_str());
                mmsg=ss;
            }
            PTR_getter<chat_user> __UUw=local_users.get$(e->uid,T_WWW);
            PTR_getter<chat_user> __UUi=local_users.get$(e->uid,T_IRC);
            if (__UUw)send_syspriv_msg(__UUw,mmsg);
            if (__UUi)send_syspriv_msg(__UUi,mmsg);

        }
        else {
            msg=str_replace("\'", "\\\'", msg);


            PTR_getter<chat_user> __ZZi=local_users.get$(e->zid,T_IRC);
            PTR_getter<chat_user> __ZZw=local_users.get$(e->zid,T_WWW);


            if (__ZZw)
            {

                string str="<SCRIPT>parent.DoInvite('"+itostring(e->CONTAINER(uid))+"','"+__u.cstate.nick.name+"','"+__z.cstate.nick.name+"','"+msg+"','"+strtime(time(NULL))+"'); </script>\n";
                data_push_back_www(__ZZw,str);
            }
            if (__ZZi)
            {

                if (msg.size()) {
                    irc_send_notice(__ZZi,"Invites you: "+msg,__u.cstate.nick.name);
                } else {
                    irc_send_notice(__ZZi,"Invites you!",__u.cstate.nick.name);
                }
            }



        }
    }
}




bool chat_thread_stuff::user_process_send_message_priv(const DB_id& uid, const DB_id&  zid, string ms, string tonick, bool usersys)
{
    MUTEX_INSPECTOR;
    st_update stu;
    if (CONTAINER(zid)==0 ||CONTAINER(uid)==0) throw cError("--Error: user_process_send_message_priv uid 0 || zid 0"+_DMI());

    char cc02[3] = {0x02,0x02,0x02};
    char cc1f[3] = {0x1f,0x1f,0x1f};
    string ms2=ms;
    PTR_getter_nm<chat_stuff_user_p> __pu=get_user_p(uid);
    PTR_getter_nm<chat_stuff_user_p> __pz=get_user_p(zid);

    chat_stuff_user &__z=__pz.___ptr->ud;
    chat_stuff_user &__u=__pu.___ptr->ud;


    if (__u.contact_options & user_profile::CONTACT_ADD_OUT_PRIV)
    {

        if (!__u.contacts.count(zid))
        {
            __send_update_d3(contact$add$3,CONTAINER(uid),CONTAINER(zid));
            __u.contacts.insert(zid);
            PTR_getter<chat_user> __UU=local_users.get$(make_pair(uid,T_WWW));
            if (__UU)
            {
                chat_event_UPDATE_CONTACT_LIST *e=new chat_event_UPDATE_CONTACT_LIST(__UU,T_WWW);
                LEAKCTL_ADD(e);
                send_chat_event_push_back(e);
            }

        }
    }
    if (__z.contact_options & user_profile::CONTACT_ADD_IN_PRIV)
    {
        if (!__z.contacts.count(uid))
        {
            __send_update_d3(contact$add$3,CONTAINER(zid),CONTAINER(uid));
            __z.contacts.insert(uid);
            PTR_getter<chat_user> __UU=local_users.get$(make_pair(uid,T_WWW));
            if (__UU)
            {
                chat_event_UPDATE_CONTACT_LIST *e=new chat_event_UPDATE_CONTACT_LIST(__UU,T_WWW);
                LEAKCTL_ADD(e);
                send_chat_event_push_back(e);
            }
        }
    }
    if (__u.cstate.channel_us_size()==0) return false; // !inchat

    if (ms.size()>__u.c$level->max_msg_len)return false;
    if (remove_spaces(ms) == "") return false;
    char c160 = 0xa0;
    if (ms.find(c160, 0) != -1) return false;

    string c02=string(cc02,1);
    string c1f=string(cc1f,1);

    if (__u.level/sub_levels < am_admin) usersys = false;

    if (ms == strupper(ms)) ms = strlower(ms);

    if (!__u.__privileges.count("$tags"))    ms = remove_tags(ms);


    map<string,string> M;
    map<string,string> M2;
    if (__u.level/sub_levels >= am_admin) {
        M["&lt;br&gt;"]="<br>";
        M["&lt;BR&gt;"]="<BR>";
        M["&lt;/br&gt;"]="</br>";
        M["&lt;/BR&gt;"]="</BR>";
        M["&lt;u&gt;"]="<u>";
        M["&lt;/u&gt;"]="</u>";
    }
    if (__u.level/sub_levels >= 3) {
        M["&lt;i&gt;"]= "<i>";
        M["&lt;/i&gt;"]="</i>";
        M["&lt;I&gt;"]="<i>";
        M["&lt;/I&gt;"]="</i>";
    }
    {
        M2["<u>"]=c1f;
        M2["</u>"]=c1f;
        M2["<b>"]=c02;
        M2["</b>"]=c02;
        M2["<U>"]=c1f;
        M2["</U>"]=c1f;
        M2["<B>"]=c02;
        M2["</B>"]=c02;
        ms2=replace_vals(M2,ms2);

    }
    ms=replace_vals(M,ms);
    ms=fix_www_irccodes(ms);
    ms=replace_links(ms);
    if (__u.c$level->replacements.size())
    {
        ms = replace_smiles(__u.c$level->replacements, ms);
    }

    if (usersys)
    {
        try {
            set<CH_id> usc=__u.cstate.get_channel(T_WWW);
            set<CH_id> zscW=__z.cstate.get_channel(T_WWW);
            set<CH_id> zscI=__z.cstate.get_channel(T_IRC);
            set<CH_id> zsc;
            for (set<CH_id>::const_iterator i=zscW.begin(); i!=zscW.end(); i++)
                zsc.insert(*i);
            for (set<CH_id>::const_iterator i=zscI.begin(); i!=zscI.end(); i++)
                zsc.insert(*i);

            PTR_getter<chat_user> __Uw=local_users.get$(uid,T_WWW);
            PTR_getter<chat_user> __Ui=local_users.get$(uid,T_IRC);
            PTR_getter<chat_user> __Zi=local_users.get$(zid,T_IRC);
            PTR_getter<chat_user> __Zw=local_users.get$(zid,T_WWW);

            if (usc==zsc && zsc.size())
            {
                chat_msg_SYSPRIV*p1=new chat_msg_SYSPRIV(*usc.begin(),uid,zid,__z.cstate.nick.name,ms);
                LEAKCTL_ADD(p1);
                chat_msg*p2=new chat_msg(p1);
                LEAKCTL_ADD(p2);
                PTR_getter_nm<chat_msg> e=p2;
                if (__Uw)data_push_back_www(__Uw,e.___ptr->data->make(*usc.begin(),uid)+SCROLL);
                if (__Ui)irc_send_notice(__Ui,ms2,"System");
                if (__Zw)data_push_back_www(__Zw,e.___ptr->data->make(*usc.begin(),zid)+SCROLL);
                if (__Zi)irc_send_notice(__Zi,ms2,"System");
                save_chat_msg(e);
            }
            else
            {
                if (usc.size())
                {
                    chat_msg_SYSPRIV *p1=new chat_msg_SYSPRIV(*usc.begin(),uid,zid,__z.cstate.nick.name,ms);
                    LEAKCTL_ADD(p1);
                    chat_msg*p2=new chat_msg(p1);
                    LEAKCTL_ADD(p2);
                    PTR_getter_nm<chat_msg> e=p2;
                    if (__Uw)
                    {
                        data_push_back_www(__Uw,e.___ptr->data->make(*usc.begin(),uid)+SCROLL);
                    }
                    if (__Ui)
                    {
                        irc_send_notice(__Ui,ms2,"System");
                    }

                    save_chat_msg(e);
                }
                if (zsc.size())
                {
                    chat_msg_SYSPRIV*p1=new chat_msg_SYSPRIV(*zsc.begin(),uid,zid,__z.cstate.nick.name,ms);
                    LEAKCTL_ADD(p1);
                    chat_msg*p2=new chat_msg(p1);
                    LEAKCTL_ADD(p2);
                    PTR_getter_nm<chat_msg> e=p2;
                    if (__Zw)
                    {
                        data_push_back_www(__Zw,e.___ptr->data->make(*zsc.begin(),zid)+SCROLL);
                    }
                    if (__Zi)
                    {
                        irc_send_notice(__Zi,ms2,"System");
                    }

                    save_chat_msg(e);
                }
            }
        }
        catch (cError e)
        {
            logErr2("catched %s %s %d",e.what(),__FILE__,__LINE__);
        }
        catch (...)
        {
            logErr2("catched %s %d",__FILE__,__LINE__);
        }

    }
    else
    {

        set<CH_id> usc=__u.cstate.get_channel(T_WWW);
        set<CH_id> zscW=__z.cstate.get_channel(T_WWW);
        set<CH_id> zscI=__z.cstate.get_channel(T_IRC);
        set<CH_id> zsc;
        for (set<CH_id>::const_iterator i=zscW.begin(); i!=zscW.end(); i++)
            zsc.insert(*i);
        for (set<CH_id>::const_iterator i=zscI.begin(); i!=zscI.end(); i++)
            zsc.insert(*i);
        if (usc==zsc && zsc.size())
        {
            chat_msg_PRIV*p1=new chat_msg_PRIV(*usc.begin(),uid,zid,__u.cstate.nick,__z.cstate.nick,ms,__u.n_color_pack);
            LEAKCTL_ADD(p1);
            chat_msg*p2=new chat_msg(p1);
            LEAKCTL_ADD(p2);
            PTR_getter_nm<chat_msg> e=p2;
            PTR_getter<chat_user> __Uw=local_users.get$(uid,T_WWW);
            if (__Uw)
            {
                data_push_back_www(__Uw,e.___ptr->data->make(*usc.begin(),uid)+SCROLL);
            }
            if (!__z.ignores.count(uid))
            {
                PTR_getter<chat_user> __Zw=local_users.get$(zid,T_WWW);
                PTR_getter<chat_user> __Zi=local_users.get$(zid,T_IRC);
                if (__Zw) {
                    data_push_back_www(__Zw,e.___ptr->data->make(*usc.begin(),zid)+SCROLL);
                }
                if (__Zi) {
                    irc_send_private_message(__Zi,ms2,__u.cstate.nick.name);
                }
            }
            save_chat_msg(e);

        }
        else
        {
            if (usc.size())
            {
                chat_msg_PRIV*p1=new chat_msg_PRIV(*usc.begin(),uid,zid,__u.cstate.nick,__z.cstate.nick,ms,__u.n_color_pack);
                LEAKCTL_ADD(p1);
                chat_msg*p2=new chat_msg(p1);
                LEAKCTL_ADD(p2);
                PTR_getter_nm<chat_msg> e=p2;
                PTR_getter<chat_user> __Uw=local_users.get$(uid,T_WWW);
                if (__Uw)
                {
                    data_push_back_www(__Uw,e.___ptr->data->make(*usc.begin(),uid)+SCROLL);
                }

                save_chat_msg(e);
            }
            if (zsc.size())
            {
                chat_msg_PRIV*p1=new chat_msg_PRIV(*zsc.begin(),uid,zid,__u.cstate.nick,__z.cstate.nick,ms,__u.n_color_pack);
                LEAKCTL_ADD(p1);
                chat_msg*p2=new chat_msg(p1);
                LEAKCTL_ADD(p2);
                PTR_getter_nm<chat_msg> e=p2;
                if (!__z.ignores.count(uid))
                {
                    {
                        PTR_getter<chat_user> __Zw=local_users.get$(zid,T_WWW);
                        if (__Zw) {
                            data_push_back_www(__Zw,e.___ptr->data->make(*zsc.begin(),zid)+SCROLL);
                        }
                    }
                    {
                        PTR_getter<chat_user> __Zi=local_users.get$(zid,T_IRC);
                        if (__Zi)
                        {
                            irc_send_private_message(__Zi,ms2,__u.cstate.nick.name);
                        }
                    }
                }
                save_chat_msg(e);

            }
        }



    }
    return true;

}
void chat_thread_stuff::push_back_channel_msg(const PTR_getter_nm<chat_msg> &e,const CH_id& ch,const DB_id& from_id,const DB_id& to_id,const string& ircd,const string& ircdto)
{
    MUTEX_INSPECTOR;
#ifdef UNOPT
    if (!__channels$.count(ch)) throw cError("if(!__channels$.count(ch)) "+_DMI());
#endif
    map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> > &ccc=__local_users_in_channels$[ch];
    for (map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> >::const_iterator i=ccc.begin(); i!=ccc.end(); i++)
    {
        if (i->first.first!=from_id)
        {
            chat_stuff_user &__z=i->second.___ptr->ci.usr.___ptr->ud;;
            if (__z.ignores.count(from_id))continue;
        }
        client_instance &ci=i->second.___ptr->ci;
        if (i->first.second==T_WWW)
        {

            data_push_back_www(i->second.___ptr->ci.__U,e.___ptr->data->make(ch,i->first.first)+SCROLL);
        }
        else if (i->first.second==T_IRC)
        {
            if (i->first.first==from_id && i->first.second==T_IRC) continue;
            if (to_id==i->first.first)
            {

                data_push_back_irc(i->second.___ptr->ci.__U,ircdto);
            }
            else
            {

                data_push_back_irc(i->second.___ptr->ci.__U,ircd);
            }

        }
    }
}


string fix_www_irccodes(const string& ms)
{
    string msout;
    {
        bool bold=false;
        bool underline=false;
        for (unsigned i=0; i<ms.size();)
        {
            if (ms[i]==0x02)
            {
                if (!bold)
                {
                    msout+="<B>";
                    bold=true;
                }
                else
                {
                    msout+="</B>";
                    bold=false;
                }
                i++;
                continue;
            }
            else if (ms[i]==0x1f)
            {
                if (!underline)
                {
                    msout+="<U>";
                    underline=true;
                }
                else
                {
                    msout+="</U>";
                    underline=false;
                }
                i++;
                continue;

            }
            else if (ms[i]==15)
            {
                i++;
                continue;
            }
            else if (ms[i]==0x03)
            {
                i++;
                while (i<ms.size() && isdigit(ms[i])) i++;
                continue;
            }
            else
            {
                msout+=ms[i];
                i++;
            }
        }

    }

    return msout;

}
bool chat_thread_stuff::user_process_send_message_to_channel(const DB_id& uid,const CH_id& __channel,const string &__ms,
        const string& tonick, bool makeit,bool usersys)
{
    MUTEX_INSPECTOR;
    XTRY;
    st_update stu;
    string ms=__ms;
    CH_id channel=__channel;

    string nmsg = ms;
    string ms2;

    if (CONTAINER(uid)==0) throw cError("--Error: user_process_send_message_to_channel uid 0"+_DMI());

    PTR_getter_nm<chat_stuff_user_p> __pu=get_user_p(uid);
    chat_stuff_user &__u=__pu.___ptr->ud;

    const chat_channel_data &__c=channel_cref(channel);

    char c160 = 0xa0;

    char cc02[3] = {0x02,0x02,0x02};
    char cc1f[3] = {0x1f,0x1f,0x1f};
    string c02;
    c02.assign(cc02,1);
    string c1f;
    c1f.assign(cc1f,1);
    bool _is_inv=is_inv(__u);
    if (__u.tmp$last_message==ms) return false;
    __u.tmp$last_message=ms;

    if ( remove_spaces(ms) == "" || ms.size() > __u.c$level->max_msg_len)
    {
        return false;
    }

    if (__u.level/sub_levels < am_admin) {
        usersys = false;
    }
    if (ms == strupper(ms))
        ms = strlower(ms);
    if (!makeit && !usersys && strupper(tonick) != to_all && tonick != "") {
        ms = tonick + ", " + ms;
    }
    ms2 = ms;
    if (!__u.__privileges.count("$tags")) {
        ms = remove_tags(ms);


    }
    map<string,string>M;
    map<string,string>M2;
    if (__u.level/sub_levels >= am_admin) {
        M["&lt;br&gt;"]="<br>";
        M["&lt;BR&gt;"]="<BR>";
        M["&lt;/br&gt;"]="</br>";
        M["&lt;/BR&gt;"]="</BR>";
        M["&lt;u&gt;"]="<u>";
        M["&lt;/u&gt;"]="</u>";
    }
    if (__u.level/sub_levels >= 3) {
        M["&lt;i&gt;"]= "<i>";
        M["&lt;/i&gt;"]="</i>";
        M["&lt;I&gt;"]="<i>";
        M["&lt;/I&gt;"]="</i>";
    }
    {
        M2["<u>"]=c1f;
        M2["</u>"]=c1f;
        M2["<b>"]=c02;
        M2["</b>"]=c02;
        M2["<U>"]=c1f;
        M2["</U>"]=c1f;
        M2["<B>"]=c02;
        M2["</B>"]=c02;
        ms2=replace_vals(M2,ms2);
    }
    ms=replace_vals(M,ms);
    ms=fix_www_irccodes(ms);
    ms=replace_links(ms);

    if (__u.c$level->replacements.size()&& __c.smiles_allowed)
    {
        ms = replace_smiles(__u.c$level->replacements, ms);
    }

    if ((_is_inv)  && !usersys) {
        send_syspriv_msg(uid,"Вы в инвизибле, и не можете писать простые мессаги в чат.");
        return false;
    }



    {
        //check message for maty
        if (__c.moderated && !__u.cstate.voice.count(channel)) {
            send_syspriv_msg(uid,"Невозможно отправить сообщение, потому что комната модерируемая...");
            return false;
        }
        if (__u.level<__c.write_level)
        {
            send_syspriv_msg(uid,"Невозможно отправить сообщение, потому что ваш уровень слишком мал...");
            return false;
        }

        if (__c.censored) {
            if (!check_maty(ms))
            {
                logErr2("check_maty");
                if (__c.move) {

                    channel=__c.move_to_channel;
                    PTR_getter<chat_user> __U=local_users.get$(uid,T_WWW);
                    if (__U)
                    {

                        string s=(string)"<script>parent.set_channel('" +
                                 itostring(CONTAINER(__c.move_to_channel)) +
                                 "'); window.top.messages.location='messages?r=" +
                                 itostring(rand()) + "';</script>\n";
                        s="<script>parent.ch_room_on_maty("+itostring(CONTAINER(__c.move_to_channel))+")</script>";
                        data_push_back_www(__U,s);
                    }
                    if (__u.cstate.get_channel(T_WWW).count(channel))
                    {

                        out_oscar_buffer o;
                        o<<__c.move_to_channel<<T_WWW;
                        string ooo=o.as_string();
                        oscar_buffer oo(ooo.data(),ooo.size());
                        logErr2("CHANGE CHANNEL %s %d",__FILE__,__LINE__);
                        do_set_user_state(user_state::CHANGE_CHANNEL,__pu,oo);
                        chat_event_SET_user_state ee;
                        ee.msg=o.as_string();
                        ee.cmd=user_state::CHANGE_CHANNEL;
                        ee.uid=uid;
                        __send_update_d3(user_profile$last_channel$3,CONTAINER(uid),__c.move_to_channel);
                        global_state.set_user_state(__u.dbid,__u.cstate);

                    }

                }
                else {
                    int nidx=msgs_maty.get_random_n(__u.gender);

                    chat_msg_MAT*p1=new  chat_msg_MAT(channel,__u.gender,nidx,__u.cstate.nick,__u.n_color_pack);
                    LEAKCTL_ADD(p1);
                    chat_msg*p2=new chat_msg(p1);
                    LEAKCTL_ADD(p2);
                    PTR_getter_nm<chat_msg> e=p2;
                    string s;
                    if (__u.gender==0)s=_MSG("saymat0");
                    if (__u.gender==1)s=_MSG("saymat1");
                    if (__u.gender==2)s=_MSG("saymat2");
                    save_chat_msg(e);
                    char ss[300];
                    snprintf(ss, sizeof(ss) - 1, s.c_str(), __u.cstate.nick.name.c_str());
                    DB_id to_id;
                    CONTAINER(to_id)=0;//empty
                    DB_id from_id;
                    CONTAINER(from_id)=0;//empty
                    string ircd=s_irc_send_system_message_to_channel(ss,"#"+__c.irc_name);
                    push_back_channel_msg(e,channel,from_id,to_id,ircd,"");

                    return false;
                }

            }
        }

        if (makeit)
        {
            chat_msg_MAKEIT*p1=new chat_msg_MAKEIT(channel,uid,__u.cstate.nick,ms,__u.n_color_pack);
            LEAKCTL_ADD(p1);
            chat_msg*p2=new chat_msg(p1);
            LEAKCTL_ADD(p2);
            PTR_getter_nm<chat_msg> e=p2;
            save_chat_msg(e);
            DB_id to_id;
            CONTAINER(to_id)=0;//empty
            string ircd=s_irc_send_action_to_channel(ms2,__u.cstate.nick.name,"#"+__c.irc_name);
            push_back_channel_msg(e,channel,uid,to_id,ircd,"");

        }
        else if (usersys)
        {
            chat_msg_SYS*p1=new chat_msg_SYS(channel,uid,ms);
            LEAKCTL_ADD(p1);
            chat_msg*p2=new chat_msg(p1);
            LEAKCTL_ADD(p2);
            PTR_getter_nm<chat_msg> e=p2;
            save_chat_msg(e);
            string ircd=s_irc_send_system_message_to_channel(ms2,"#"+__c.irc_name);
            DB_id to_id;
            CONTAINER(to_id)=0;
            push_back_channel_msg(e,channel,uid,to_id,ircd,"");
        }
        else
        {
            char cc2=2;
            DB_id zid;
            CONTAINER(zid)=0;
            string stonick = str_nick(tonick);
            PTR_getter_nm<chat_stuff_user_p>  pzit=find_nick(stonick);
            if (pzit)
            {
                zid=pzit.___ptr->ud.dbid;
            }
            else
                CONTAINER(zid)=0;

            chat_msg_STDTO*p1=new chat_msg_STDTO(channel,uid,zid,__u.cstate.nick,ms,__u.n_color_pack);
            LEAKCTL_ADD(p1);
            chat_msg*p2=new chat_msg(p1);
            LEAKCTL_ADD(p2);
            PTR_getter_nm<chat_msg> e=p2;

            save_chat_msg(e);
            string ircd=	s_irc_send_message_to_channel(ms2,__u.cstate.nick.name,"#"+__c.irc_name);
            string ircdto=	s_irc_send_message_to_channel(cc2+ms2+cc2,__u.cstate.nick.name,"#"+__c.irc_name);
            push_back_channel_msg(e,channel,uid,zid,ircd,ircdto);

        }

    }
    XPASS;
    return true;
}


void chat_thread_stuff::on_SEND_MESSAGE(chat_event_SEND_MESSAGE *e)
{
    MUTEX_INSPECTOR;
    st_update stu;
    DB_id uid=e->from_uid;
    if (CONTAINER(uid)==0)throw cError("on_SEND_MESSAGE uid 0 || zid 0"+_DMI());;

    PTR_getter_nm<chat_stuff_user_p> __pu=get_user_p(uid);
    chat_stuff_user &__u=__pu.___ptr->ud;
    if (__u.cstate.__channel_us.size()==0) return;
    if (__u.cstate.nick.name=="") return;
    if (__u.level/sub_levels<am_admin)
    {
        // check for flood
        if (__u.tmp$last_m_time==time(NULL)/chat_config.max_msg_period_sec.get()) {//;
            __u.tmp$msg_cnt_per_time++;
            if (__u.tmp$msg_cnt_per_time>chat_config.max_msg_per_period.get() && !__u.__privileges.count("$tags")) {
                for (map<CH_id,set<CT> >::const_iterator i=__u.cstate.__channel_us.begin(); i!=__u.cstate.__channel_us.end(); i++)
                {
                    do_old_ipban(__u.cstate.ip,600, "flood",uid);
                }
                kick_user(e->from_uid,_MSG("floodkick"));
                logErrN("users","User %s (%s) was kicked by System(antiflood protection)",__u.cstate.nick.name.c_str(),__u.login.c_str());
                return;
            }
        }
        else {
            __u.tmp$msg_cnt_per_time=0;
        }

    }

    if (CONTAINER(e->channel))
    {
        XTRY;
        chat_channel_data &c=channel_ref(e->channel);

        string timecnt,za;
        time_t end;
        if (check_klaped(c,__u.dbid,timecnt,za,end))
        {
            vector<PTR_getter<chat_user> > __UU=local_users.get$(e->from_uid);
            for (unsigned i=0; i<__UU.size(); i++)
            {
                if (__UU[i])
                {
                    send_syspriv_msg(__UU[i], "Вы не можете отправить сообщение потому что у вас кляп на этом канале на "+timecnt+" за "+za+" до "+date2rus(end));
                }
            }

            return;
        }
        XPASS;
    }

    if (e->priv)
    {
        string strn=str_nick(e->tonick);
        bool found=true;
        PTR_getter_nm<chat_stuff_user_p> pzit=find_nick(strn);
        if (!pzit)
        {
            vector<PTR_getter<chat_user> > __UU=local_users.get$(e->from_uid);
            for (unsigned i=0; i<__UU.size(); i++)
            {
                if (__UU[i])
                {
                    send_syspriv_msg(__UU[i],"Ник не найден");
                }
//				vector<PTR_getter<chat_user> > __ZZ=local_users.get$(e->to_uid);
            }
        }
        else
        {
            chat_stuff_user &__z=pzit.___ptr->ud;
            if (__u.I_can_see_other(__z))
            {
                user_process_send_message_priv(e->from_uid,pzit.___ptr->ud.dbid,e->msg,e->tonick,e->sys);
            }
            else
            {
                send_syspriv_msg(e->from_uid,"Ник не найден");
            }
        }
    }
    else
    {

        user_process_send_message_to_channel(e->from_uid,e->channel,e->msg,e->tonick,e->makeit,e->sys);
    }
    if (time(NULL)-__u.tmp$last_m_time>1 )
    {

        if (user_message_count.count(uid)) user_message_count[uid]++;
        else user_message_count[uid]=1;
        __u.tmp$last_m_time=time(NULL);
    }
    __u.tmp$last_m_time=time(NULL);

}
m_var<map<string,pair<size_t,string> > > get_irc_list_container;
void chat_thread_stuff::get_irc_list()
{
    MUTEX_INSPECTOR;
    map<string,pair<size_t ,string> > r;

    vector<CH_id>chs=chat_channels._keys();//splitTOK(" ,",chat_config.channel_list.get());

    for (unsigned i=0; i<chs.size(); i++)
    {


        if (!__channels$.count(chs[i]))
        {
            logErr2("if(!__channels$.count(chs[i]))");
            continue;
        }
        map<CH_id,chat_channel_data> ::const_iterator cit=__channels$.find(chs[i]);
        if (cit==__channels$.end()) continue;
        r["#"+cit->second.irc_name].first=__users_in_channels$[cit->first].size();
        r["#"+cit->second.irc_name].second=cit->second.topic;
    }
    get_irc_list_container=r;

}

void chat_thread_stuff::on_CHMODE(chat_event_CHMODE *e)
{
    MUTEX_INSPECTOR;
    XTRY;
    if (e->CONTAINER(uid)==0) throw cError("on_CHMODE uid 0 || zid 0"+_DMI());

#ifdef UNOPT
    if (!__channels$.count(e->channel)) throw cError("if(__channels$.count(c)==0)"+_DMI());
#endif
    PTR_getter_nm<chat_stuff_user_p> __pu=get_user_p(e->uid);
    chat_stuff_user &__u=__pu.___ptr->ud;
    chat_channel_data &c=channel_ref(e->channel);

    string mode;
    if (e->mode==-1)
    {
        if (c.moderated)
        {
            mode="-m";
            c.moderated=false;
        }
        else
        {
            mode="+m";
            c.moderated=true;
        }
    }
    else
    {
        if (e->mode)
        {
            mode="+m";
            c.moderated=true;
        }
        else
        {
            mode="-m";
            c.moderated=false;
        }
    }
    string cmm;
    if (c.moderated) cmm="1";
    else cmm="0";

    {
        __CH_LOCKW(chat_channels[e->channel]);
        CH->cd.moderated=c.moderated;
    }
    st_dbh d;
    d.dbh->real_queryQ((QUERY)"update channels set moderated='?' where id='?'"<<c.moderated<<CONTAINER(e->channel));
    chat_msg_MODE*p1=new chat_msg_MODE(e->channel,e->uid,e->nick,"",mode,rand()%msgs_modechanges.size(),__u.gender,__u.n_color_pack);
    LEAKCTL_ADD(p1);
    chat_msg*p2=new chat_msg(p1);
    LEAKCTL_ADD(p2);
    PTR_getter_nm<chat_msg> m=p2;
    save_chat_msg(m);
    string ircd=":"+e->nick.name+"!"+irc_nick$host(e->nick.name)+" MODE #"+c.irc_name+" "+mode+"\r\n";

    map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> > &ccc=__local_users_in_channels$[e->channel];
    for (map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> >::const_iterator i=ccc.begin(); i!=ccc.end(); i++)
    {

        if (i->first.second==T_WWW )
        {
            string s=m.___ptr->data->make(e->channel,i->first.first);
            s+="<script>parent.set_moderated("+cmm+");</script>\n";
            data_push_back_www(i->second.___ptr->ci.__U,s+SCROLL);
        }
        else if (i->first.second==T_IRC)
        {
            data_push_back_irc(i->second.___ptr->ci.__U,ircd);
        }


    }
    XPASS;
}
void chat_thread_stuff::on_CHVOICE(chat_event_CHVOICE *e)
{
    MUTEX_INSPECTOR;
    XTRY;
    if (e->CONTAINER(uid)==0) throw cError("on_CHVOICE uid 0"+_DMI());

    if (!__channels$.count(e->channel))
    {
        return;
        throw cError("if(__channels$.count(c)==0)"+_DMI());;
    }
    PTR_getter_nm<chat_stuff_user_p> __pu=get_user_p(e->uid);
    chat_stuff_user &__u=__pu.___ptr->ud;

    chat_channel_data &c=channel_ref(e->channel);




    if (__u.level/sub_levels<am_admin) {
        send_syspriv_msg(e->uid,"У Вас слишком маленький уровень!");
        return;
    }

    if (!c.extended)
    {
        send_syspriv_msg(e->uid,"Данная функция доступна только в \"Расширенных\" комнатах...");
        return;
    }

    map<DB_id,PTR_getter_nm<chat_stuff_user_p> >::iterator pit=__users$.find(e->zid);
    if (pit==__users$.end())
    {
        send_syspriv_msg(e->uid,"Ник не найден.");
        return;
    }
    chat_stuff_user &__z=pit->second.___ptr->ud;
    string msg;
    if (e->voice==-1) {
        if (__z.cstate.voice.count(e->channel)) {
            __z.cstate.voice.erase(e->channel);
            global_state.set_user_state(e->uid,__z.cstate);
            msg="-v";
        }
        else {
            __z.cstate.voice.insert(e->channel);
            global_state.set_user_state(e->uid,__z.cstate);
            msg="+v";
        }

    } else {
        if (e->voice)
        {
            __z.cstate.voice.insert(e->channel);
            global_state.set_user_state(e->uid,__z.cstate);
            msg="+v";
        }
        else {
            __z.cstate.voice.erase(e->channel);;
            global_state.set_user_state(e->uid,__z.cstate);
            msg="-v";
        }
    }
    chat_msg_MODE*p1=new chat_msg_MODE(e->channel,e->uid,__u.cstate.nick,__z.cstate.nick.name,msg,rand()%msgs_modechanges.size(),__u.gender,__u.n_color_pack);
    LEAKCTL_ADD(p1);
    chat_msg*p2=new chat_msg(p1);
    LEAKCTL_ADD(p2);
    PTR_getter_nm<chat_msg> m=p2;
    save_chat_msg(m);


    string ircd=(string)":"+__u.cstate.nick.name+"!"+irc_nick$host(__u.cstate.nick.name)+" MODE #"+c.irc_name+" "+msg+" "+__z.cstate.nick.name+"\r\n";
    map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> > &ccc=__local_users_in_channels$[e->channel];
    for (map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> >::const_iterator i=ccc.begin(); i!=ccc.end(); i++)
    {
        if (i->first.second==T_WWW)
        {

            string s=m.___ptr->data->make(e->channel,i->first.first);
            s+=get_remove_string(__z.dbid)+get_add_string(e->channel,i->second.___ptr->ci.usr.___ptr->ud,__z);
            data_push_back_www(i->second.___ptr->ci.__U,s+SCROLL);
        } else if (i->first.second==T_IRC)
        {

            data_push_back_irc(i->second.___ptr->ci.__U,ircd);
        }
    }
    XPASS;
}

void chat_thread_stuff::on_CHTOPIC(chat_event_CHTOPIC *e)
{
    MUTEX_INSPECTOR;
    XTRY;
    if (e->CONTAINER(uid)==0) throw cError("on_CHTOPIC uid 0 "+_DMI());

#ifdef UNOPT
    if (!__channels$.count(e->channel)) throw cError("if(__channels$.count(c)==0)"+_DMI());
#endif


    PTR_getter_nm<chat_stuff_user_p> __pu=get_user_p(e->uid);

    chat_stuff_user& __u=__pu.___ptr->ud;

    chat_channel_data &c=channel_ref(e->channel);
    string timecnt,za;
    time_t end;

    if (check_klaped(c,e->uid,timecnt,za,end))
    {
        vector<PTR_getter<chat_user> > __UU=local_users.get$(e->uid);
        for (unsigned i=0; i<__UU.size(); i++)
        {
            if (__UU[i])
            {
                send_syspriv_msg(__UU[i], "Вы не можете сменить топик потому что у вас кляп на этом канале");
                send_syspriv_msg(__UU[i], "Вы не можете сменить топик потому что у вас кляп на этом канале на "+timecnt+" за "+za+" до "+date2rus(end));

            }

        }

        return;
    }







    string cnick=__u.cstate.nick.name;
    string ircd=":"+cnick+"!"+irc_nick$host(cnick)+" TOPIC #"+c.irc_name+" :"+e->top+"\r\n";
    string webd="<script>parent.set_topic('"+replace_for_js(e->top)+"');</script>\n";
    chat_msg_CHTOPIC*p1=new chat_msg_CHTOPIC(e->channel,__u.gender,rand()%msgs_topicchanges.size(),__u.cstate.nick,e->top,__u.n_color_pack);
    LEAKCTL_ADD(p1);
    chat_msg*p2=new chat_msg(p1);
    LEAKCTL_ADD(p2);
    PTR_getter_nm<chat_msg> m=p2;
    save_chat_msg(m);

    map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> > &ccc=__local_users_in_channels$[e->channel];
    for (map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> >::const_iterator i=ccc.begin(); i!=ccc.end(); i++)
    {
        if (i->first.second==T_WWW)
        {

            string s=m.___ptr->data->make(e->channel,i->first.first);
            s+=webd;
            data_push_back_www(i->second.___ptr->ci.__U,s+SCROLL);
        }
        else if (i->first.second==T_IRC)
        {

            data_push_back_irc(i->second.___ptr->ci.__U,ircd);
        }
    }
    XPASS;
}


void chat_thread_stuff::on_SEND_DATA_TO_WWW_USERS(chat_event_SEND_DATA_TO_WWW_USERS*e)
{
    MUTEX_INSPECTOR;
    for (map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> >::const_iterator i=local_clients.begin(); i!=local_clients.end(); i++)
    {

        data_push_back_www(i->second.___ptr->ci.__U,e->msg);
    }
}


void chat_thread_stuff::on_UPDATE_CONTACT_LIST(chat_event_UPDATE_CONTACT_LIST *e)
{
    DB_id uid=user$id(e->__U);
    PTR_getter_nm<chat_stuff_user_p> __pu=get_user_p(uid);
    chat_stuff_user& __u=__pu.___ptr->ud;

    pair<DB_id,CT> lkey=make_pair(uid,e->_ct);
    map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> >::const_iterator it=local_clients.find(lkey);
    if (it==local_clients.end())
    {
        client_instance_p*p=new client_instance_p(e->__U,__pu,e->_ct);
        LEAKCTL_ADD(p);
        local_clients.insert(make_pair(lkey,p));
        it=local_clients.find(lkey);
    }
    it->second.___ptr->ci.__U=e->__U;
    foreach_channel_us(i,__u.cstate)
    {
        if (i->second.size())
            update_my_contact_list_fix_www_list(it->second,i->first,e->_ct);
    }
}

bool u_has_console(const user_pair& __U)
{
    unsigned int level;
    {
        PGLR(user_profile,u,__U.up);
        if (u->__privileges.size()) return true;
        level=u->level;
    }
    if (level>=600) return true;

    return false;
}






static M_HASHMAP<string,DB_id> all_user_nick_2_id;
void local_free_handler__2()
{
    all_user_nick_2_id.clear();

}


DB_id  all_user_nick_2_DB_id(const string & nick)
{
    string s_nick=str_nick(nick);
    if (all_user_nick_2_id.count(s_nick))
    {
        DB_id sess=all_user_nick_2_id[s_nick];
        return sess;
    }
    DB_id a;
    CONTAINER(a)=0;
    return a;

}


void user_register_1_nick(const DB_id& id, const string& nick)
{
    all_user_nick_2_id.set(str_nick(nick),id);
}
void user_unregister_1_nick(const DB_id& id, const string& nick)
{
    all_user_nick_2_id.erase_key(str_nick(nick));
}
void user_register_all_nicks(const DB_id& id)
{

    map<unsigned int,user_nick> n;

    {
        PGLR(nick_vec, u,nick_vecs.find_by_id_create(CONTAINER(id)));
        n=u->u_nicks;

    }

    for (map<unsigned int,user_nick>::const_iterator i=n.begin(); i!=n.end(); i++) {
        string strn=str_nick(i->second.name);
        all_user_nick_2_id.set(strn,id);
    }

}



PTR_getter_nm<chat_stuff_user_p>  chat_thread_stuff::get_user_p(const DB_id& uid)
{

    map<DB_id,PTR_getter_nm<chat_stuff_user_p > >::const_iterator i=__users$.find(uid);
    if (i!=__users$.end())
        return i->second;
    preload_user(uid);
    PTR_getter_nm<chat_stuff_user_p>  __u=__users$.find(uid)->second;
    return __u;

}
void chat_thread_stuff::save_chat_msg(const PTR_getter_nm<chat_msg>& e)
{

    _$messages.push_back(e);

    backup_messages.push_back(e);

}
void chat_thread_stuff::normalize_user_channels_clean(const DB_id& uid)
{
    MUTEX_INSPECTOR;

    for (map<CH_id,chat_channel_data>::const_iterator i=__channels$.begin(); i!=__channels$.end(); i++)
    {
        __users_in_channels$[i->first].erase(uid);
        __local_users_in_channels$[i->first].erase(make_pair(uid,T_WWW));
        __local_users_in_channels$[i->first].erase(make_pair(uid,T_IRC));

    }
}

void chat_thread_stuff::normalize_user_channels(const CT& __ct,const PTR_getter_nm<chat_stuff_user_p>&__pu)
{
    MUTEX_INSPECTOR;
    chat_stuff_user &__u=__pu.___ptr->ud;
    map<CH_id,int> cc;
    foreach_channel_us(i,__u.cstate)
    {
        if (i->second.size()) cc[i->first]=1;
    }
    vector<CT> ct;
    ct.push_back(T_WWW);
    ct.push_back(T_IRC);
    for (map<CH_id,chat_channel_data>::const_iterator i=__channels$.begin(); i!=__channels$.end(); i++)
    {
        set<DB_id> &uic_f=	__users_in_channels$[i->first];
        bool cond_a=uic_f.count(__u.dbid);
        bool cond_b=cc.count(i->first);
        if (cond_a && !cond_b)
        {
            uic_f.erase(__u.dbid);
        }
        if (!cond_a && cond_b)
        {
            uic_f.insert(__u.dbid);
        }


        {
            map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> > &__local_users_in_channels$__c=__local_users_in_channels$[i->first];
            pair<DB_id,CT> p=make_pair(__u.dbid,__ct);
            bool cond_a=__local_users_in_channels$__c.count(p);
            bool cond_b=__u.cstate.channel_us_count_usr(i->first,__ct);
            if (cond_a && !cond_b)
            {
                __local_users_in_channels$__c.erase(p);
            }
            if (!cond_a && cond_b)
            {
                map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> >::const_iterator lcc=local_clients.find(make_pair(__u.dbid,__ct));
                if (lcc==local_clients.end())
                {
                    return;
                    throw cError("FATAL if(lcc==local_clients.end()) "+_DMI());
                }

                __local_users_in_channels$__c.insert(make_pair(p,lcc->second));
            }
        }
    }
}

string ai_val(const string &s, int val)
{
    return "<b>"+s+"</b>: "+itostring(val)+"<br>\n";
}
string ai_val(const string &s, int val1,int val2)
{
    return "<b>"+s+"</b>: "+itostring(val1)+" "+itostring(val2)+"<br>\n";
}
string ai_vals(const string &s, const string& val)
{
    return "<b>"+s+"</b>: "+val+"<br>\n";
}

extern M_DEQUE<string> update_commands;


size_t messages_to_backup_size();


size_t __chat_events_size();

#pragma warning( disable:4267)
#pragma warning( disable:4244)
string sender2_dump();
string c_socket_dump();
string dump_tcp_dial();

void chat_thread_stuff::on_ainf(chat_event_ainf *e)
{
    MUTEX_INSPECTOR;
    map<string,string> m;
    string out="\r\n\r\n"+RVfiles(m,"ainf_template");
    out+="<h3>Globals:</h3><p>";


    out+=ai_val("update_commands",update_commands.size());



    out+=ai_val("messages_to_backup",messages_to_backup_size());

    out+=ai_val("user$profiles",user$profiles.__xx_size());
    out+=ai_val("user$profiles",user$profiles.__xx_size());
    out+=ai_val("user$profiles",user$profiles.__xx_size());
    out+=ai_val("nick_vecs",nick_vecs.__xx_size());
    out+=ai_val("ignore_vecs",ignore_vecs.__xx_size());
    out+=ai_val("contact_vecs",contact_vecs.__xx_size());
    out+=ai_val("user$profiles",user$profiles.__xx_size());
    out+=ai_val("local_users",local_users.size$());
    out+=ai_val("__chat_events",__chat_events_size());

    out+=ai_val("messages_to_backup",messages_to_backup_size());

    out+=ai_val("global_state",global_state.size());

    out+=ai_val("chat_thread_stuff: messages",_$messages.size());

    out+=ai_val("chat_thread_stuff: backup_messages",backup_messages.size());

    out+=ai_val("chat_thread_stuff: __channels$",__channels$.size());
    out+=ai_vals("chat_thread_stuff: __users_in_channels$",__users_in_channels$_dump());

    out+=ai_val("chat_thread_stuff: __users$",__users$.size());
    out+=ai_val("chat_thread_stuff: strnick2__u",strnick2__u.size());

    out+=ai_val("chat_thread_stuff: join_time",join_time.size());
    out+=ai_val("chat_thread_stuff: trash",trash.size());
    out+=ai_val("chat_thread_stuff: local_clients",local_clients.size());
    out+=ai_vals("chat_thread_stuff: __local_users_in_channels$",__local_users_in_channels$_dump());

    out+=ai_val("chat_thread_stuff: greeting_msgs_channels",greeting_msgs_channels.size());
    out+=ai_val("chat_thread_stuff: bye_msgs_channels",bye_msgs_channels.size());
    out+=ai_val("chat_thread_stuff: chroom_msgs_channels",chroom_msgs_channels.size());
    out+=ai_val("chat_thread_stuff: kick_msgs_channels",kick_msgs_channels.size());
    out+=ai_val("chat_thread_stuff: chnick_msgs_channels",chnick_msgs_channels.size());
    out+=ai_val("chat_thread_stuff: update_contact_list_data",update_contact_list_data.size());
    out+=sender2_dump();
    out+=c_socket_dump();
    out+=dump_tcp_dial();
    reply_socket(e->sock,out);




}
#pragma warning( default:4267)
#pragma warning( default:4244)

void us_PART_CHANNEL(const set<CH_id> &chs,const CT& _ct,const DB_id &uid)
{


    out_oscar_buffer o;
    o<<chs<<_ct;
    __set_user_state(user_state::PART_CHANNEL,uid,o);
    global_state.part_channels(chs,_ct,uid);
}
void us_PART_CHANNEL(const CT& _ct,const DB_id &uid)
{

    const set<CH_id> chs;
    out_oscar_buffer o;
    o<<chs<<_ct;
    __set_user_state(user_state::PART_CHANNEL,uid,o);
    global_state.part_channels(chs,_ct,uid);
}

void us_SET_STATUS(const DB_id& uid,const user_status& st)
{
    st_update stu;
    out_oscar_buffer o;
    o<<st;
    __set_user_state(user_state::SET_STATUS,uid,o);
    __send_update_d3(chat_thread$set_status$3,CONTAINER(uid),st);
    global_state.set_status(uid,st);
    st_dbh d;
    d.dbh->real_queryQ((QUERY)"update tbl_users set last_status_id='?' where id=?"<<st.id<<CONTAINER(uid));


}
void us_SET_NICK(const DB_id &uid, const user_nick& nn)
{
    st_update stu;
    out_oscar_buffer o;
    o<<nn;
    __set_user_state(user_state::SET_NICK,uid,o);
    __send_update_d3(user_profile$set_nick$3,CONTAINER(uid),nn);
    st_dbh d;
    d.dbh->real_queryQ((QUERY)"update tbl_users set last_nick='?',last_nick_id='?' where id=?"<<nn.name<<nn.id<<CONTAINER(uid));
    d.dbh->real_queryQ((QUERY)"update nicks set last_usage_date=NOW() where uid=? and id=?"<<CONTAINER(uid)<<nn.id);

    global_state.set_nick(uid,nn);
}


