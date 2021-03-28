
#ifndef TCP_DIALOG_SERVER__H
#define TCP_DIALOG_SERVER__H

#include <map>
#include <time.h>
#include "_mutex.h"
#include <vector>
#include "tcp_server.h"
#include "tcp_server_connection.h"
#include "PTR.h"
#include "oscar_buffer.h"
#include "M_SET.h"
#include "M_DEQUE.h"
class out_oscar_buffer;
class tcp_dialog_server;


class tcp_dialog_server: public tcp_server
{
public:


    /*second_data container
    */
    struct EVT
    {
        enum {
            E_ADD,
            E_REMOVE,
            E_TOPROCESS,
        };
        int type;
        EVT(int t):type(t) {}
    };
    M_DEQUE<PTR_getter<c_socket> > accepteds;
    M_DEQUE<PTR_getter<c_socket> > readable_socks;

    tcp_dialog_server();
    virtual void start(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m);

    ~tcp_dialog_server();
private:
    void load_config(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m);
    static void *handler(void*);
    static void *socket_selector(void*);
public:

    // process buffer by derived calls
    virtual void process_buffer(const PTR_getter<c_socket>& __SD)=0;

    // call inherited to do something like log output etc..
    virtual void on_delete(const PTR_getter<c_socket>& __SD,const std::string& msg)=0;

    // virtual destructor to delete second_data.extra_user_id (means user data)


    // virtual second_data validator.  Called in inherited classes
    // return 0 if user invalid
    // 1 - if valid
//	virtual bool validate_second_data(const PTR_getter<c_socket>&,std::string& errstr)=0;

    // call to do something when connections accepted.
    // f.e. FTPD case - print "220 FTP server is ready."
    virtual void on_accept_dialog_socket(const PTR_getter<c_socket>&s)=0;

    // thread timeouts
    // CONF
    unsigned int dialog_handler_timeout_usec;
    unsigned int selector_timeout_usec;
    unsigned int dialog_handler_cnt;
    // stuff to prevent hacker attack.
    // is is possible to establish many connection and take all system sockets.
    // to prevent we limited number of connections in time_period
    // ex: 20,5 means per 20 sec max accepted connections will be 5.
    //
#ifdef ANTIFLOOD_CHECK
    unsigned int antiflood_period;
    unsigned int antiflood_period_cnt;
#endif

    //ENDCONF

    m_var<time_t> last_antiflood_period_start;
    m_var<int>connects_in_period;
    virtual void delete_extra_user_id(const std::string&)=0;

private:

};
class tcp_dialog_connection:public tcp_server_connection
{
public:
    tcp_dialog_server *server;
    tcp_dialog_connection(const PTR_getter<c_socket> &sk, tcp_dialog_server* serv):
        tcp_server_connection(sk),server(serv) {}

    // virtual inherited from tcp_server_connection
    void process_tcp();

};
class tcp_linedialog_server: public tcp_dialog_server
{
public:
    tcp_linedialog_server();
    virtual void start(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m);
    void load_config(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m);

    // derived call to process buffer
    void process_buffer(const PTR_getter<c_socket>& __SD);

    // process command in inherited class
    virtual void process_command(const std::string& cmd,const PTR_getter<c_socket>& __SD)=0;
    static std::string extract_command(string& buffer);

};
void disconnect(const PTR_getter<c_socket>& __SD);
void reply_second_data(const PTR_getter<c_socket>& __SD, const std::string & _data);
void reply_second_data(const PTR_getter<c_socket>& __SD, const OCALC & _data);

#endif
