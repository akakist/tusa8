#ifndef CHAT_MSG______H
#define CHAT_MSG______H

#include "user_nick.h"
struct chat_thread_stuff;

struct chat_msg$ {
    enum M_TYPE {
        STDTO	=1,
        PRIV	=2,
        MAKEIT	=3,
        SYS	=4,
        SYSPRIV	=5,
        INVITE	=6,
        JOIN	=7,
        CHTOPIC	=8,
        KILLED	=9,
        KICK	=10,
        CHNICK	=11,
        MODE	=12,
        CHROOM	=13,
        MAT	=14,
        QUIT	=15,
    };
    static m_var<unsigned int> msg_counter;
    M_TYPE msgtype;
    string msgid;
    virtual string make(const CH_id&, const DB_id& id)=0;
    virtual string make_backup(chat_thread_stuff&)=0;
    virtual void pack(out_oscar_buffer &o)=0;
    virtual void unpack(oscar_buffer &o)=0;
    virtual bool is_ignored(const set<DB_id> &by_who);

    chat_msg$(M_TYPE t);
    string send_message(const string&s);

    virtual ~chat_msg$() {}
};
struct chat_msg: public base_container_nm
{

    chat_msg$ * data;
    chat_msg(chat_msg$* m):data(m) {};
    ~chat_msg();
    const char* getclassname()const {
        return "chat_msg";
    }
private:
    chat_msg(const chat_msg&);             // Not defined to prevent usage
    chat_msg& operator=(const chat_msg&);  // Not defined to prevent usage

};

struct chat_msg_STDTO: public chat_msg$
{
    CH_id channel;
    DB_id from_id;
    DB_id to_id;
    user_nick from_nick;
    string msg;
    unsigned int setting_n_color_pack;
    string make(const CH_id&,const DB_id& id);
    virtual void pack(out_oscar_buffer &o);
    virtual void unpack(oscar_buffer &o);
    virtual string make_backup(chat_thread_stuff&);
    ~chat_msg_STDTO();
    chat_msg_STDTO(const   CH_id& $channel,const DB_id &$from_id,const 	DB_id &$to_id,const	user_nick &$from_nick,const string& $msg,	unsigned int $setting_n_color_pack);
    bool is_ignored(const set<DB_id> &by_who);


private:
    string prepf,prept,prepa;
};
struct chat_msg_PRIV: public chat_msg$
{
    CH_id channel;
    DB_id from_id;
    DB_id to_id;
    user_nick from_nick;
    user_nick to_nick;
    string msg;
    unsigned int setting_n_color_pack;
    string make(const CH_id&,const DB_id& id);
    virtual string make_backup(chat_thread_stuff&);
    virtual void pack(out_oscar_buffer &o);
    virtual void unpack(oscar_buffer &o);
    ~chat_msg_PRIV();
    chat_msg_PRIV(const CH_id& $channel,const DB_id& $from_id,const DB_id& $to_id,const user_nick& $from_nick,const user_nick& $to_nick,const string& $msg,unsigned int $setting_n_color_pack);
    bool is_ignored(const set<DB_id> &by_who);

private:
    string prepf,prept;
};
struct chat_msg_MAKEIT: public chat_msg$
{
    CH_id channel;
    DB_id from_id;
    user_nick from_nick;
    string msg;
    unsigned int setting_n_color_pack;
    string make(const CH_id&,const DB_id& id);
    virtual string make_backup(chat_thread_stuff&);
    virtual void pack(out_oscar_buffer &o);
    virtual void unpack(oscar_buffer &o);
    bool is_ignored(const set<DB_id> &by_who);
    ~chat_msg_MAKEIT();
    chat_msg_MAKEIT(const CH_id& $channel,const DB_id& $from_id,const user_nick& $from_nick,const string& $msg,unsigned int $setting_n_color_pack);
private:
    string preparation;
};
struct chat_msg_MODE: public chat_msg$
{
    CH_id channel;
    DB_id from_id;
    user_nick nick1;
    string nick2,msg;
    int n_index;
    unsigned char gender;
    int n_color_pack;
    string make(const CH_id&,const DB_id& id);
    virtual string make_backup(chat_thread_stuff&);
    virtual void pack(out_oscar_buffer &o);
    virtual void unpack(oscar_buffer &o);
    ~chat_msg_MODE();
    chat_msg_MODE(const CH_id& $channel,const DB_id& $from_id,const user_nick& $nick1,const string& $nick2,const string& $msg,int $n_index,unsigned char $gender, const int& $n_color_pack);
private:
    string preparation;
};
struct chat_msg_SYS: public chat_msg$
{
    CH_id channel;
    DB_id from_id;
    string msg;
    string make(const CH_id&,const DB_id& id);
    virtual string make_backup(chat_thread_stuff&);
    virtual void pack(out_oscar_buffer &o);
    virtual void unpack(oscar_buffer &o);
    ~chat_msg_SYS();
    chat_msg_SYS(const CH_id& $channel,const DB_id& $from_id,const string& $msg);
private:
    string preparation;
};
struct chat_msg_SYSPRIV: public chat_msg$
{
    CH_id channel;
    DB_id from_id;
    DB_id to_id;
    string to_nick;
    string msg;
    string make(const CH_id&,const DB_id& id);
    virtual string make_backup(chat_thread_stuff&);
    virtual void pack(out_oscar_buffer &o);
    virtual void unpack(oscar_buffer &o);

