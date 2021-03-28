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
#include "levels.h"
#include "server_config.h"
#include "tcp_server_data.h"
#include "irc_server_config.h"

#include "irc_utils.h"
#include "irc_session.h"
#include "chat_thread_event.h"
#include "update_cmd.h"
#include "tcp_stats.h"
#include "global_state.h"
#include "des_sid.h"
#include "chat_stuff.h"
#include "user.h"

void flush_user_data(const DB_id&uid);
void disconnect_irc_user(const PTR_getter<chat_user> &__UU);
bool check_for_ipban(const string &ip);

void user_process_irc_command(const PTR_getter<chat_user> &__UU,irc_command &c);
void user_process_irc_command2(const PTR_getter<chat_user> &__UU,irc_command &c);
__irc_sessions irc_sessions;
size_t irc_sessions_size() {
    return irc_sessions.size();
}
void irc_cmd_EYE(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_EYEN(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_JOIN(const PTR_getter<chat_user> &__UU,const PTR_getter<irc_session> &__IS,irc_command &cmd);
void irc_cmd_KICK(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_LIST(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_MODE(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_PRIVMSG(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_WHOIS(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_USERHOST(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_ISON(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_CTTU(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_UTTC(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_EUI(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_PART(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_SYS(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_TOPIC(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_WHO(const PTR_getter<chat_user> &__UU,irc_command &cmd);

void irc_change_nick(const PTR_getter<chat_user> &__UU,const string &nick);



S_id next_session_id();

void irc_server::start(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m)
{
    {


        tcp_linedialog_server::start(pn,prefix,m);
    }
    {


        load_config(pn,prefix,m);
    }
}

irc_server::irc_server()
{


}

void *irc_server::new_connection(const PTR_getter<c_socket>&s)
{


    irc_connection *i=new irc_connection(s,this);
    LEAKCTL_ADD(i);
    return i;
}

irc_connection::~irc_connection()
{
}

irc_command irc_parse_command(const string &s)
{
    irc_command c;
    size_t pz=s.find(" ",0);
    map<string,string> m;
    m["\r"]="";
    m["\n"]="";

    if (pz!=-1) {
        c.command=strupper(s.substr(0,pz));
        c.data=s.substr(pz+1,s.size()-pz-1);
        c.data=replace_vals(m,c.data);
    } else {
        c.command=strupper(s);
        c.command=replace_vals(m,c.command);
    }
    return c;
}


void irc_iterate()
{
    XTRY;
    irc_sessions.iterate();
    XPASS;
}
void __irc_sessions::add(const S_id &sid,const PTR_getter<irc_session> &__IS)
{
    M_LOCK(Mutex);
    container.insert(make_pair(sid,__IS));
}
PTR_getter<irc_session> __irc_sessions::get(const S_id &s)
{
    M_LOCK(Mutex);
    map<S_id,PTR_getter<irc_session> >::const_iterator i=container.find(s);
    if (i!=container.end()) return i->second;
    return NULL;
}
void __irc_sessions::iterate()
{
    XTRY;
    set<S_id> rm;
    map<S_id,PTR_getter<irc_session> > V;
    {
        M_LOCK(Mutex);
        V=container;

    }
    for (map<S_id,PTR_getter<irc_session> > ::const_iterator i=V.begin(); i!=V.end(); i++)
    {
        XTRY;
        PTR_getter<c_socket> __S(NULL);
        PTR_getter<chat_user> __U(NULL);
        if (i->second)
        {
            PGLR(irc_session,u,i->second);
            if (time(NULL)-u->last_pong>irc_server_config.ping_kill_timeout) rm.insert(i->first);
            if (!u->__U && time(NULL)-u->create_time > irc_server_config.login_timeout) rm.insert(i->first);
            __S=u->__S;
            __U=u->__U;
        }

        if (__S)
        {
            PGLW(c_socket,s,__S);
            if (!s->connected())rm.insert(i->first);
        }
        else rm.insert(i->first);
        if (__U)
        {
            PGLR(chat_user,u,__U);
            if (u->erased) rm.insert(i->first);
        }

        // PING PONG
        string spb;
        {
            XTRY;
            PGLW(irc_session,u,i->second);

            if (time(NULL)>=u->last_ping+irc_server_config.ping_time.get()) {
                u->last_ping=time(NULL);
                spb="PING ";
                spb+=string(irc_server_config.host);
                spb+="\r\n";
            }
            XPASS;
        }
        if (spb.size())reply_socket(__S,spb);
        XPASS;
    }
    for (set<S_id>::const_iterator i=rm.begin(); i!=rm.end(); i++)
    {
        erase(*i);
    }
    XPASS;
}
void __irc_sessions::erase(const S_id &sid)
{
    PTR_getter<irc_session> __IS(NULL);
    {
        M_LOCK(Mutex);
        map<S_id,PTR_getter<irc_session> >::const_iterator i=container.find(sid);
        if (i!=container.end()) __IS=i->second;
        container.erase(sid);
    }
    if (!__IS) return;
    PTR_getter<chat_user> __U(NULL);
    PTR_getter<c_socket> __S(NULL);

    {
        PGLR(irc_session,u,__IS);
        __U=u->__U;
        __S=u->__S;
    }
    if (__U)
    {
        DB_id uid;
        CONTAINER(uid)=0;
        CONTAINER(uid)=0;
        {
            PGLR(chat_user,u,__U);
            uid=u->db_id;
        }
        logErr2("us_PART_CHANNEL(T_IRC,%d) %s %d",CONTAINER(uid),__FILE__,__LINE__);
        us_PART_CHANNEL(T_IRC,uid);
        local_users.erase$(uid,T_IRC);
    }

}
PTR_getter<irc_session> __irc_sessions::get_by_dbid(const DB_id& s)
{
    map<S_id,PTR_getter<irc_session> > m;

    {
        M_LOCK(Mutex);
        m=container;
    }
    for (map<S_id,PTR_getter<irc_session> >::const_iterator i=m.begin(); i!=m.end(); i++)
    {
        if (i->second)
        {
            PTR_getter<chat_user> __U(NULL);
            {
                PGLR(irc_session,u,i->second);
                __U=u->__U;
            }
            if (__U)
            {


                PGLR(chat_user,u,__U);
                if (u->db_id==s) return i->second;
            }
        }
    }
    return NULL;
}


pair<string,DB_id> get_pass_by_nick(const string &nick)
{
    MUTEX_INSPECTOR;
    pair<string,DB_id> r;

    st_dbh d;
    vector<string> v=d.dbh->select_1_rowQ((QUERY)"select uid from nicks where str_nick='?' and banned<>'1'"<<str_nick(nick));

    if (v.size())
    {
        if (atoi(v[0]))
        {
            vector<string> v1=d.dbh->select_1_rowQ((QUERY)"select id,pass from tbl_users where id=?"<<v[0]);
            if (v1.size()==2) {
                r.first=v1[1];
                CONTAINER(r.second)=atoi(v1[0]);
            }
        }
    }
    return r;
}


void irc_server::process_command(const string & command, const PTR_getter<c_socket> &__SD)
{
    MUTEX_INSPECTOR;
    st_update stu;

    if (command.size()==0)return;


    string u_key;
    ++tcp_total_accepted_connections;

    {
        MUTEX_INSPECTOR;

        PGLR( c_socket , sd,__SD);
        u_key=sd->extra_user_id;
    }
    S_id st;
    CONTAINER(st)=atoi(u_key);
    PTR_getter<chat_user> __UU(NULL);
    PTR_getter<irc_session> __IS=irc_sessions.get(st);
    string ip;



    if (__SD)
    {
        MUTEX_INSPECTOR;
        PGLR(c_socket,s,__SD);
        ip=s->peer_ipaddress;
    }
    if (!__IS)
    {

        MUTEX_INSPECTOR;

        irc_session* sess=new irc_session;
        LEAKCTL_ADD(sess);
        sess->ip=ip;
        sess->__S=__SD;
        st=sess->session;
        __IS=sess;
        irc_sessions.add(st,__IS);

        {
            MUTEX_INSPECTOR;
            PGLW( c_socket , sd,__SD);
            sd->extra_user_id=itostring(CONTAINER(st));
        }

    }
    else
    {
        MUTEX_INSPECTOR;
        PGLW(irc_session,s,__IS);
        __UU=s->__U;
        s->last_pong=time(NULL);
    }
    try {

        MUTEX_INSPECTOR;
        if (__UU) {
            MUTEX_INSPECTOR;
            PGLW(chat_user, u,__UU);
            u->last_access_time=time(NULL);
        }
        irc_command cmd = irc_parse_command(command);
        if (cmd.command=="") {
            return;
        }
        else if (cmd.command=="PING")
        {
            string host=cmd.data;

            data_push_back(__IS,(string)"PONG "+irc_server_config.host.get()+" "+host+"\r\n");

            return;
        }
        else if (cmd.command=="PONG")
        {
            MUTEX_INSPECTOR;
            PGLW(irc_session, u,__IS);
            u->last_pong=time(NULL);
            return;
        }
        else if (cmd.command=="QUIT") {
            MUTEX_INSPECTOR;
            if (!__UU)
            {
                MUTEX_INSPECTOR;
                string nick;
                {
                    MUTEX_INSPECTOR;
                    PGLW(irc_session,s,__IS);
                    nick=s->nick.str;
                }
                if (!nick.size())nick="tipok";

                data_push_back(__IS,"ERROR :Closing Link "+nick+"~1@"+string(irc_server_config.host)+"] ("+cmd.data+")\r\n");
            }
            else
            {
                MUTEX_INSPECTOR;
                user_pair __U(__UU);
                string cnick=get_cnick(__U);
                logErrN("users","User %s (%s) logout from IRC [%s] {%d}",cnick.c_str(),user$login(__U).c_str(),user$irc_ip(__U).c_str(),user$get_spent_time(__U));
                data_push_back_irc(__U.cu,"ERROR :Closing Link "+cnick+"[~1@"+string(irc_server_config.host)+"] ("+cmd.data+")\r\n");
                PTR_getter<c_socket> s(NULL);
                {
                    __UC_LOCK;
                    u->erased=true;
                }
            }
            irc_sessions.erase(st);
            return;
        }
        else if (cmd.command=="NICK")
        {
            MUTEX_INSPECTOR;
            string nick=cmd.data;
            bool too_fast=false;
            bool unregistered=false;
            bool irc_registered=false;

            if (nick.size()>2 && nick.substr(0,1)==":") nick=nick.substr(1,nick.size()-1);
            if (nick.size()>16 || nick.size()<0) {
                irc_send_reply(__IS,432,nick+": Nick is too long");
                return;
            }
            if (!check_nick(nick)) {
                irc_send_reply(__IS,432,nick+" is incorrect Nick");
                return;
            }
            if (__UU)
            {
                MUTEX_INSPECTOR;
                irc_change_nick(__UU,nick);
                {
                    PGLW(irc_session,u,__IS);
                    u->nick.str=nick;
                    u->nick.entered=true;
                }
                return;
            }
            else
            {
                MUTEX_INSPECTOR;
                {
                    PGLW(irc_session,u,__IS);
                    u->nick.str=nick;
                    u->nick.entered=true;
                }
                irc_send_reply(__IS,512,"Authorization required to use Registered nick "+nick);
                irc_send_notice(__IS,"If this is your nickname, type /PASS <password>","System");
            }

        }
        else if (cmd.command=="PASS")
        {
            MUTEX_INSPECTOR;
            {
                MUTEX_INSPECTOR;
                PGLW(irc_session, u,__IS);
                u->pass.str=cmd.data;
                u->pass.entered=true;
            }

            if (__UU)
            {
                MUTEX_INSPECTOR;
                irc_send_notice(__IS,"Already logged, please log off first","System");
                return;
            }

        }
        else if (cmd.command=="USER")
        {

            MUTEX_INSPECTOR;
            string rn;
            size_t pz=cmd.data.find(":",0);
            if (pz!=-1) {
                MUTEX_INSPECTOR;
                rn=cmd.data.substr(pz+1,cmd.data.size()-pz-1);
            }
            if (!rn.size()) {
                MUTEX_INSPECTOR;
                irc_send_reply(__IS,461,"USER: Not enought parameters");
                return;
            }
            {
                MUTEX_INSPECTOR;
                PGLW(irc_session, u,__IS);
                u->user.str=rn;
                u->user.entered=true;
            }


        }
        if (!__UU)
        {
            bool cond=false;
            {
                MUTEX_INSPECTOR;
                PGLW(irc_session, u,__IS);
                if ( u->nick.entered && u->user.entered&& u->pass.entered)
                    cond=true;
            }
            if (cond)
            {
                MUTEX_INSPECTOR;
                string nick;
                string pass;
                {
                    MUTEX_INSPECTOR;
                    PGLW(irc_session, u,__IS);
                    nick=u->nick.str;
                    pass=u->pass.str;

                }
                if (nick=="") {
                    MUTEX_INSPECTOR;
                    irc_send_reply(__IS,431,"No nickname given." );
                    return;
                }

                pair<string,DB_id> r=get_pass_by_nick(nick);
                if (CONTAINER(r.second)==0)
                {

                    irc_send_reply(__IS,465,"Invalid nick, or nick banned.");
                    return;
                }
                st_dbh d;
                {
                    vector<string> bb=d.dbh->select_1_rowQ((QUERY)"select uid,time_cnt,descr from banned_login where uid='?'"<<CONTAINER(r.second));

                    if (bb.size()==3) {
                        irc_send_reply(__IS,465,"You are banned from this server by login. Time: "+bb[1]+". "+bb[2]);
                        disconnect(__IS);
                        return;
                    }
                    DB_id uid=r.second;
                    PTR_getter<user_profile>__US(user$profiles.FindByID(CONTAINER(uid)));
                    unsigned int level=0;
                    {
                        PGLR(user_profile,u,__US);
                        level=u->level;
                    }

                    if (level<am_admin*sub_levels) {
                        if (!check_for_ipban(ip)) {
                            irc_send_reply(__IS,465,"You are banned from this server by IP.");
                            disconnect(__IS);
                            return;
                        }
                    }
                }

                if (strupper(bin2hex(MD5(pass),0))!=strupper(r.first)) {

                    irc_send_reply(__IS,464,"Password incorrect.");
                    return;
                }
                DB_id uid=r.second;
                PTR_getter<chat_user> __Z__=local_users.get$(uid,T_IRC);
                if (__Z__)
                {
                    disconnect_irc_user(__Z__);
                }


                PTR_getter<user_profile>__UP(user$profiles.FindByID(CONTAINER(uid)));
                unsigned int level=0;
                if (__UP) {
                    PGLR(user_profile,u,__UP);
                    level=u->level;
                }

                d.dbh->real_queryQ((QUERY)"update tbl_users set user_agent='irc user' where id='?'"<<CONTAINER(uid));


                flush_user_data(uid);

                if (__UP) {
                    MUTEX_INSPECTOR;

                    chat_user *cu=new chat_user(T_IRC,uid);
                    LEAKCTL_ADD(cu);

                    cu->db_id=uid;
                    cu->__user_profile=__UP;
                    cu->login_date=time(NULL);
                    cu->last_access_time=time(NULL);
                    cu->erased=false;
                    cu->irc_ip=ip;
                    cu->persistent_socket=__SD;


                    string login,cnick=nick;

                    vector<string> nicks;
                    {
                        PGLR(user_profile,u,cu->__user_profile);
                        login=u->login;

                    }

                    {
                        PGLR(nick_vec,u,nick_vecs.find_by_id_create(CONTAINER(uid)));
                        for (map<unsigned int,user_nick>::const_iterator i=u->u_nicks.begin(); i!=u->u_nicks.end(); i++)
                        {
                            nicks.push_back(i->second.name);
                        }
                    }

                    __UU=cu;

                }

                {

                    PGLW(irc_session,u,__IS);
                    u->__U=__UU;
                    u->__S=__SD;
                }

                user_pair __U(__UU);
                init_internal_data(__U);

                user_register_all_nicks(uid);
                {
                    PGLW(irc_session,u,__IS);
                    u->__U=__UU;
                }
                local_users.insert$(__UU);


                user_nick nn;
                {

                    MUTEX_INSPECTOR;
                    PGLW(nick_vec, n,nick_vecs.find_by_id_create(CONTAINER(uid)));
                    for (map<unsigned int,user_nick> ::const_iterator i=n->u_nicks.begin(); i!=n->u_nicks.end(); i++)
                    {
                        if (str_nick(i->second.name)==str_nick(nick))
                        {
                            nn=i->second;
                        }

                    }
                }

                bool allok=false;
                if (nn.id!=user_state::UNDEF)
                {

                    PGLW(user_profile, u,__U.up);
                    u->last_nick=nn.name;
                    u->last_nick_id=nn.id;
                }


                us_SET_NICK(uid,nn);

                __send_update_d3(user_profile$stat_last_ip$3,CONTAINER(uid),ip);
                d.dbh->real_queryQ((QUERY)"update tbl_users set stat_last_ip='?', stat_v_count=stat_v_count+1, stat_last_date=NOW() where id=?"<<ip<<CONTAINER(uid));
                irc_send_welcome(__U);
                return;

            }

        }
        if (__UU)
        {

            MUTEX_INSPECTOR;

            if (cmd.command=="USERHOST") {
                irc_cmd_USERHOST(__UU,cmd);
                return;
            }
            else if (cmd.command=="MODE") {
                irc_cmd_MODE(__UU,cmd);
                return;
            }
            else if (cmd.command=="TOPIC") {
                irc_cmd_TOPIC(__UU,cmd);
                return;
            }
            else if (cmd.command=="EYE") {
                irc_cmd_EYE(__UU,cmd);
                return;
            }
            else if (cmd.command=="EYEN") {
                irc_cmd_EYEN(__UU,cmd);
                return;
            }
            else if (cmd.command=="KICK") {
                irc_cmd_KICK(__UU,cmd);
                return;
            }
            else if (cmd.command=="ISON") {
                irc_cmd_ISON(__UU,cmd);
                return;
            }
            else if (cmd.command=="EUI") {
                irc_cmd_EUI(__UU,cmd);
                return;
            }
            else if (cmd.command=="CTTU") {
                irc_cmd_CTTU(__UU,cmd);
                return;
            }
            else if (cmd.command=="UTTC") {
                irc_cmd_UTTC(__UU,cmd);
                return;
            }
            else if (cmd.command=="PART" || cmd.command=="PARTALL") {
                irc_cmd_PART(__UU,cmd);
                return;
            }
            else if (cmd.command=="JOIN") {
                irc_cmd_JOIN(__UU,__IS,cmd);
                return;
            }
            else if (cmd.command=="SYS") {
                irc_cmd_SYS(__UU,cmd);
                return;
            }
            else if (cmd.command=="PRIVMSG") {
                irc_cmd_PRIVMSG(__UU,cmd);
                return;
            }
            else if (cmd.command=="LIST") {
                irc_cmd_LIST(__UU,cmd);
                return;
            }
            else if (cmd.command=="WHOIS") {
                irc_cmd_WHOIS(__UU,cmd);
                return;
            }
            else if (cmd.command=="WHO") {
                irc_cmd_WHO(__UU,cmd);
                return;
            }
            else irc_send_reply(__UU,421,cmd.command+": Unknown command");

        }
        else
        {
        }
    }
    catch (cError e)
    {
        {
            PTR_getter<irc_session> __ZZ=irc_sessions.get(st);
            if (__ZZ)
            {
                irc_send_reply(__ZZ,421, (string)"Command syntax error: "+e.error_string);
            }
        }

    }
    catch (...) {
        {
            PTR_getter<irc_session> __ZZ=irc_sessions.get(st);
            if (__ZZ)
            {
                irc_send_reply(__ZZ,421, "Command syntax error");
            }
        }
    }
}
void irc_server::on_delete(const PTR_getter<c_socket > &__SD,const string& err_msg)
{
    MUTEX_INSPECTOR;
    if (!__SD) return;
    string u_key;
    {
        PGLR( c_socket , sd,__SD);
        u_key=sd->extra_user_id;
    }
    S_id st;
    CONTAINER(st)=atoi(u_key);
    PTR_getter<irc_session> __IS=irc_sessions.get(st);
    PTR_getter<chat_user>__UU(NULL);
    if (__IS)
    {
        PGLR(irc_session,is,__IS);
        __UU=is->__U;
    }
    if (__UU)
    {
        user_pair __U(__UU);
        logErrN("users","User %s (%s) killed due irc connection lost [%s] {%d}. (%s)", get_cnick(__U).c_str(),
                user$login(__U).c_str(), user$irc_ip(__U).c_str(), user$get_spent_time(__U),err_msg.c_str());
    }
    irc_sessions.erase(st);
}
void irc_server::delete_extra_user_id(const string& kk) {
}
void irc_server::load_config(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m)
{

}
