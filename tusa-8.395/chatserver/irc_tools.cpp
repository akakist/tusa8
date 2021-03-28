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
#include "version.h"
#include "irc_server_config.h"
#include "irc_utils.h"
#include "irc_session.h"
#include "global_state.h"
#include "chat_config.h"
extern m_var<time_t> start_date;
string irc_gen_reply(const PTR_getter<chat_user>&__U,int code,const string &text);
string irc_gen_reply(const user_pair&__U,int code,const string &text);


string irc_gen_reply(const user_pair&__U,int code,const string &val,const string &text)
{
    return irc_gen_reply2(code,val,text,string(irc_server_config.host),get_cnick(__U));
}
string irc_gen_reply(const PTR_getter<chat_user>&__U,int code,const string &val,const string &text)
{
    return irc_gen_reply2(code,val,text,string(irc_server_config.host),irc_NICK(__U));
}

void irc_send_reply(const user_pair&__U,int code,const string &val,const string &text)
{

    data_push_back_irc(__U.cu,irc_gen_reply(__U,code,val,text));
}
void irc_send_reply(const PTR_getter<chat_user>&__U,int code,const string &val,const string &text)
{

    data_push_back_irc(__U,irc_gen_reply(__U,code,val,text));
}


void irc_send_reply(const user_pair&__U,int code,const string &text)
{
    MUTEX_INSPECTOR;
    data_push_back_irc(__U.cu,irc_gen_reply(__U,code,text));
}
void irc_send_reply(const PTR_getter<chat_user>&__U,int code,const string &text)
{
    MUTEX_INSPECTOR;
    data_push_back_irc(__U,irc_gen_reply(__U,code,text));
}



void irc_send_welcome(const user_pair&__U)
{

    size_t all_u;
    all_u=global_state.size();

    irc_send_reply(__U,1,string(irc_server_config.msg001)+get_cnick(__U));
    irc_send_reply(__U,2,string(irc_server_config.msg002)+SERVER_VERSION);
    irc_send_reply(__U,3,string(irc_server_config.msg003)+CTIME(start_date));
    irc_send_reply(__U,4,string(irc_server_config.msg004));
    irc_send_reply(__U,254,itostring(chat_channels._keys().size()),string(irc_server_config.msg254));
    char ss[300];
    snprintf(ss,sizeof(ss)-1,string(irc_server_config.msg255).c_str(),all_u,all_u);
    irc_send_reply(__U,255,ss);
    string motd;
    if (load_1_file(motd,string(irc_server_config.motd))==-1) {
        irc_send_reply(__U,422,"MOTD File is missing");
    } else {
        irc_send_reply(__U,375,string(irc_server_config.msg375));
        data_push_back_irc(__U.cu,motd);
        irc_send_reply(__U,376,"End of MOTD command.");
    }
    logErrN("users","%s (%s) logged in from IRC [%s].",get_cnick(__U).c_str(),user$login(__U).c_str(),user$irc_ip(__U).c_str());
}

void irc_send_notice(const user_pair&__U,const string &msg,const string &from)
{
    string s;
    s<<":"<<from<<"!"<<irc_nick$host(from)<<" NOTICE "<<get_cnick(__U)<<" :"<<msg<<"\r\n";
    data_push_back_irc(__U.cu,s);
}
void irc_send_notice(const PTR_getter<chat_user>&__U,const string &msg,const string &from)
{
    string s;
    s<<":"<<from<<"!"<<irc_nick$host(from)<<" NOTICE "<<irc_NICK(__U)<<" :"<<msg<<"\r\n";
    data_push_back_irc(__U,s);
}

void irc_send_private_message(const user_pair&__U,const string &msg,const string &from)
{
    string ms=msg;
    string s;
    s<<":"<<from<<"!"<<irc_nick$host(from)<<" PRIVMSG "<<get_cnick(__U)<<" :";
    if (ms.find("<br>")!=-1) {
        ms=str_replace("<br>","\r\n"+s,msg);
    }
    s<<ms<<"\r\n";
    data_push_back_irc(__U.cu,s);
    logErr2("irc_send_private_message %s",s.c_str());
}

