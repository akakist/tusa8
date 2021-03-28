#ifndef chat_event___h
#define chat_event___h
#include "oscar_buffer.h"
#include "S_id.h"
#include "DB_id.h"
#include "CH_id.h"
#include "PTR.h"
#include "M_DEQUE.h"
#include "chat_stuff.h"
#include "user.h"

class chat_event: public base_container_nm
{
public:
    const char* getclassname()const =0;
    enum TYPE {
        INVITE				=1,
        CHMODE				=2,
        CHVOICE				=3,
        SEND_MESSAGE			=5,
        SET_user_state			=6,
        SEND_DATA_TO_WWW_USERS		=7,
        CHTOPIC				=8,
        UPDATE_DATA			=9,
        CHAT_MESSAGES2			=10,
        irc_cmd_ISON			=15,
        irc_cmd_WHOIS			=16,
        irc_cmd_JOIN			=19,
        UPDATE_CONTACT_LIST		=20,
        AINF				=23,
        CHROOM				=24,
        FLUSH_USER_DATA		=25,


    };
    TYPE type;
    virtual ~chat_event() {}
    chat_event(TYPE t):type(t) {}
    virtual void pack(out_oscar_buffer &b)
    {
        throw cError("unimplemented chat_event::pack for type "+itostring(type)+" "+_DMI());
    }


};
class chat_event_FLUSH_USER_DATA: public  chat_event
{
public:
    const char* getclassname()const {
        return "chat_event_FLUSH_USER_DATA";
    }
    DB_id uid;
    chat_event_FLUSH_USER_DATA(const DB_id& _uid):chat_event(FLUSH_USER_DATA),uid(_uid) {}
    chat_event_FLUSH_USER_DATA(oscar_buffer& in_b):chat_event(FLUSH_USER_DATA) {
        unpack(in_b);
    }
    void pack(out_oscar_buffer &b);
    void unpack(oscar_buffer &b);
};

class chat_event_CHROOM: public  chat_event
{
public:
    const char* getclassname()const {
        return "chat_event_CHROOM";
    }
    DB_id uid;
    CH_id oldchan;
    CH_id newchan;
    chat_event_CHROOM(const DB_id& _uid, const CH_id& _oldchan, const CH_id& _newchan):chat_event(CHROOM),uid(_uid),oldchan(_oldchan),newchan(_newchan) {}
    chat_event_CHROOM(oscar_buffer& in_b):chat_event(CHROOM) {
        unpack(in_b);
    }
    void pack(out_oscar_buffer &b);
    void unpack(oscar_buffer &b);
};

class chat_event_INVITE: public  chat_event
{
public:
    const char* getclassname()const {
        return "chat_event_INVITE";
    }
    DB_id uid;
    DB_id zid;
    string msg;
    chat_event_INVITE(const DB_id& _uid, const DB_id& _zid, const string& _msg):chat_event(INVITE),uid(_uid),zid(_zid),msg(_msg) {}
    chat_event_INVITE(oscar_buffer& in_b):chat_event(INVITE) {
        unpack(in_b);
    }
    void pack(out_oscar_buffer &b);
    void unpack(oscar_buffer &b);
};

class chat_event_CHVOICE: public  chat_event
{   // global

public:
    const char* getclassname()const {
        return "chat_event_CHVOICE";
    }

    DB_id zid;
    CH_id channel;
    DB_id uid;
    int voice;// // -1 to invert current voice
    chat_event_CHVOICE(const DB_id& _zid, const CH_id& ch, const DB_id& _uid, int _voice):chat_event(CHVOICE),zid(_zid),channel(ch),uid(_uid),voice(_voice) {}
    chat_event_CHVOICE(oscar_buffer& in_b):chat_event(CHVOICE) {
        unpack(in_b);
    }
    void pack(out_oscar_buffer &b);
    void unpack(oscar_buffer &b);

};
class chat_event_CHMODE: public  chat_event
{
public:
    const char* getclassname()const {
        return "chat_event_CHMODE";
    }
    user_nick nick;
    CH_id channel;
    DB_id uid;
    int mode; // -1 to invert current mode
    chat_event_CHMODE(const user_nick& _nick, const CH_id& ch, const DB_id& _uid, int _mode):chat_event(CHMODE),nick(_nick),channel(ch),uid(_uid),mode(_mode) {}
    chat_event_CHMODE(oscar_buffer& in_b):chat_event(CHMODE) {
        unpack(in_b);
    }
    void pack(out_oscar_buffer &b);
    void unpack(oscar_buffer &b);
};

