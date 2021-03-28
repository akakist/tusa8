#include "stdafx.h"
#include <string>
#include "PTR.h"
#include "oscar_buffer.h"
#include "str_lib.h"
#include "DB_id.h"
#include "CH_id.h"
#include "chat_msg.h"
#include "chat_colors.h"
#include "str_lib.h"
#include "cm_container.h"
#include "cutils.h"
#include "chat_config.h"
#include "M_DEQUE.h"
#include "chat_stuff.h"
using namespace std;

string _MSG(const string &n);
m_var<unsigned int>chat_msg$::msg_counter(0);

chat_msg$::chat_msg$(M_TYPE t):msgtype(t),msgid(itostring(++msg_counter)) {}


chat_msg_STDTO::~chat_msg_STDTO() {}
chat_msg_PRIV::~chat_msg_PRIV() {}
chat_msg_SYSPRIV::~chat_msg_SYSPRIV() {}
chat_msg_SYS::~chat_msg_SYS() {}
chat_msg_MAKEIT::~chat_msg_MAKEIT() {}
chat_msg_QUIT::~chat_msg_QUIT() {}
chat_msg_KICK::~chat_msg_KICK() {}
chat_msg_CHNICK::~chat_msg_CHNICK() {}
chat_msg_KILLED::~chat_msg_KILLED() {}
chat_msg_GREETING::~chat_msg_GREETING() {}
chat_msg_CHTOPIC::~chat_msg_CHTOPIC() {}
chat_msg_CHROOM::~chat_msg_CHROOM() {}
chat_msg_MAT::~chat_msg_MAT() {}
chat_msg_MODE::~chat_msg_MODE() {}



string chat_msg$::send_message(const string&s)
{

    return "<div id=msg"+msgid+">"+s+"</div>\n"+
           "<script>pm('"+msgid+"');</script>\n";
    //return (string)"<script>parent.append_messages('"+MES(s)+"');</script>\n";
}

string chat_msg_STDTO::make(const CH_id& chan,const DB_id& __id)
{

    if (chan!=channel)return "";
    if (from_id==__id)
    {
        if (prepf.size()) return prepf;

        PTR_getter<__c_pack> cp=c_packs.get(setting_n_color_pack);
        map<string,string>m;

        m["~nick~"]=_nick$(from_nick,false,cp);
        m["~msg~"]=msg;

        string ccc;
        {
            PGLR(__c_pack, c,cp);
            ccc=c->stdto;
        }
        prepf=send_message(replace_vals(m,ccc));
        return prepf;
    }
    else if (to_id==__id)
    {
        if (prept.size()) return prept;
        PTR_getter<__c_pack> cp=c_packs.get(setting_n_color_pack);
        map<string,string>m;
        m["~nick~"]=_nick$(from_nick,false,cp);
        m["~msg~"]=msg;
        string ccc;
        {
            PGLR(__c_pack, c,cp);
            ccc=c->stdfrom;
        }
        prept=send_message(replace_vals(m,ccc));
        return prept;
    }
    else
    {
        if (prepa.size()) return prepa;
        PTR_getter<__c_pack> cp=c_packs.get(setting_n_color_pack);
        map<string,string>m;

        m["~nick~"]=_nick$(from_nick,false,cp);

        m["~msg~"]=msg;
        string ccc;
        {
            PGLR(__c_pack, c,cp);
            ccc=c->std;
        }
        prepa=send_message(replace_vals(m,ccc));
        return prepa;
    }
}

string chat_msg_PRIV::make(const CH_id& __ch,const DB_id& __id)
{

    if (channel!=__ch) return "";
    if (__id==from_id)
    {
        if (prepf.size()) return prepf;
        PTR_getter<__c_pack> cp=c_packs.get(setting_n_color_pack);
        map<string,string>m;
        m["~nick~"]=_nick$(to_nick,true,cp);;
        m["~msg~"]=msg;
        string ccc;
        {
            PGLR(__c_pack, c,cp);
            ccc=c->privto;
        }

        prepf=send_message(replace_vals(m,ccc));
        return prepf;
    }
    else if (__id==to_id)
    {
        if (prept.size()) return prept;
        PTR_getter<__c_pack> cp=c_packs.get(setting_n_color_pack);
        map<string,string>m;
        m["~nick~"]=_nick$(from_nick,true,cp);;
        m["~msg~"]=msg;
        string ccc;
        {
            PGLR(__c_pack, c,cp);
            ccc=c->privfrom;
        }
        prept=send_message(replace_vals(m,ccc));
        return prept;



    }
    return "";
}