void irc_send_private_message(const PTR_getter<chat_user>&__U,const string &msg,const string &from)
{
    string ms=msg;
    string s;
    s<<":"<<from<<"!"<<irc_nick$host(from)<<" PRIVMSG "<<irc_NICK(__U)<<" :";
    if (ms.find("<br>")!=-1) {
        ms=str_replace("<br>","\r\n"+s,msg);
    }
    s<<ms<<"\r\n";
    data_push_back_irc(__U,s);
}
string s_irc_send_action_to_channel(const string &msg,const string &from, const string& chan)
{
    char cc1=1;
    string s;
    s<<":"<<from<<"!"<<irc_nick$host(from)<<" PRIVMSG "<<chan<<" :";
    s+=cc1;
    s<<"ACTION "<<msg;
    s+=cc1;
    s<<"\r\n";
    return s;
}
string s_irc_send_message_to_channel(const string &msg,const string &from,const string& channel)
{
    string s;
    s<<":"<<from<<"!"<<irc_nick$host(from)<<" PRIVMSG "<<channel<<" :"<<msg<<"\r\n";
    return s;
}

string s_irc_send_system_message_to_channel(const string &msg, const string& channel)
{
    char cc1=1;
    string s;
    s<<":System!system@"<<string(irc_server_config.host)<<" PRIVMSG "<<channel<<" :";
    s+=cc1;
    s<<"ACTION "<<msg;
    s+=cc1;
    s<<"\r\n";
    return s;
}


string irc_gen_reply(const user_pair&__U,int code,const string &text)
{
    return irc_gen_reply2(code,text,string(irc_server_config.host),get_cnick(__U));
}
string irc_gen_reply(const PTR_getter<chat_user>&__U,int code,const string &text)
{
    return irc_gen_reply2(code,text,string(irc_server_config.host),irc_NICK(__U));
}


string irc_gen_reply2(int code,const string &text,const string &irc_host,const string &cnick)
{
    string c=itostring(code);
    if (c.size()==1) c="00"+c;
    if (c.size()==2) c="0"+c;
    string tmp;
    if (!cnick.size()) {
        tmp<<":"<<irc_host<<" "<<c<<" * :"<<text<<"\r\n";
    } else {
        tmp<<":"<<irc_host<<" "<<c<<" "<<cnick<<" :"<<text<<"\r\n";
    }
    return tmp;
}


string irc_gen_reply2(int code,const string &val,const string &text,const string &irc_host,const string &cnick)
{
    string c=itostring(code);
    if (c.size()==1) c="00"+c;
    if (c.size()==2) c="0"+c;
    string tmp;
    if (!cnick.size()) {
        tmp<<":"<<irc_host<<" "<<c<<" * "<<val<<" :"<<text<<"\r\n";
    } else {
        tmp<<":"<<irc_host<<" "<<c<<" "<<cnick<<" "<<val<<" :"<<text<<"\r\n";
    }
    return tmp;
}
string irc_NICK(const PTR_getter<chat_user>&__U)
{
    PTR_getter<user_profile> __US(NULL);
    {
        PGLR(chat_user,u,__U);
        __US=u->__user_profile;
    }
    {
        PGLR(user_profile,u,__US);
        return u->last_nick;
    }
    return "";
}


void irc_send_notice(const PTR_getter<irc_session>&__U,const string &msg,const string &from)
{
    string s;
    {
        PGLR(irc_session,u,__U);
        s<<":"<<from<<"!"<<irc_nick$host(from)<<" NOTICE "<<u->nick.str<<" :"<<msg<<"\r\n";
    }
    data_push_back(__U,s);

}
void data_push_back(const PTR_getter<irc_session> &__IS,const string& s)
{
    PTR_getter<c_socket>__S(NULL);
    {
        PGLR(irc_session,u,__IS);
        __S=u->__S;
    }
    reply_socket(__S,s);
}
string irc_gen_reply(const PTR_getter<irc_session>&__IS,int code,const string &text)
{
    string nick;
    {
        PGLR(irc_session,u,__IS);
        nick=u->nick.str;
    }
    return irc_gen_reply2(code,text,string(irc_server_config.host),nick);
}

void irc_send_reply(const PTR_getter<irc_session>&__IS,int code,const string &text)
{
    data_push_back(__IS,irc_gen_reply(__IS,code,text));
}
void disconnect(const PTR_getter<irc_session> &__IS)
{
    S_id session;
    {
        PGLW(irc_session,u,__IS);
        session=u->session;
        u->__S=NULL;
    }
    irc_sessions.erase(session);
}
