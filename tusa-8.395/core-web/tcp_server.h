
#ifndef tcp_server_H__
#define tcp_server_H__
#include <sys/types.h>
#include <string>
#include "m_var.h"
#include "threadable.h"
class tcp_server_data;
class c_socket;
template <class T>
class PTR_getter;
#include "c_conn.h"
class tcp_server: public threadable
{
private:
    void load_config(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m);
protected:

public:

    // virtual constructor for n ew connection
    // must overrided in application class
    virtual void *new_connection(const PTR_getter<c_socket>&s)=0;
    virtual bool validate_peer_ip_address(const sockaddr_in & sa)=0;
    tcp_server_data *tcpdata;
    virtual std::string server_name()=0;
    static void *listener(void *);
    static void *handler(void *);
    tcp_server();
    virtual void start(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m);
    virtual ~tcp_server();


};

#endif
