#ifndef ooscar___cli__H
#define ooscar___cli__H
#include "threadable.h"
#include "c_conn.h"
struct oscar_client_data;
class oscar_client:public threadable
{
private:
    oscar_client_data* ocd;
    _mutex ocdmx;
    bool Parse(); // true if stop thread
    bool receiver(); // true if stop thread
public:
    oscar_client();
    virtual ~oscar_client();
    virtual void parse_ch1(oscar_buffer & b)=0;
    virtual void parse_ch2(oscar_buffer & b)=0;
    virtual void parse_ch3(oscar_buffer & b)=0;
    virtual void parse_ch4(oscar_buffer & b)=0;
    virtual void parse_ch5(oscar_buffer & b)=0;
    virtual void parse_ch6(oscar_buffer & b)=0;
    void client_connect( const char *host, unsigned short port);
    void reply(const string& buf);
    virtual void on_socket_create(SOCKET sock)=0;
    void do_parse_buffer();
    void handle_packets();
    virtual void cleanup();
private:
#ifndef WIN32
    static void *PTHR_handler(void *pr);
#endif
    static void *PTHR_receiver(void *pr);
public:
    void do_disconnect();
    //virtual time_t get_socket_inactive_kill_timeout()=0;


    void break_connect(const string &reason);
    virtual void on_disconnect(const string& reason)=0;
    virtual void start(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m);

private:
    void load_config(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m);
#ifndef WIN32
    unsigned int handler_timeout_usec;
#endif
    unsigned int receiver_timeout_usec;
    m_var<string> bind_ip;
    unsigned int handlers_count;

};

#endif