string chat_msg_MAKEIT::make(const CH_id& chan,const DB_id& )
{

    if (chan!=channel)return "";
    if (preparation.size()) return preparation;
    PTR_getter<__c_pack> cp=c_packs.get(setting_n_color_pack);
    map<string,string>m;
    string n;
    n=_nick$(from_nick,false,cp);
    m["~nick~"]=n;
    m["~msg~"]=msg;
    string ccc;
    {
        PGLR(__c_pack, c,cp);
        ccc=c->makeit;
    }
    preparation=send_message(replace_vals(m,ccc));
    return preparation;
}
string chat_msg_SYS::make(const CH_id& chan,const DB_id& )
{

    if (chan!=channel)return "";
    if (preparation.size()) return preparation;
    PTR_getter<__c_pack> cp=c_packs.get(0);
    map<string,string>m;
    m["~msg~"]=msg;
    string ccc;
    {
        PGLR(__c_pack, c,cp);
        ccc=c->sys;
    }
    preparation=send_message(replace_vals(m,ccc));
    return preparation;
}
string chat_msg_SYSPRIV::make(const CH_id& __ch,const DB_id& __id)
{

    if (channel!=__ch) return "";
    if (__id==from_id)
    {
        map<DB_id,string>::const_iterator it=preparations.find(__id);
        if (it!=preparations.end())return it->second;
        PTR_getter<__c_pack> cp=c_packs.get(0);
        map<string,string>m;
        if (to_nick.size())
            m["~nick~"]="для "+to_nick;
        else
            m["~nick~"]="";
        m["~msg~"]=msg;
        string ccc;
        {
            PGLR(__c_pack, c,cp);
            ccc=c->syspriv;
        }
        string ret=send_message(replace_vals(m,ccc));
        preparations[__id]=ret;
        return ret;
    }
    else if (__id==to_id)
    {
        map<DB_id,string>::const_iterator it=preparations.find(__id);
        if (it!=preparations.end())return it->second;
        PTR_getter<__c_pack> cp=c_packs.get(0);
        map<string,string>m;
        m["~msg~"]=msg;
        m["~nick~"]="";
        string ccc;
        {
            PGLR(__c_pack, c,cp);
            ccc=c->syspriv;
        }
        string ret=send_message(replace_vals(m,ccc));
        preparations[__id]=ret;
        return ret;
    }
    return "";
}
string chat_msg_GREETING::make(const CH_id& chan,const DB_id& )
{

    if (chan!=channel)return "";
    if (preparation.size()) return preparation;
    PTR_getter<__c_pack> cp=c_packs.get(n_color_pack);
    string s=msgs_greeting.get(gender,n_index);
    string n=_nick$(nick,false,cp);
    map<string,string> q;
    char ss[300];
    tm TM=LOCALTIME(time(NULL));
    int z=strftime(ss,sizeof(ss)-2,string(chat_config.time_format).c_str(),&TM);
    string t(ss,z);

    q["~who~"]=n;
    q["~time~"]=t;
    map<string,string> m;
    m["~msg~"]=replace_vals(q,s);
    string ccc;
    {
        PGLR(__c_pack, c,cp);
        ccc=c->sys;
    }
    preparation=send_message(replace_vals(m,ccc));
    return preparation;
}

string chat_msg_KILLED::make(const CH_id& chan,const DB_id& )
{

    if (chan!=channel)return "";
    if (preparation.size()) return preparation;
    PTR_getter<__c_pack> cp=c_packs.get(n_color_pack);
    string s=msgs_killing.get(gender,n_index);
    string n=_badnick$2(nick,cp);
    char ss[300];
    snprintf(ss,sizeof(ss)-1,s.c_str(),n.c_str());
    map<string,string>m;
    m["~msg~"]=ss;
    string ccc;
    {
        PGLR(__c_pack, c,cp);
        ccc=c->sys;
    }
    preparation=send_message(replace_vals(m,ccc));
    return preparation;

}

