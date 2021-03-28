#ifndef IRC_SESSUION__H
#define IRC_SESSUION__H
S_id next_session_id();

class irc_session:public base_container
{
public:
    const char* getclassname()const {
        return "irc_session";
    }

    PTR_getter<chat_user> __U;
    PTR_getter<c_socket> __S;

    time_t create_time;
    time_t last_pong;
    time_t last_ping;
    string ip;
    time_t last_join;
    struct str_n {
        string str;
        bool entered;
        str_n():entered(false) {}
    } nick,pass,user;
    S_id session;

    irc_session():base_container("irc_session"),
        __U(NULL),
        __S(NULL),
        create_time(time(NULL)),
        last_pong(time(NULL)), last_ping(time(NULL)),
        last_join(0),
        session(next_session_id())
    {}
};
class __irc_sessions
{
    _mutex Mutex;
    map<S_id,PTR_getter<irc_session> > container;
public:
    void add(const S_id &sid,const PTR_getter<irc_session> &__IS);
    void iterate();
    void erase(const S_id &sid);
    size_t  size() {
        M_LOCK(Mutex);
        return container.size();
    }
    PTR_getter<irc_session> get(const S_id &s);
    PTR_getter<irc_session> get_by_dbid(const DB_id& s);
    __irc_sessions():Mutex("") {}
};
extern __irc_sessions irc_sessions;
void data_push_back(const PTR_getter<irc_session> &__IS,const string& s);
void disconnect(const PTR_getter<irc_session> &__IS);
void irc_send_reply(const PTR_getter<irc_session>&__IS,int code,const string &text);
void irc_send_notice(const PTR_getter<irc_session>&__U,const string &msg,const string &from);


#endif
