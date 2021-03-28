#ifndef CHAT_STUFFFF___HH
#define CHAT_STUFFFF___HH


#include "chat_channel_data.h"
#include "PTR.h"
#include "chat_msg.h"
#include "threadable.h"
#include <set>
#include <map>
#define ACCEPT_ALL_PUB 1
#define ACCEPT_ALL_PRIV 2
#define ACCEPT_PRIV_CONTACT 4
#define ACCEPT_ALL_CONTACT 8

class c_level;

struct chat_stuff_user
{


    DB_id dbid;
    set<DB_id> ignores;
    set<DB_id> contacts;
    unsigned char gender;
    int _invisibility;
    int _bought_invisibility;
    string nickclass;
    string nickclass_birthdate;

    unsigned int level;
    c_level *c$level;

    user_state cstate;

    bool setting_show_eye;
    int setting_neye;
    unsigned int n_color_pack;
    string login;

    set<string> __privileges;
    unsigned int contact_options;


    bool ul_mode_contacts_only;
    bool ul_mode_hide_male;
    bool ul_mode_hide_female;
    bool unreged;
    time_t birth_date;

    map<unsigned int, string> str_nicks;

    //tmp
    time_t	tmp$last_m_time;
    string	tmp$last_message;// not need to init

    time_t	tmp$last_invite_time;
    int		tmp$msg_cnt_per_time;



    bool I_can_see_contact_list(const chat_stuff_user &zz) const;
    bool I_can_see_other(const chat_stuff_user &zz) const ;
    chat_stuff_user():tmp$last_m_time(0),tmp$last_invite_time(0),tmp$msg_cnt_per_time(0),c$level(NULL)
    {
        CONTAINER(dbid)=0;
    }
};
class chat_user;

class chat_event_CHAT_MESSAGES2;
class chat_event_CHMODE;
class chat_event_CHTOPIC;
class chat_event_CHVOICE;
class chat_event_INVITE;
class chat_event_irc_cmd_ISON;
class chat_event_irc_cmd_WHOIS;
class chat_event_common_JOIN;
class chat_event_SEND_DATA_TO_WWW_USERS;
class chat_event_SEND_MESSAGE;
class chat_event_SET_user_state;
class chat_event_UPDATE_DATA;
class chat_event_UPDATE_CONTACT_LIST;
class chat_event_ainf;

class chat_user;
out_oscar_buffer & operator<<(out_oscar_buffer&b, const chat_stuff_user &n);
oscar_buffer & operator>>(oscar_buffer&b, chat_stuff_user &n);

struct chat_stuff_user_p:public base_container_nm
{
    const char* getclassname()const {
        return "chat_stuff_user_p";
    }

    chat_stuff_user ud;
};
class chat_channel_data;

struct chat_channel_data_p:public base_container_nm
{
    chat_channel_data cd;
};

struct client_instance //local users only
{
    DB_id dbid;
    struct vis_item
    {
        int status;
        string nick;
        PTR_getter_nm<chat_stuff_user_p> __pu;
        vis_item(int st, const string&n, const PTR_getter_nm<chat_stuff_user_p> & p):
            status(st),nick(n),__pu(p) {}
    };
    map<CH_id,map<DB_id,vis_item > > visible_users;
    PTR_getter<chat_user> __U;
    PTR_getter_nm<chat_stuff_user_p> usr;
    CT user_type;
    client_instance (const PTR_getter<chat_user> &u,const PTR_getter_nm<chat_stuff_user_p> &_usr, const CT & utype):__U(u),usr(_usr),user_type(utype)
    {
        CONTAINER(dbid)=0;
    }

};

struct client_instance_p:public base_container_nm
{
    const char* getclassname()const {
        return "client_instance_p";
    }

    client_instance ci;
    client_instance_p(const PTR_getter<chat_user> &u,const PTR_getter_nm<chat_stuff_user_p> &_usr, const CT &ut): ci(u,_usr,ut) {}
};
class chat_processor:public threadable
{
public:
    const char* getclassname()const {
        return "chat_processor";
    }

    static void *PTHR_handler(void *pr);
    void handler();
    void start(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m);
    void load_config(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m);

};

extern M_DEQUE<PTR_getter_nm<chat_msg> > external_$messages;
struct chat_thread_stuff
{
    chat_processor* processor;
    chat_thread_stuff(chat_processor*p):processor(p) {}

    deque<PTR_getter_nm<chat_msg> > _$messages;

    deque<PTR_getter_nm<chat_msg> > backup_messages;

    map<CH_id,chat_channel_data> __channels$;
    vector<CH_id> __channels_keys$;
    map<unsigned int,c_level*> level$;


    map<CH_id,set<DB_id> >	__users_in_channels$;
    string __users_in_channels$_dump();
    map<CH_id,map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> >  >	__local_users_in_channels$;
    string __local_users_in_channels$_dump();

    map<DB_id,PTR_getter_nm<chat_stuff_user_p > > __users$;
    map<string, PTR_getter_nm<chat_stuff_user_p > > strnick2__u;

    string to_all;
    map<DB_id,time_t> join_time;
    map<DB_id,time_t> trash;
    map<DB_id,int>  user_message_count;

    map<pair<DB_id,CT>,PTR_getter_nm<client_instance_p> > local_clients;
    time_t last_clean_local_clients;

