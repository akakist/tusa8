#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <sys/types.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include <vector>
#include <map>
#include <map>
#include "tcp_server_connection.h"
#include <stdio.h>
#include "_mutex.h"
#include "web_server.h"
#include "errs.h"
#include "str_lib.h"
#include "st_stuff.h"
#include <errno.h>

#include "file_set.h"
#include "des_sid.h"
#include "mutex_inspector.h"
#include <algorithm>
#include <set>
#include <deque>

#include "M_DEQUE.h"

#include "tcp_server_data.h"
#include "web_connection.h"
#include "web_server_data.h"

#include "tcp_stats.h"
#ifdef WIN32
#define snprintf _snprintf
#endif

/////////////QUEUES
struct conntime_item {
    string ip;
    time_t conn_time;
};
struct admval
{
    int container;
    admval() {
        container=0;
    }
};


M_DEQUE<string> hit_ip_queue;
M_DEQUE<string> hit_url_queue;
M_DEQUE<conntime_item> conntime_items;
map<string/*ip*/,string/*country*/> whois_infos;
_mutex whois_infosMX("whois_infosMX");
void local_free_handle_webadminfo()
{
    hit_ip_queue.clear();
    hit_url_queue.clear();
    conntime_items.clear();
    whois_infos.clear();
}


int srt_s_i(map<string, int >::const_iterator i1, map<string, int >::const_iterator i2)
{
    return i1->second > i2->second;
}
