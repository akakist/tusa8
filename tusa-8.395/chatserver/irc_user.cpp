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
#include "update_cmd.h"
#include "user_profile.h"
#include "user.h"
#include "global_state.h"
#include "admin.h"


void irc_cmd_EYE(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_EYEN(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_JOIN(const PTR_getter<chat_user> &__UU,const PTR_getter<irc_session> &__IS,irc_command &cmd);
void irc_cmd_KICK(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_LIST(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_MODE(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_PRIVMSG(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_WHOIS(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_TOPIC(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_WHO(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_WHO(const PTR_getter<chat_user> &__UU,irc_command &cmd)
{

}
void irc_cmd_TOPIC(const PTR_getter<chat_user> &__UU,irc_command &cmd)
{
    st_update stu;
    int pz=cmd.data.find(":",0);
    string top;
    string cnick;
    user_pair __U(__UU);
    string irc_name;
    if (pz==-1) {

    }
    {
        MUTEX_INSPECTOR;
        if (user$level(__U)/sub_levels<am_vip) {
            irc_send_reply(__U,481,"Permission denied. You not an IRC operator.");
            return;
        }
        cnick=get_cnick(__U);
        if (pz!=-1)
        {
            irc_name=remove_spaces(cmd.data.substr(0,pz));
            if (irc_name.size())
            {
                if (irc_name[0]=='#')
                    irc_name=irc_name.substr(1,irc_name.size()-1);
            }
            top=cmd.data.substr(pz+1,cmd.data.size()-pz-1);
            if (top.size()>128) top=top.substr(0,128);
            top=str_replace("'","",top);
            top=str_replace("\\\\","",top);
        }
        else
        {
            irc_name=remove_spaces(cmd.data);
            if (irc_name.size())
            {
                if (irc_name[0]=='#')
                    irc_name=irc_name.substr(1,irc_name.size()-1);
            }
        }
    }
    CH_id __ch=chat_channels.get_id(irc_name);
    if (pz!=-1)
    {
        __send_update_d5(channel$topic$5,__ch,top,cnick,time(NULL));
        chat_event_CHTOPIC *e=new chat_event_CHTOPIC(__ch,user$id(__U),top);
        LEAKCTL_ADD(e);
        send_chat_event_push_back(e);
    }
    else
    {
        PTR_getter<chat_channel> ML=chat_channels.___get_s(irc_name);
        time_t toptime;
        string setby;
        if (ML) {

            __CH_LOCK(ML);
            top=CH->cd.topic;
            toptime=CH->cd.topic_time;
            setby=CH->cd.set_by;
        }
        if (top.size()) irc_send_reply(__U,332,"#"+irc_name,top);
        data_push_back_irc(__U,":"+string(irc_server_config.host)+" 333 "+cnick+" #"+irc_name+" "+setby+" "+itostring(toptime)+"\r\n");

    }
    return;

}
string strip_special_chars(const string& str)
{
    return str;
    string out;
    for (unsigned i=0; i<str.size(); i++)
    {
        if (str[i]==31) continue;
        if (str[i]==3)
        {
            i++;
            if (i>=str.size())break;
            if (str[i]=='0' || str[i]=='1')
            {
                i+=2;
                continue;
            }
            else
            {
                i+=1;
                continue;
            }
        }
        out+=str[i];
    }
    return out;
}
void irc_cmd_SYS(const PTR_getter<chat_user> &__UU,irc_command &cmd)
{
    MUTEX_INSPECTOR;		//system message

    user_pair __U(__UU);
    vector<string> v=splitTOK(" ",cmd.data);
    if (!v.size())
    {
        irc_send_reply(__U,403,"Not in channel");
        logErr2("403 Not in channel %s %d",__FILE__,__LINE__);
        return;
    }
    string irc_name=v[0];

    if (irc_name.size())
    {
        if (irc_name[0]=='#')
            irc_name=irc_name.substr(1,irc_name.size()-1);
    }

    PTR_getter<chat_channel> ML=chat_channels.___get_s(irc_name);
    if (!ML)
    {
        irc_send_reply(__U,403,"Bad channel");
        return;
    }
    CH_id ch;
    CONTAINER(ch)=0;
    {
        __CH_LOCK(ML);
        ch=CH->cd.chid;
    }

    if (user$level(__U)/sub_levels<am_admin) {
        irc_send_reply(__U,481,"You are not an IRC operator");
        return;
    }
    chat_event_SEND_MESSAGE *e=new chat_event_SEND_MESSAGE(ch,user$id(__UU),"",strip_special_chars(cmd.data),false,true,false);
    LEAKCTL_ADD(e);

    send_chat_event_push_back(e);

    return;
}

void irc_cmd_PART(const PTR_getter<chat_user> &__UU,irc_command &cmd)
{
    MUTEX_INSPECTOR;
    user_pair __U(__UU);
    DB_id uid=user$id(__UU);
    if (!global_state.inchat$irc(uid)) {
        irc_send_reply(__U,403,"Not in channel");
        logErr2("403 Not in channel %s %d",__FILE__,__LINE__);
        return;
    }
    vector<string> v=splitTOK(" ,",cmd.data);
    if (!v.size())
    {
        irc_send_reply(__U,403,"Bad syntax");
        return;

    }
    for (unsigned i=0; i<v.size(); i++)
    {
        string irc_name=v[i];

        if (irc_name.size())
        {
            if (irc_name[0]=='#')
                irc_name=irc_name.substr(1,irc_name.size()-1);
        }


        PTR_getter<chat_channel> ML=chat_channels.___get_s(irc_name);
        if (!ML)
        {
            irc_send_reply(__U,403,"Invalid channel "+v[i] );
            continue;

        }
        CH_id ch;
        CONTAINER(ch)=0;
        {
            __CH_LOCK(ML);
            ch=CH->cd.chid;
        }

        set<CH_id> chs;
        chs.insert(ch);
        logErr2("us_PART_CHANNEL %d %s %d",CONTAINER(uid),__FILE__,__LINE__);
        us_PART_CHANNEL(chs,T_IRC,uid);

    }
}



void irc_cmd_EUI(const PTR_getter<chat_user> &__UU,irc_command &cmd)
{
    MUTEX_INSPECTOR;
    user_pair __U(__UU);
    if (user$level(__U)/sub_levels<am_admin) return;
    st_dbh d;

    string rp;
    DB_id zid=all_user_nick_2_DB_id(cmd.data);

    if (!CONTAINER(zid))return;


    vector<string> v=d.dbh->select_1_rowQ((QUERY)"select last_nick,level,stat_last_ip,login,gender,credit_summa from tbl_users where id=?"<<CONTAINER(zid));


    string znick=			 v[0];
    unsigned int zlevel=atoi(v[1]);
    string IP=				 v[2];
    string zlogin=			 v[3];
    unsigned char zgender=atoi(v[5]);
    Double summa;
    summa.container=atof(v[5].c_str());

    if (v.size()==6)
    {
        rp=
            "NICK:"+znick+";"
            "LOGIN:"+zlogin+";"
            "GENDER:"+itostring(zgender)+";"
            "LEVEL:"+itostring(zlevel)+";";

        rp+="CREDITS:"+ftostring(summa.container)+";"
            "IP:"+IP+";";
        string s="EUI_REPLY "+rp+"\r\n";
        data_push_back_irc(__U.cu,s);
    }
    return;
}

void irc_cmd_ISON(const PTR_getter<chat_user> &__UU,irc_command &cmd)
{
    MUTEX_INSPECTOR;

    chat_event_irc_cmd_ISON *e=new chat_event_irc_cmd_ISON;
    LEAKCTL_ADD(e);
    e->__U=__UU;
    e->nicks=splitTOK(" ",cmd.data);
    send_chat_event_push_back(e);
}
void irc_cmd_USERHOST(const PTR_getter<chat_user> &__UU,irc_command &cmd)
{
    MUTEX_INSPECTOR;
    user_pair __U(__UU);

    string sss;
    irc_send_reply(__U,302,sss<<get_cnick(__U)<<"=+~"<<get_cnick(__U)<<"@"<<user$irc_ip(__U));
    return;


}

void irc_cmd_JOIN(const PTR_getter<chat_user> &__UU,const PTR_getter<irc_session> &__IS,irc_command &cmd)
{
    MUTEX_INSPECTOR;
    user_pair __U(__UU);
    DB_id uid=user$id(__UU);

    string irc_channel=cmd.data;
    time_t last_join;
    string ip;
    {
        PGLR(irc_session,u,__IS);
        last_join=u->last_join;
        ip=u->ip;
    }
    if (time(NULL)<last_join+5) {
        irc_send_reply(__U,405,irc_channel+": Too fast join");
        return;
    }
    {
        PGLW(irc_session,u,__IS);
        u->last_join=time(NULL);
    }
    if (irc_channel.size())
    {
        if (irc_channel[0]=='#')
            irc_channel=irc_channel.substr(1,irc_channel.size()-1);
    }
    irc_channel=trim(irc_channel);

    PTR_getter<chat_channel> ML=chat_channels.___get_s(irc_channel);

    if (!ML) {
        irc_send_reply(__U,403,irc_channel+": No such channel");
        return;
    }
    CH_id ch_id;
    CONTAINER(ch_id)=0;
    {
        __CH_LOCK(ML);
        ch_id=CH->cd.chid;
    }
    klap_item bit;
    if (_klap_stuff.check_ban_by_uid(ch_id,uid,bit))
    {

        irc_send_reply(__IS,465,"You are klapped from this channel. Time: "+bit.time_cnt_str+". "+bit.descr+". End time:"+date2rus(bit.time_ban_end));
        logErr2("JOIN ban_by_uid %d, %s %s",CONTAINER(user$id(__U)),user$login(__U).c_str(),irc_channel.c_str());
        return;

    }
    user_state st=global_state.get_user_state(uid);
    user_status status=get_user_status_by_id(uid,st_online);

    if (st.nick.name=="")
    {
        irc_send_reply(__U,401,st.nick.name,"No such nick/channel.");

        return;

    }
    chat_event_common_JOIN *e=new chat_event_common_JOIN(__UU,ch_id,status,st.nick,T_IRC);
    LEAKCTL_ADD(e);

    send_chat_event_push_back(e);

}
void irc_cmd_WHOIS(const PTR_getter<chat_user> &__UU,irc_command &cmd)
{
    MUTEX_INSPECTOR;

    chat_event_irc_cmd_WHOIS *e=new chat_event_irc_cmd_WHOIS(__UU,cmd.data);
    LEAKCTL_ADD(e);
    send_chat_event_push_back(e);
}
void irc_cmd_PRIVMSG(const PTR_getter<chat_user> &__UU,irc_command &cmd)
{
    MUTEX_INSPECTOR;
    user_pair __U(__UU);
    DB_id uid=user$id(__UU);
    string irc_channel;

    string to;
    string ms;
    size_t pz=cmd.data.find(":");
    if (pz!=-1) {
        to=cmd.data.substr(0,pz);
        to=remove_spaces(to);
    }


    ms=cmd.data.substr(pz+1,cmd.data.size()-pz-1);

    if (!to.size() || !ms.size()) {
        irc_send_reply(__U,401,to,"No such nick/channel.");
        return;
    }	//anti flood protection

    if (!to.size())
    {
        irc_send_reply(__U,411,to,"No recipient given.");
        return;
    }
    bool to_channel=false;
    if (to.size())
    {
        if (to[0]=='#')
        {
            to=to.substr(1,to.size()-1);
            to_channel=true;
        }

    }

    PTR_getter<chat_channel> ML(NULL);
    if (to_channel) {
        ML=chat_channels.___get_s(to);
        if (!ML) {

            string s="No recipient found. '"+to+"'";
            irc_send_reply(__U,411,to,s);
            return;
        }
    }
    DB_id zid;
    CONTAINER(zid)=0;
    if (!to_channel)
    {
        zid=all_user_nick_2_DB_id(to);
        if (!CONTAINER(zid))
        {
            string s="No recipient found. '"+to+"'";
            irc_send_reply(__U,411,to,s);
            return;
        }
        CH_id c;
        CONTAINER(c)=0;
        chat_event_SEND_MESSAGE *e=new chat_event_SEND_MESSAGE(c,uid,to,strip_special_chars(ms),true,false,false);
        LEAKCTL_ADD(e);
        send_chat_event_push_back(e);
        return;

    }

    if (to_channel)
    {
        bool makeit=false;
        if (ms.size()>=7) {	//may be action message
            char cc1=1;
            string act(cc1,1);
            act+="ACTION";
            if (ms.substr(0,7)==act) {
                makeit=true;
                ms=ms.substr(8,ms.size()-9);
            }
        }

        string ton;
        if (!makeit)
        {
            pz=ms.find(",",0);
            if (pz!=-1) {
                ton=ms.substr(0,pz);
                ms=ms.substr(pz+1,ms.size()-1-pz);
            }

        }
        CH_id channel;
        CONTAINER(channel)=0;
        CONTAINER(channel)=0;

        {
            __CH_LOCK(ML);
            channel=CH->cd.chid;
        }

        chat_event_SEND_MESSAGE *e=new chat_event_SEND_MESSAGE(channel,uid,ton,strip_special_chars(ms),false,false,makeit);
        LEAKCTL_ADD(e);

        send_chat_event_push_back(e);

        return;
    }
    if (to.size()>16) {
        irc_send_reply(__U,404,to,"Cant send to channel");
        return;
    }
    if (CONTAINER(zid))
    {

        return;
    }

    irc_send_reply(__U,411,to,"No recipient given.");

}

extern m_var<map<string,pair<size_t,string> > > get_irc_list_container;
void irc_cmd_LIST(const PTR_getter<chat_user> &__UU,irc_command &cmd)
{
    MUTEX_INSPECTOR;
    user_pair __U(__UU);

    map<string,pair<size_t,string> >r= get_irc_list_container.get();
    string cnick=get_cnick(__U);
    for (map<string,pair<size_t,string> >::const_iterator i=r.begin(); i!=r.end(); i++) {
        string ss;
        ss+=":"+string(irc_server_config.host)+" 322 "+cnick+" "+i->first+" "+itostring(i->second.first)+" :"+i->second.second+"\r\n";
        data_push_back_irc(__U.cu,ss);
    }
    irc_send_reply(__U,323,"End of list");
}