string chat_msg_QUIT::make(const CH_id& chan,const DB_id& )
{

    if (chan!=channel)return "";
    if (preparation.size()) return preparation;
    PTR_getter<__c_pack> cp=c_packs.get(n_color_pack);
    string s=msgs_bye.get(gender,n_index);
    string n=_badnick$2(nick,cp);
    map<string,string> q;
    char ss[300];
    tm TM=LOCALTIME(time(NULL));
    int z=strftime(ss,sizeof(ss)-2,string(chat_config.time_format).c_str(),&TM);
    string t(ss,z);
    q["~who~"]=n;
    q["~time~"]=t;
    map<string,string>m;
    m["~msg~"]=replace_vals(q,s);
    string ccc;
    {
        PGLR(__c_pack,c,cp);
        ccc=c->sys;
    }
    preparation=send_message(replace_vals(m,ccc));
    return preparation;

}
string chat_msg_KICK::make(const CH_id& chan,const DB_id& )
{

    if (chan!=channel)return "";
    if (preparation.size()) return preparation;

    PTR_getter<__c_pack> cp=c_packs.get(n_color_pack);
    string s=msgs_kicking.get(gender,n_index);
    string n=_badnick$2(nick,cp);
    string pr;
    if (zamsg.size()) {
        pr=_MSG("za")+" "+zamsg;
    }
    char ss[2000];
    snprintf(ss,sizeof(ss)-1,s.c_str(),n.c_str(),pr.c_str());
    map<string,string>m;
    m["~msg~"]=ss;
    string ccc;
    {
        PGLR(__c_pack, c,cp);
        ccc=c->sys;
    }
    preparation=send_message(replace_vals(m,ccc));
    return preparation;

}
string chat_msg_CHROOM::make(const CH_id& chan,const DB_id& )
{

    if (chan!=channel)return "";
    if (preparation.size()) return preparation;
    PTR_getter<__c_pack> cp=c_packs.get(n_color_pack);
    string s=msgs_changes.get(gender,n_index);
    string n=_badnick$2(nick,cp);
    char ss[300];
    snprintf(ss,sizeof(ss)-1,s.c_str(),n.c_str(),chroomname.c_str());
    map<string,string>m;
    m["~msg~"]=ss;
    string ccc;
    {
        PGLR(__c_pack, c,cp);
        ccc=c->sys;
    }
    preparation=send_message(replace_vals(m,ccc));
    return preparation;

}
string chat_msg_CHNICK::make(const CH_id& chan,const DB_id& )
{

    if (chan!=channel)return "";
    if (preparation.size()) return preparation;

    PTR_getter<__c_pack> cp=c_packs.get(n_color_pack);
    string s=msgs_nickchanges.get(gender,n_index);
    string n=_badnick$(oldnick,cp);
    string n2=_nick$(newnick,false,cp);
    char ss[300];
    snprintf(ss,sizeof(ss)-1,s.c_str(),n.c_str(),n2.c_str());
    map<string,string>m;
    m["~msg~"]=ss;
    string ccc;
    {
        PGLR(__c_pack, c,cp);
        ccc=c->sys;
    }
    preparation=send_message(replace_vals(m,ccc));
    return preparation;

}

string chat_msg_MAT::make(const CH_id& chan,const DB_id& )
{

    if (chan!=channel)return "";
    if (preparation.size()) return preparation;

    PTR_getter<__c_pack> cp=c_packs.get(n_color_pack);
    string s=msgs_maty.get(gender,n_index);

    string n2=_nick$(nick,false,cp);
    char ss[300];
    snprintf(ss,sizeof(ss)-1,s.c_str(),n2.c_str());
    map<string,string>m;
    m["~msg~"]=ss;
    string ccc;
    {
        PGLR(__c_pack, c,cp);
        ccc=c->sys;
    }
    preparation=send_message(replace_vals(m,ccc));
    return preparation;

}
string chat_msg_CHTOPIC::make(const CH_id& chan,const DB_id& )
{

    if (chan!=channel)return "";
    if (preparation.size()) return preparation;

    PTR_getter<__c_pack> cp=c_packs.get(n_color_pack);

    string s=msgs_topicchanges.get(gender,n_index);
    string n;
    n=_nick$(nick,false,cp);

    char ss[300];
    snprintf(ss,sizeof(ss)-1,s.c_str(),n.c_str(),topic.c_str());
    map<string,string> m;
    m["~msg~"]=ss;
    string ccc;
    {
        PGLR(__c_pack, c,cp);
        ccc=c->sys;
    }
    preparation=send_message(replace_vals(m,ccc));
    return preparation;

}
string chat_msg_MODE::make(const CH_id& chan,const DB_id& )
{

    if (chan!=channel)return "";
    if (preparation.size()) return preparation;

    PTR_getter<__c_pack> cp=c_packs.get(n_color_pack);

    string s=msgs_modechanges.get(gender,n_index);
    string n=_nick$(nick1,false,cp);

    char ss[300];
    if (msg=="+v" || msg=="-v") {
        string z=msg+" "+_badnick$(nick2,cp);
        snprintf(ss,sizeof(ss)-1,s.c_str(),n.c_str(),z.c_str());
    } else {
        snprintf(ss,sizeof(ss)-1,s.c_str(),n.c_str(),_MSG("channelmode"+msg).c_str());
    }
    map<string,string>m;
    m["~msg~"]=ss;
    string ccc;
    {
        PGLR(__c_pack, c,cp);
        ccc=c->sys;
    }
    preparation=send_message(replace_vals(m,ccc));
    return preparation;
}

