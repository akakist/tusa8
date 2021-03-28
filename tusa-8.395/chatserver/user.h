#ifndef CHAT_USER_H__
#define CHAT_USER_H__
#include "_mutex.h"
#include "chat_def.h"
#include <time.h>
#include <list>
#include "levels.h"
#include <map>
#include "message.h"
#include <set>
#include "oscar_buffer.h"
#include "DB_id.h"
#include "CH_id.h"
#include "S_id.h"

#include "user_nick.h"
#include "user_status.h"
#include "user_state.h"
#include "nick_vec.h"
#include "user_profile.h"
#include "user_profile.h"
#include "c_conn.h"
class app_server;

class irc_connection;
struct u_notes;
struct u_fotos;

class user_nick;
struct user_status;
class user_state;


class user_profile;
class user_profile;
class user_stat;
class nick_vec;



class chat_user: public base_container
{

public:
    const char* getclassname()const {
        return "chat_user";
    }
    PTR_getter<c_socket>  persistent_socket;

    chat_user(const CT&, const DB_id&);



    PTR_getter<user_profile> __user_profile;
    virtual ~chat_user();

    unsigned int iframe_buffer_size;
    time_t iframe_last_send_time; /* last data_push_back time & send used
								  for send \r\n for keep concurrent connection*/
public:
    string iframe_push_buffer;
    bool www_offline_status_sent;
    time_t www_offline_time;
    bool www_inchat;
    string php_session_id;

    bool erased;
    time_t last_change_nick;

    time_t last_access_time;	// last access time (last http_request)
    time_t login_date;
    string irc_ip;

    time_t create_time;
    DB_id db_id;

    //////////////////////////////////////////////////////////////////////////
    int invisibility;
    c_level clevel;
    int max_msg_len;
    //////////////////////////////////////////////////////////////////////////
    CT _ct;


};

class user_profile;
class user_profile;
class user_stat;
class nick_vec;
struct user_pair
{
    PTR_getter<chat_user> cu;
    PTR_getter<user_profile> up;
    explicit user_pair(const PTR_getter<chat_user> &CU);
};
class chat_users_container
{

    _mutex Mutex;
    map<DB_id,map<CT,PTR_getter<chat_user> > > container;
public:


    map<DB_id,map<CT,PTR_getter<chat_user> > > get_container()
    {
        M_LOCK(Mutex);
        return container;
    }
    void insert$(const PTR_getter<chat_user> &p);
    PTR_getter<chat_user> get$(const DB_id& uid, const CT&);

    PTR_getter<chat_user> get$(const pair<DB_id,CT>& uid);

    vector<PTR_getter<chat_user> >get$(const DB_id& uid);

    vector<PTR_getter<chat_user> > values$();
    vector<pair<DB_id,CT> > keys$();
    size_t size$();

    int count$(const DB_id& zid, const CT&);
    int count$(const PTR_getter<chat_user> &);
    void erase$(const DB_id& uid, const CT&);
    void __clean$();
    chat_users_container():Mutex("chat_users_container") {}
};
extern chat_users_container local_users;



#ifdef LOCKCTL_DBUG

#define __UC_LOCK  PGLW(chat_user, u,__U.cu); int __U,__Z;__U=0,__Z=0;
#define __ZC_LOCK  PGLW(chat_user, z,__Z.cu); int __Z,__U;__Z=0,__U=0;
#define __UC_LOCKW PGLW(chat_user, u,__U.cu); int __Z,__U;__Z=0,__U=0;
#define __ZC_LOCKW PGLW(chat_user, z,__Z.cu); int __Z,__U;__Z=0,__U=0;
#define __S_LOCK PGLW(c_socket, s,__S);int __S;__S=0;int __U;__U=0;
#else
#define __UC_LOCK PGLW(chat_user, u,__U.cu);
#define __ZC_LOCK PGLW(chat_user, z,__Z.cu);
#define __UC_LOCKW PGLW(chat_user, u,__U.cu);
#define __ZC_LOCKW PGLW(chat_user, z,__Z.cu);

#define __S_LOCK PGLW(c_socket, s,__S);

#endif

bool is_inv(const DB_id&uid);


bool user$nick_entered(const user_pair& __U);
class chat_message;
struct user_pair;

int has_inv(const user_pair& __U);
int user$gender(const user_pair& __U);
time_t user$get_spent_time(const user_pair& __U);
set<string> get_user__privileges(const user_pair& __U);
bool user__privileges(const user_pair& __U, const string&);

string build_nicks_select(const DB_id& uid,const string &name,const string & defnick,const string &addp);
string build_nicks_select(const DB_id& uid,const string &name,int def,const string &addp);
void make_user_info(const DB_id& zid,const DB_id& uid, bool admin,const string &irc_host,const string &cnick, bool logined,string& out_irc);
string user$irc_ip(const user_pair& __U);
string user$login(const user_pair& __U);
string user$login(const DB_id& uid);
struct user_status;

DB_id user$id(const user_pair& __U);
DB_id user$id(const PTR_getter<chat_user>& __U);
unsigned int user$level(const user_pair& __U);
unsigned int user$setting_n_color_pack(const user_pair&__U);
unsigned int user$nicks_size(const DB_id& uid);

void init_internal_data(const user_pair& __U);

user_state get_user_state(const user_pair &__U);
user_status get_user_status_by_id(const user_pair&__U, int id);
user_status get_user_status_by_id(const DB_id&  uid , int status_id);

void __set_user_state(int cmd,const DB_id&uid, const out_oscar_buffer &s);

class chat_channel;


void data_push_back_www(const PTR_getter<chat_user>&,const string&);
void data_push_back_www(const user_pair&,const string&);
void data_push_back_irc(const PTR_getter<chat_user>&,const string&);
void data_push_back_irc(const user_pair&,const string&);

void kick_user(const DB_id& uid,const string &msgs);

void send_syspriv_msg(const PTR_getter<chat_user>&__U,const string &msg);
void send_syspriv_msg(const DB_id& uid,const string &msg);
void send_syspriv_msg(const user_pair&__U,const string &msg);

void user_destroy(const PTR_getter<chat_user> &__UU);
void user_register_all_nicks(const DB_id&);

bool can_add_nick(const user_pair &__U);
bool do_change_mode(PTR_getter<chat_channel> &ML,const PTR_getter<chat_user> &__UU,int mode);
bool do_change_voice(const CH_id& ch,const DB_id& uid,const DB_id& zid, int voice);
bool u_has_console(const user_pair& __U);
bool I_can_see(const PTR_getter<chat_user>__U,const PTR_getter<chat_user>__Z);

void us_PART_CHANNEL(const set<CH_id> &chs,const CT& _ct,const DB_id &uid);
void us_PART_CHANNEL(const CT& _ct,const DB_id &uid);

void us_SET_STATUS(const DB_id& uid,const user_status& st);
void us_SET_NICK(const DB_id &uid, const user_nick& nn);

DB_id  all_user_nick_2_DB_id(const string & nick);
void user_register_1_nick(const DB_id& id, const string& nick);
void user_unregister_1_nick(const DB_id& id, const string& nick);



#define MB (1024*1024)
#define KB (1024)

#endif