    // tmp vars
    deque<pair<CH_id,PTR_getter_nm<chat_stuff_user_p > > > greeting_msgs_channels;
    deque<pair<CH_id,PTR_getter_nm<chat_stuff_user_p > > > bye_msgs_channels;
    deque<pair<CH_id/*channel to send msg*/, pair<PTR_getter_nm<chat_stuff_user_p > ,string /*name of new channel*/> > > chroom_msgs_channels;
    deque<pair<CH_id,pair<PTR_getter_nm<chat_stuff_user_p > ,string> > > kick_msgs_channels;
    deque<pair<CH_id,pair<PTR_getter_nm<chat_stuff_user_p > ,string/*oldnick*/> > > chnick_msgs_channels;
    bool need_send_rc;
    time_t last_send_rc;
    time_t last_sync_time;
    // end tmp vars

    void make_url_users_full_to_file();

    void get_irc_list();
    time_t last_make_url_users_content;
    bool need_make_url_users_content;

    string LK_get_users$count_for_web();


    PTR_getter_nm<chat_stuff_user_p>  get_user_p(const DB_id& uid);
    PTR_getter_nm<chat_stuff_user_p>  find_nick(const string& strn);

    void preload_user(const DB_id& uid);
    bool can_preload_user(const DB_id& uid);
    void save_chat_msg(const PTR_getter_nm<chat_msg>& e);
    void unload_user(const DB_id& uid);
    void on_INVITE(chat_event_INVITE *e);
    void on_UPDATE_CONTACT_LIST(chat_event_UPDATE_CONTACT_LIST *e);
    void on_CHAT_MESSAGES2(chat_event_CHAT_MESSAGES2*e);
    void on_SEND_MESSAGE(chat_event_SEND_MESSAGE*e);
    void on_SET_user_state(chat_event_SET_user_state*e);
    void on_SEND_DATA_TO_WWW_USERS(chat_event_SEND_DATA_TO_WWW_USERS*e);
    void on_CHMODE(chat_event_CHMODE *e);
    void on_CHVOICE(chat_event_CHVOICE *e);
    void on_CHTOPIC(chat_event_CHTOPIC *e);
    void on_UPDATE_DATA(chat_event_UPDATE_DATA *e);
    void on_irc_cmd_ISON(chat_event_irc_cmd_ISON *e);
    void on_irc_cmd_WHOIS(chat_event_irc_cmd_WHOIS *e);
    void on_common_JOIN(chat_event_common_JOIN *e);

    void on_ainf(chat_event_ainf *e);

    void handle_stat_t_count(const PTR_getter_nm<chat_stuff_user_p> &__pu);
    void on_JOIN_fix_lists(const chat_channel_data &__c,const PTR_getter_nm<chat_stuff_user_p> &__pu, const CH_id& channel,const user_status &status, const user_nick& nick, const CT& _ct);



    void do_set_user_state(int cmd,const PTR_getter_nm<chat_stuff_user_p > &__pu,oscar_buffer& o);
    bool user_process_send_message_to_channel(const DB_id& uid, const CH_id& chan,const string &__ms,const string& tonick, bool makeit,bool usersys);
    bool user_process_send_message_priv(const DB_id& uid, const DB_id&  zid, string ms, string tonick, bool usersys);
    void push_back_channel_msg(const PTR_getter_nm<chat_msg> &e,const CH_id& ch,const DB_id& from_id,const DB_id& to_id,const string& ircd,const string& ircdto);
    string LK_chat_channel_get_n_messages(const CH_id& ch_id, const chat_stuff_user& __u,unsigned int n);
    string LK_get_java_users(const CH_id& channel, chat_stuff_user &__u,client_instance& __u_instance);

    void send_chnick_msgs(chat_stuff_user &__u,const string &from_nick,const string& new_nick, set<CH_id> &chans);


    void update_my_contact_list_fix_www_list(const PTR_getter_nm<client_instance_p> &__pci , const CH_id &ch, const CT &_ct);
    void update_my_contact_list_nofix_www_list(const PTR_getter_nm<client_instance_p> &__pci , const CH_id &ch, const CT &_ct);


    void handle_room_event_messages();



    const chat_channel_data & channel_cref(const CH_id &ch) const;
    chat_channel_data & channel_ref(const CH_id &ch);


    void normalize_user_channels(const CT& ct,const PTR_getter_nm<chat_stuff_user_p>&__pu);

    void normalize_user_channels_clean(const DB_id& uid);
    void update_contact_list_all(const set<CH_id> &fluent_channels,/*int oldst,int New_st,*/const PTR_getter_nm<chat_stuff_user_p>&__pu/*, const string &fromnick, const string& tonick, bool kicked, const string& zamsg*/);
    map<CH_id,map<DB_id,PTR_getter_nm<chat_stuff_user_p> > > update_contact_list_data;
    time_t update_contact_list_data_create_time;
    void __update_contact_list_all();

    void update_contact_list_all_kicked(const set<CH_id> &fluent_channels,const PTR_getter_nm<chat_stuff_user_p>&__pu, const string& _zamsg);

    bool check_klaped(chat_channel_data& __c,const DB_id & uid, string& timecnt,string&za,time_t &end);// true if banned

};



out_oscar_buffer & operator<<(out_oscar_buffer&b, const CT &n);
oscar_buffer & operator>>(oscar_buffer&b, CT &n);


#endif
