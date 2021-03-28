#ifndef tcp_server_priv_d__h
#define tcp_server_priv_d__h
#include <sys/types.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include <map>
#include "M_DEQUE.h"
#include "tcp_server_connection.h"
#include <set>
#include "M_MAP.h"

#include <string>
#include <vector>
#include <sys/types.h>
#include <map>
#ifndef WIN32
#include  <pthread.h>
#endif
using namespace std;
class tcp_server_data {
public:
    const char* getclassname()const {
        return "tcp_server_data";
    }
    M_DEQUE<SOCKET> asocket;
    // list of accepted connection
    M_DEQUE<PTR_getter<c_socket> > conn_list;

    tcp_server_data();
    ~tcp_server_data();

    //CONFIG
    // total handlers count
    m_var<unsigned int> handlers_count;
    // number of busy handlers. If 0 we'll create new handler thread
    m_var<unsigned int> handlers_busy;
    M_DEQUE<pair<std::string,unsigned short> > listen_addresses;
    unsigned int accept_handler_timeout_usec;
    unsigned int max_number_of_accept_handlers;
    //END_CONF
};



#endif