class chat_event_SEND_MESSAGE: public  chat_event
{
public:
    const char* getclassname()const {
        return "chat_event_SEND_MESSAGE";
    }
    chat_event_SEND_MESSAGE
    (const CH_id&c, const DB_id& f, const string& _tonick, const string&m, bool pr,bool _sys, bool mk):chat_event(SEND_MESSAGE),
        channel(c),	from_uid(f),tonick(_tonick),msg(m),priv(pr),sys(_sys),makeit(mk)
    {}
    chat_event_SEND_MESSAGE(oscar_buffer&b):chat_event(SEND_MESSAGE) {
        unpack(b);
    }
    CH_id channel;
    DB_id from_uid;
    string tonick;
    string msg;
    bool priv;
    bool sys;
    bool makeit;
    void pack(out_oscar_buffer &b);
    void unpack(oscar_buffer &b);
};
class user_state;
class chat_event_SET_user_state: public  chat_event
{
public:
    const char* getclassname()const {
        return "chat_event_SET_user_state";
    }
    chat_event_SET_user_state():chat_event(SET_user_state) {
        CONTAINER(uid)=0;
    }
    chat_event_SET_user_state(oscar_buffer&b):chat_event(SET_user_state) {
        unpack(b);
    }
    int cmd;
    DB_id uid;
    string msg;
    CT ct;
    void pack(out_oscar_buffer &b);
    void unpack(oscar_buffer &b);
};


class chat_event_CHTOPIC: public  chat_event
{
public:
    const char* getclassname()const {
        return "chat_event_CHTOPIC";
    }
    DB_id uid;
    CH_id channel;
    string top;
    chat_event_CHTOPIC(const CH_id& c, const DB_id& u,const string &t):chat_event(CHTOPIC),channel(c),uid(u),top(t) {}
    chat_event_CHTOPIC(oscar_buffer& in_b):chat_event(CHTOPIC) {
        unpack(in_b);
    }
    void pack(out_oscar_buffer &b);
    void unpack(oscar_buffer &b);
};

class chat_event_ainf: public  chat_event
{
public:
    const char* getclassname()const {
        return "chat_event_ainf";
    }
    PTR_getter<c_socket> sock;
    chat_event_ainf(const PTR_getter<c_socket> &u):chat_event(AINF),sock(u) {}
};

class chat_event_irc_cmd_ISON: public  chat_event
{
public:
    const char* getclassname()const {
        return "chat_event_irc_cmd_ISON";
    }
    vector<string> nicks;
    PTR_getter<chat_user> __U;
    chat_event_irc_cmd_ISON():chat_event(irc_cmd_ISON),__U(NULL) {}
};
class chat_event_irc_cmd_WHOIS: public  chat_event
{
public:

    const char* getclassname()const {
        return "chat_event_irc_cmd_WHOIS";
    }
    string nick;
    PTR_getter<chat_user> __U;
    chat_event_irc_cmd_WHOIS(const PTR_getter<chat_user> &u,const string &n):chat_event(irc_cmd_WHOIS),__U(u),nick(n) {}
};
class chat_event_common_JOIN: public  chat_event
{
public:
    const char* getclassname()const {
        return "chat_event_common_JOIN";
    }
    CH_id channel;
    PTR_getter<chat_user> __U;


    user_status status;
    user_nick nick;
    CT _ct;
    chat_event_common_JOIN(const PTR_getter<chat_user>  &U,const CH_id&c, const user_status& st,const user_nick&n,const CT& _$ct):chat_event(irc_cmd_JOIN),
        __U(U),channel(c),status(st),nick(n),_ct(_$ct)
    {}
};




class chat_event_UPDATE_CONTACT_LIST: public  chat_event
{
public:
    const char* getclassname()const {
        return "chat_event_UPDATE_CONTACT_LIST";
    }
    PTR_getter<chat_user> __U;
    CT _ct;
    chat_event_UPDATE_CONTACT_LIST(const PTR_getter<chat_user> &u, const CT& c ):chat_event(UPDATE_CONTACT_LIST),__U(u),_ct(c) {}
};

class chat_event_CHAT_MESSAGES2: public  chat_event
{
public:
    const char* getclassname()const {
        return "chat_event_CHAT_MESSAGES2";
    }
    chat_event_CHAT_MESSAGES2(const CH_id&ch, const PTR_getter<chat_user> &u):__U(u),c(ch),chat_event(CHAT_MESSAGES2) {}
    CH_id c;
    PTR_getter<chat_user> __U;
};

class chat_event_SEND_DATA_TO_WWW_USERS: public  chat_event
{
public:
    const char* getclassname()const {
        return "chat_event_SEND_DATA_TO_WWW_USERS";
    }
    chat_event_SEND_DATA_TO_WWW_USERS():chat_event(SEND_DATA_TO_WWW_USERS) {}
    string msg;
};
class chat_event_UPDATE_DATA: public  chat_event
{
public:
    const char* getclassname()const {
        return "chat_event_UPDATE_DATA";
    }
    string buf;
    chat_event_UPDATE_DATA(const string& b,int n):chat_event(UPDATE_DATA),buf(b) {}
};


extern M_DEQUE<PTR_getter_nm<chat_event> > __chat_events;
void send_chat_event_push_back(chat_event*);
void send_chat_event_push_front(chat_event*);

#endif