void chat_msg_CHNICK::pack(out_oscar_buffer&o) {
    o<<channel<<gender<<n_index<<oldnick<<newnick;
}
void chat_msg_CHNICK::unpack(oscar_buffer&o) {
    o>>channel>>gender>>n_index>>oldnick>>newnick;
}
void chat_msg_CHROOM::pack(out_oscar_buffer&o) {
    o<<channel<<gender<<n_index<<nick<<chroomname;
}
void chat_msg_CHROOM::unpack(oscar_buffer&o) {
    o>>channel>>gender>>n_index>>nick>>chroomname;
}
void chat_msg_CHTOPIC::pack(out_oscar_buffer&o) {
    o<<channel<<gender<<n_index<<nick<<topic;
}
void chat_msg_CHTOPIC::unpack(oscar_buffer&o) {
    o>>channel>>gender>>n_index>>nick>>topic;
}
void chat_msg_GREETING::pack(out_oscar_buffer&o) {
    o<<channel<<n_index<<nick<<gender;
}
void chat_msg_GREETING::unpack(oscar_buffer&o) {
    o>>channel>>n_index>>nick>>gender;
}
void chat_msg_KICK::pack(out_oscar_buffer&o) {
    o<<channel<<nick<<zamsg<<gender<<n_index;
}
void chat_msg_KICK::unpack(oscar_buffer&o) {
    o>>channel>>nick>>zamsg>>gender>>n_index;
}
void chat_msg_KILLED::pack(out_oscar_buffer&o) {
    o<<channel<<n_index<<gender<<nick;
}
void chat_msg_KILLED::unpack(oscar_buffer&o) {
    o>>channel>>n_index>>gender>>nick;
}
void chat_msg_MAKEIT::pack(out_oscar_buffer&o) {
    o<<channel<<from_id<<from_nick<<msg<<setting_n_color_pack;
}
void chat_msg_MAKEIT::unpack(oscar_buffer&o) {
    o>>channel>>from_id>>from_nick>>msg>>setting_n_color_pack;
}
void chat_msg_MAT::pack(out_oscar_buffer&o) {
    o<<channel<<gender<<n_index<<nick;
}
void chat_msg_MAT::unpack(oscar_buffer&o) {
    o>>channel>>gender>>n_index>>nick;
}
void chat_msg_PRIV::pack(out_oscar_buffer&o) {
    o<<channel<<from_id<<to_id<<from_nick<<to_nick<<msg<<setting_n_color_pack;
}
void chat_msg_PRIV::unpack(oscar_buffer&o) {
    o>>channel>>from_id>>to_id>>from_nick>>to_nick>>msg>>setting_n_color_pack;
}
void chat_msg_QUIT::pack(out_oscar_buffer&o) {
    o<<channel<<n_index<<nick<<gender;
}
void chat_msg_QUIT::unpack(oscar_buffer&o) {
    o>>channel>>n_index>>nick>>gender;
}
void chat_msg_STDTO::pack(out_oscar_buffer &o) {
    o<<from_id<<to_id<<channel<<from_nick<<msg<<setting_n_color_pack;
}
void chat_msg_STDTO::unpack(oscar_buffer &o) {
    o>>from_id>>to_id>>channel>>from_nick>>msg>>setting_n_color_pack;
}
void chat_msg_SYS::pack(out_oscar_buffer&o) {
    o<<channel<<from_id<<msg;
}
void chat_msg_SYS::unpack(oscar_buffer&o) {
    o>>channel>>from_id>>msg;
}
void chat_msg_SYSPRIV::pack(out_oscar_buffer&o) {
    o<<channel<<from_id<<to_id<<to_nick<<msg;
}
void chat_msg_SYSPRIV::unpack(oscar_buffer&o) {
    o>>channel>>from_id>>to_id>>to_nick>>msg;
}
void chat_msg_MODE::pack(out_oscar_buffer&o) {
    o<<channel<<from_id<<nick1<<nick2<<msg<<n_index<<gender;
}
void chat_msg_MODE::unpack(oscar_buffer&o) {
    o>>channel>>from_id>>nick1>>nick2>>msg>>n_index>>gender;
}




