#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#endif
#ifndef WIN32
#include <sys/time.h>
#endif
#include <errno.h>
#include "tcp_server_connection.h"
#include "errs.h"
#ifndef WIN32
#include <unistd.h>
#else
#include <time.h>
#endif
#include <string.h>
#include "str_lib.h"
#include "mutex_inspector.h"
using namespace std;
#include "tcp_sender.h"


#ifdef WIN32
#define snprintf _snprintf
#endif
void _stream_conn_cpp() {}
tcp_server_connection::tcp_server_connection(const PTR_getter<c_socket> &s):socket(s)
{

    max_recv = 0;
    handle_max_recv_limit = false;
}
tcp_server_connection::~tcp_server_connection()
{
}

void tcp_server_connection::write(const std::string & buf)
{   MUTEX_INSPECTOR;

#ifdef WITH_SENDERS
    write_sender_data(socket,buf);
#else
    reply_socket(socket,buf);
#endif
}
tcp_server_connection::tcp_server_connection(const tcp_server_connection* s):
    socket(s->socket),
    tcp_peer_ipaddress(s->tcp_peer_ipaddress),
    peer_address(s->peer_address),
    max_recv(s->max_recv),
    handle_max_recv_limit(s->handle_max_recv_limit)

{

}
