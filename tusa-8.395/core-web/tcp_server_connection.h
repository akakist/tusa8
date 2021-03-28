#ifndef STREAM_CONN_H11__
#define STREAM_CONN_H11__
#include "c_conn.h"
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include <string>
#include "PTR.h"
class tcp_server_connection
{
protected:

public:
    const char* getclassname()const {
        return "tcp_server_connection";
    }

    PTR_getter<c_socket> socket;

    tcp_server_connection(const PTR_getter<c_socket> &s);
    virtual ~tcp_server_connection();
    virtual void process_tcp()=0;

    void write(const std::string & buf);
    std::string tcp_peer_ipaddress;
    sockaddr_in peer_address;
    size_t max_recv;
    bool handle_max_recv_limit;
    tcp_server_connection(const tcp_server_connection* s);
};
#endif