string chat_msg_MAT::make_backup(chat_thread_stuff& stuff)
{
    return ":MAT: n:+"+nick.name+"\n";
}
string chat_msg_STDTO::make_backup(chat_thread_stuff&)
{
    return ":STDTO: n:"+from_nick.name+" m:"+msg+"\n";
}
string chat_msg_PRIV::make_backup(chat_thread_stuff&)
{
    return ":PRIV: fn:"+from_nick.name+" tn:"+to_nick.name+" m:"+msg+"\n";
}
string chat_msg_MAKEIT::make_backup(chat_thread_stuff&)
{
    return ":MAKEIT: n:"+from_nick.name+" m:"+msg+"\n";
}
string chat_msg_MODE::make_backup(chat_thread_stuff&)
{
    return "";
}
string chat_msg_GREETING::make_backup(chat_thread_stuff&)
{
    return "";
}
string chat_msg_QUIT::make_backup(chat_thread_stuff&)
{
    return "";
}
string chat_msg_KILLED::make_backup(chat_thread_stuff&)
{
    return "";
}
string chat_msg_KICK::make_backup(chat_thread_stuff&)
{
    return "";
}
string chat_msg_CHTOPIC::make_backup(chat_thread_stuff&)
{
    return "";
}
string chat_msg_SYS::make_backup(chat_thread_stuff& stuff)
{

    map<DB_id,PTR_getter_nm<chat_stuff_user_p> >::const_iterator it=stuff.__users$.find(from_id);
    if (it!=stuff.__users$.end())
    {
        return ":SYS: n:"+it->second.___ptr->ud.cstate.nick.name+" m:"+msg+"\n";
    }
    return "";
}
string chat_msg_SYSPRIV::make_backup(chat_thread_stuff& stuff)
{

    map<DB_id,PTR_getter_nm<chat_stuff_user_p> >::const_iterator it=stuff.__users$.find(from_id);
    if (it!=stuff.__users$.end())
    {
        return ":SYS: fn:"+it->second.___ptr->ud.cstate.nick.name+" tn:"+to_nick+" m:"+msg+"\n";
    }
    return "";
}
string chat_msg_CHROOM::make_backup(chat_thread_stuff&)
{
    return "";
}
string chat_msg_CHNICK::make_backup(chat_thread_stuff&)
{
    return "";
}

chat_msg::~chat_msg()
{
    MUTEX_INSPECTOR;

    if (data)delete data;
    data=NULL;
}

chat_msg_STDTO::chat_msg_STDTO(const   CH_id& $channel,const DB_id &$from_id,const 	DB_id &$to_id,const	user_nick &$from_nick,const string& $msg,	unsigned int $setting_n_color_pack)
    : chat_msg$(STDTO),channel($channel),from_id($from_id),to_id($to_id),from_nick($from_nick),
      msg($msg),setting_n_color_pack($setting_n_color_pack) {}

chat_msg_PRIV::chat_msg_PRIV(const CH_id& $channel,const DB_id& $from_id,const DB_id& $to_id,const user_nick& $from_nick,const user_nick& $to_nick,const string& $msg,unsigned int $setting_n_color_pack)
    :chat_msg$(PRIV),channel($channel),from_id($from_id),to_id($to_id),from_nick($from_nick),to_nick($to_nick),msg($msg),setting_n_color_pack($setting_n_color_pack)	{}

chat_msg_MAKEIT::chat_msg_MAKEIT(const CH_id& $channel,const DB_id& $from_id,const user_nick& $from_nick,const string& $msg,unsigned int $setting_n_color_pack)
    :chat_msg$(MAKEIT),channel($channel),from_id($from_id),from_nick($from_nick),msg($msg),setting_n_color_pack($setting_n_color_pack) {}

