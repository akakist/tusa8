#ifndef GLOBALSTATE___H
#define GLOBALSTATE___H

class __global_state
{
    _mutex Mutex;

    map<DB_id,user_state> user_states;
    map<DB_id, map<CT,set<CH_id > >  > tmpc__;
public:
    string dump();
    void set_data(const string & packet);
    string get_data();
    void set_user_state(const DB_id& uid, const user_state& st);
    user_state get_user_state(const DB_id& uid);
    map<DB_id,user_state> get_user_states();
    void pack(out_oscar_buffer &b);
    void unpack(oscar_buffer &b);
    set<CH_id> get_user_channel(const CT &_ct, const DB_id&);
    void set_nick(const DB_id& ,const user_nick &);
    size_t channel_size(const DB_id& );
    user_status get_status(const DB_id& );
    void remove(const DB_id& );
    set<CH_id> part_channels(const set<CH_id>&ch,const CT& _ct, const DB_id&uid);
    void set_status(const DB_id& uid, const user_status& st);
    void set_ip(const DB_id& uid,const string& ip);
    string get_ip(const DB_id& uid);
    int get_users$count();
    size_t size();
    map<DB_id,user_state> get_container();
    bool inchat$www(const DB_id &uid);
    bool inchat$irc(const DB_id &uid);
    void erase(const DB_id &uid)
    {
        M_LOCK(Mutex);
        user_states.erase(uid);
        tmpc__.erase(uid);
    }

};
extern __global_state global_state;
#endif