    ~chat_msg_SYSPRIV();
    chat_msg_SYSPRIV(const CH_id& $channel,const DB_id& $from_id,const DB_id& $to_id,const string& $to_nick,const string& $msg);
private:
    map<DB_id,string> preparations;
};
struct chat_msg_GREETING: public chat_msg$
{
    CH_id channel;
    int n_index;
    user_nick nick;
    unsigned char gender;
    int n_color_pack;
    virtual string make_backup(chat_thread_stuff&);
    string make(const CH_id& channel,const DB_id& );
    virtual void pack(out_oscar_buffer &o);
    virtual void unpack(oscar_buffer &o);
    ~chat_msg_GREETING();
    chat_msg_GREETING(const CH_id& $channel,int $n_index,const user_nick& $nick,unsigned char $gender,const int &n_color_pack);
private:
    string preparation;

};
struct chat_msg_KILLED: public chat_msg$
{
    CH_id channel;
    int n_index;
    unsigned char gender;
    user_nick nick;
    int n_color_pack;

    virtual string make_backup(chat_thread_stuff&);
    string make(const CH_id& channel,const DB_id& __id);
    virtual void pack(out_oscar_buffer &o);
    virtual void unpack(oscar_buffer &o);
    ~chat_msg_KILLED();
    chat_msg_KILLED(const CH_id& $channel,int $n_index,	unsigned char $gender,const user_nick& $nick,const int &n_color_pack);
private:
    string preparation;
};
struct chat_msg_QUIT: public chat_msg$
{
    CH_id channel;
    int n_index;
    user_nick nick;
    int n_color_pack;
    unsigned char gender;
    string make(const CH_id& chan,const DB_id& __id);
    virtual string make_backup(chat_thread_stuff&);
    virtual void pack(out_oscar_buffer &o);
    virtual void unpack(oscar_buffer &o);
    ~chat_msg_QUIT();
    chat_msg_QUIT(const CH_id& $channel,int $n_index,const user_nick& $nick,unsigned char $gender,const int& n_color_pack);
private:
    string preparation;

};
struct chat_msg_KICK: public chat_msg$
{
    CH_id channel;
    user_nick nick;
    int n_color_pack;
    string zamsg;
    unsigned char gender;
    int n_index;
    string make(const CH_id& chan,const DB_id& __id);
    virtual string make_backup(chat_thread_stuff&);
    virtual void pack(out_oscar_buffer &o);
    virtual void unpack(oscar_buffer &o);
    ~chat_msg_KICK();
    chat_msg_KICK(const CH_id& $channel,const user_nick& $nick,const string& $zamsg,unsigned char $gender,int $n_index, const int &n_color_pack);
private:
    string preparation;
};
struct chat_msg_CHROOM: public chat_msg$
{
    CH_id channel;
    unsigned char gender;
    int n_index;
    user_nick nick;
    int n_color_pack;
    string chroomname;
    string make(const CH_id& chan,const DB_id& __id);
    virtual string make_backup(chat_thread_stuff&);
    virtual void pack(out_oscar_buffer &o);
    virtual void unpack(oscar_buffer &o);
    ~chat_msg_CHROOM();
    chat_msg_CHROOM(const CH_id& $channel,unsigned char $gender,int $n_index,const user_nick& $nick, const string& $chroomname,const int &n_color_pack);
private:
    string preparation;

};
struct chat_msg_CHNICK: public chat_msg$
{
    CH_id channel;
    unsigned char gender;
    int n_index;
    string oldnick;
    user_nick newnick;
    int n_color_pack;
    string make(const CH_id& chan,const DB_id& __id);
    virtual string make_backup(chat_thread_stuff&);
    virtual void pack(out_oscar_buffer &o);
    virtual void unpack(oscar_buffer &o);
    ~chat_msg_CHNICK();
    chat_msg_CHNICK(const CH_id& $channel,unsigned char $gender,int $n_index,const string& $oldnick, const user_nick& $newnick,const int &n_color_pack);
private:
    string preparation;
};
struct chat_msg_CHTOPIC: public chat_msg$
{
    CH_id channel;
    unsigned char gender;
    int n_index;
    user_nick nick;
    int n_color_pack;
    string topic;

    virtual string make_backup(chat_thread_stuff&);
    string make(const CH_id& chan,const DB_id& __id);
    virtual void pack(out_oscar_buffer &o);
    virtual void unpack(oscar_buffer &o);
    ~chat_msg_CHTOPIC();
    chat_msg_CHTOPIC(const CH_id& $channel,unsigned char $gender,int $n_index,const user_nick& $nick,const string& $topic, const int& n_color_pack);
private:
    string preparation;
};


struct chat_msg_MAT: public chat_msg$
{
    CH_id channel;
    unsigned char gender;
    int n_index;
    user_nick nick;
    int n_color_pack;
    virtual string make_backup(chat_thread_stuff&);
    string make(const CH_id& chan,const DB_id& __id);
    virtual void pack(out_oscar_buffer &o);
    virtual void unpack(oscar_buffer &o);
    ~chat_msg_MAT();
    chat_msg_MAT(const CH_id& $channel,unsigned char $gender, int $n_index,const user_nick& $nick,const int& n_color_pack);
private:
    string preparation;
};



#endif