chat_msg_MODE::chat_msg_MODE(const CH_id& $channel,const DB_id& $from_id,const user_nick& $nick1,const string& $nick2,const string& $msg,int $n_index,unsigned char $gender, const int& $n_color_pack)
    :chat_msg$(MODE),channel($channel),from_id($from_id),nick1($nick1),nick2($nick2),msg($msg),n_index($n_index),gender($gender),n_color_pack($n_color_pack) {}

chat_msg_SYS::chat_msg_SYS(const CH_id& $channel,const DB_id& $from_id,const string& $msg)
    :chat_msg$(SYS),channel($channel),from_id($from_id),msg($msg) {}

chat_msg_SYSPRIV::chat_msg_SYSPRIV(const CH_id& $channel,const DB_id& $from_id,const DB_id& $to_id,const string& $to_nick,const string& $msg)
    :chat_msg$(SYSPRIV),channel($channel),from_id($from_id),to_id($to_id),to_nick($to_nick),msg($msg)
{}

chat_msg_GREETING::chat_msg_GREETING(const CH_id& $channel,int $n_index,const user_nick& $nick,unsigned char $gender,const int&_nickstyle)
    :chat_msg$(JOIN),channel($channel),n_index($n_index),nick($nick),gender($gender),n_color_pack(_nickstyle) {}

chat_msg_KILLED::chat_msg_KILLED(const CH_id& $channel,int $n_index,	unsigned char $gender,const user_nick& $nick,const int&_nickstyle)
    :chat_msg$(KILLED),channel($channel),n_index($n_index),gender($gender),nick($nick),n_color_pack(_nickstyle) {}

chat_msg_QUIT::chat_msg_QUIT(const CH_id& $channel,int $n_index,const user_nick& $nick,unsigned char $gender,const int&_nickstyle)
    :chat_msg$(QUIT),channel($channel),n_index($n_index),nick($nick),gender($gender),n_color_pack(_nickstyle) {}

chat_msg_KICK::chat_msg_KICK(const CH_id& $channel,const user_nick& $nick,const string& $zamsg,unsigned char $gender,int $n_index,const int&_nickstyle)
    :chat_msg$(KICK),channel($channel),nick($nick),zamsg($zamsg),gender($gender),n_index($n_index),n_color_pack(_nickstyle) {}

chat_msg_CHROOM::chat_msg_CHROOM(const CH_id& $channel,unsigned char $gender,int $n_index,const user_nick& $nick, const string& $chroomname,const int&_nickstyle)
    :chat_msg$(CHROOM),
     channel($channel),gender($gender),n_index($n_index),nick($nick),chroomname($chroomname),n_color_pack(_nickstyle)
{}


chat_msg_CHNICK::chat_msg_CHNICK(const CH_id& $channel,unsigned char $gender,int $n_index,const string& $oldnick, const user_nick& $newnick,const int&_nickstyle)
    :chat_msg$(CHNICK),
     channel($channel),gender($gender),n_index($n_index),oldnick($oldnick),newnick($newnick),n_color_pack(_nickstyle)
{}

chat_msg_CHTOPIC::chat_msg_CHTOPIC(const CH_id& $channel,unsigned char $gender,int $n_index,const user_nick& $nick,const string& $topic,const int&_nickstyle)
    :chat_msg$(CHTOPIC),
     channel($channel),gender($gender),n_index($n_index),nick($nick),topic($topic),n_color_pack(_nickstyle)
{}

chat_msg_MAT::chat_msg_MAT(const CH_id& $channel,unsigned char $gender, int $n_index,const user_nick& $nick,const int&_nickstyle)
    : channel($channel),gender($gender),n_index($n_index),nick($nick),chat_msg$(MAT),n_color_pack(_nickstyle)
{
}
bool chat_msg$::is_ignored(const set<DB_id> &by_who)
{
    return false;
}
bool chat_msg_MAKEIT::is_ignored(const set<DB_id> &by_who)
{
    if (by_who.count(from_id)) return true;
    return false;
}
bool chat_msg_STDTO::is_ignored(const set<DB_id> &by_who)
{
    if (by_who.count(from_id)) return true;
    return false;
}
bool chat_msg_PRIV::is_ignored(const set<DB_id> &by_who)
{
    if (by_who.count(from_id)) return true;
    return false;
}
