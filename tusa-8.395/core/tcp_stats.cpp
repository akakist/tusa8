#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <sys/stat.h>
#include "mutex_inspector.h"
#include <set>
#include "str_lib.h"
#include "m_var.h"
#include <ctime>
#include <stdio.h>

using namespace std;
struct tm LOCALTIME(time_t timep);

#ifndef WIN32
m_var<long long> tcp_total_send(0);
m_var<long long> tcp_total_recv(0);
m_var<long long> tcp_total_accepted_connections(0);
#else
m_var<__int64> tcp_total_send(0);
m_var<__int64> tcp_total_recv(0);
m_var<__int64> tcp_total_accepted_connections(0);
#endif

#ifdef WIN32
__int64 tcp_total_send_get() {
    return tcp_total_send;
}
__int64 tcp_total_recv_get() {
    return tcp_total_recv;
}
__int64 tcp_total_accepted_connections_get() {
    return tcp_total_accepted_connections;
}
#endif

void dump_traf_incoming(const string& bb)
{
    tm tt=LOCALTIME(time(NULL));
    char ss[200];
    snprintf(ss,sizeof(ss)-1,"traf.in.%04d%02d%02d-%02d",1900+tt.tm_year,tt.tm_mon+1,tt.tm_mday,tt.tm_hour);
    FILE *f=fopen(ss,"ab");
    if(f)
    {
        fwrite(bb.data(),bb.size(),1,f);
        fclose(f);
    }
}
void dump_traf_outgoing(const string& bb)
{

    tm tt=LOCALTIME(time(NULL));
    char ss[200];
    snprintf(ss,sizeof(ss)-1,"traf.out.%04d%02d%02d-%02d",1900+tt.tm_year,tt.tm_mon+1,tt.tm_mday,tt.tm_hour);
    FILE *f=fopen(ss,"ab");
    if(f)
    {
        fwrite(bb.data(),bb.size(),1,f);
        fclose(f);
    }
}
