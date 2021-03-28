#ifndef ADMIN_H__
#define ADMIN_H__

// 0 - success, 1 - already banned



#ifdef DEBUG
struct B_id
{
    unsigned int container;
    B_id():container(0) {}
};

inline int operator < (const B_id& a,const B_id& b) {
    return a.container<b.container;
}
inline bool operator == (const B_id& a,const B_id& b) {
    return a.container==b.container;
}
inline bool operator != (const B_id& a,const B_id& b) {
    return a.container!=b.container;
}
inline out_oscar_buffer & operator<< (out_oscar_buffer& b,const B_id& s) {
    b<<s.container;
    return b;
}
inline oscar_buffer & operator>> (oscar_buffer& b,  B_id& s) {
    b>>s.container;
    return b;
}

/*namespace std {
_STLP_TEMPLATE_NULL struct hash< B_id> {
    size_t operator()( B_id n) const {
        return CONTAINER(n);
    }
};*/
//}

#else
typedef unsigned int B_id;
#endif

B_id get_next_klap_id();
string get_name_of_ban_time(time_t _tt);

class klap_item
{
public:

private:
public:
    void setcurtime(size_t t)
    {
        time_ban_start=time(NULL);
        time_ban_end=t+time_ban_start;
        time_cnt_str=get_name_of_ban_time(t);
    }
    B_id id;
    DB_id uid;
    string descr;
    time_t time_ban_start;
    time_t time_ban_end;
    string time_cnt_str;
    CH_id channel;
    DB_id adm_uid;
    string adm_name;
    string user_name;
    int alevel;
    int ulevel;
    klap_item()
    {
        CONTAINER(channel)=0;
        CONTAINER(id)=0;
        CONTAINER(uid)=0;
        time_ban_start=0;
        time_ban_end=0;
        CONTAINER(adm_uid)=0;
        alevel=0;
        ulevel=0;
    }
    void pack(out_oscar_buffer& o);
    void unpack(oscar_buffer&);
};
out_oscar_buffer & operator<< (out_oscar_buffer& b,const klap_item& s);
oscar_buffer & operator>> (oscar_buffer& b,  klap_item& s);
class klap_stuff
{
    _mutex Mutex;
    map<CH_id,map<DB_id,B_id> > by_uid;
    map<B_id,klap_item> items;
    void rebuild_NM();
public:
    bool check_ban_by_uid(const CH_id& ch,const DB_id& uid, klap_item& result); // true if banned
    string add__klap(const klap_item& item);
    string /*success result*/ remove_klap(const B_id& bid);
    string /*success result*/ remove_klap_NOLOCK(const B_id& bid);
    map<B_id ,klap_item> get_items();
    bool get_item(const B_id& bid, klap_item & ret);
    void clear();
    B_id get_next_klap_id();

};
extern klap_stuff _klap_stuff;

string /*success result*/ remove_klap(const DB_id& adm_remover,  const B_id& bid);
string add_klap$$(const klap_item& item);

int get_klap_ulevel(const DB_id&uid);
string get_klap_uname(const DB_id&uid);


#endif
