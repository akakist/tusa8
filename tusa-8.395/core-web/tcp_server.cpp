#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include "tcp_server_data.h"
#include "tcp_server.h"
#include "mutex_inspector.h"
#include <errno.h>
#include <sys/stat.h>
#ifndef WIN32
#include <arpa/inet.h>
#endif
#ifdef WIN32
#include <process.h>
#include <time.h>
#endif
#ifdef WIN32
#define snprintf _snprintf
#endif
#include "PTR.h"
#include "str_lib.h"
#include "tcp_stats.h"
#ifndef WIN32
#include <sys/time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif
#include <sys/types.h>
using namespace std;
void on_socket_create(SOCKET_id sock);
void on_socket_close(SOCKET_id sock);
tcp_server_data::tcp_server_data(
)
{
    handlers_count=0;
    handlers_busy=0;
}
tcp_server_data::~tcp_server_data()
{
}

void tcp_server::start(const string& pn,const string& prefix,map<string,string>&m)
{
    MUTEX_INSPECTOR;

    load_config(pn,prefix,m);

    deque<pair<string,unsigned short> > addrs=tcpdata->listen_addresses.get_container();
    for(unsigned i=0; i<addrs.size(); i++)
    {

        SOCKET sock=::socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(sock==-1) {
            throw cError("Server: socket() failed %s",strerror(errno));
        }
        struct sockaddr_in sa;
        sa.sin_family = AF_INET;
        sa.sin_port = htons(addrs[i].second);
        {
            if(addrs[i].first=="INADDR_ANY") sa.sin_addr.s_addr = INADDR_ANY;
            else sa.sin_addr.s_addr = inet_addr(addrs[i].first.c_str());
        }
        {
            int i = 1;
            i=setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(char *)&i,sizeof(i));
        }
        logErr2("binding to %s:%d",addrs[i].first.c_str(),addrs[i].second);
        while(::bind (sock,(struct sockaddr *) &sa, sizeof (sa)) == -1)
        {


            char s[100];
            {
                snprintf(s,sizeof(s)-1,"Server: bind() failed: %s",strerror(errno));
            }
            logErr2("%s %s:%d", s,inet_ntoa(sa.sin_addr),htons(sa.sin_port));

                if(SLEEP(1))return;
//            }

        }
        logErr2("%s binded to %s:%d", server_name().c_str(),inet_ntoa(sa.sin_addr),htons(sa.sin_port));
        if(listen(sock,128)==-1) {
            throw cError("Server: listen() failed: %s",strerror(errno));
            exit(1);

        }
        tcpdata->asocket.push_back(sock);
    }
    for(unsigned i=0; i<addrs.size(); i++)
    {
        create_thread(listener,server_name()+" tcp listener");
    }


}
tcp_server::tcp_server():tcpdata(new tcp_server_data()) {
    LEAKCTL_ADD(tcpdata);
}
tcp_server::~tcp_server()
{

}


#ifdef WIN32
typedef int socklen_t;
#endif




class st_THREAD_BUSY
{
    /*stack based class to calculated currently runned handlers

    constructor: increment serv->tcpdata->handlers_busy
    destructor: decrement serv->tcpdata->handlers_busy
    */
    tcp_server *serv;
    pthread_t self;
public:
    st_THREAD_BUSY(tcp_server *s,pthread_t sf)
    {
        serv=s;
        self=sf;
        ++serv->tcpdata->handlers_busy;
    }
    ~st_THREAD_BUSY()
    {
        --serv->tcpdata->handlers_busy;
    }
};


void *tcp_server::handler(void *pr)
{

    tcp_server *srv=(tcp_server*)pr;
    string thread_name=((tcp_server *) pr)->server_name()+" tcp_server::handler";
    log_start_thread(thread_name);
    pthread_t self = pthread_self();
    while (1) {
        try {

            PTR_getter <c_socket> sok(NULL);
            /*what while not accepted connection available
            */
            if(g_must_terminate)
            {
                return NULL;
            }
#ifdef WIN32
            while (!srv->tcpdata->conn_list.extract_first(sok)) {
                if(g_must_terminate)
                {
                    return NULL;
                }
                if(srv->USLEEP(srv->tcpdata->accept_handler_timeout_usec))
                    return NULL;
            }
#else
            while (!srv->tcpdata->conn_list.try_extract_first(sok)) {
                if(g_must_terminate)
                {
                    return NULL;
                }
                if(srv->USLEEP(srv->tcpdata->accept_handler_timeout_usec))return NULL;
            }
#endif




            if (sok) {

                sockaddr_in ip;
                string ips;
                {
                    PGLW(c_socket, s,sok);
                    ip=s->peer_address;
                    ips=s->peer_ipaddress;
                }
                {
                    if(!srv->validate_peer_ip_address(ip))
                    {
                        continue;
                    }

                }
                tcp_server_connection * connection=(tcp_server_connection *)srv->new_connection(sok);
                connection->peer_address=ip;
                connection->tcp_peer_ipaddress=ips;

                {
                    /*mark thread as busy
                    */
                    st_THREAD_BUSY kall_mazafaka_AAAAAAA(srv, self);
                    try {
                        /*call virtual inherited function
                        */
                        connection->process_tcp();
                    }
                    STDCATCHS("tcpserver handler STDCATCHS");
                }	// if(connection->connected())
                LEAKCTL_REMOVE(connection);
                delete connection;
            }	// if(connection)
        }
        STDCATCHS("tcp server handler");
    }			// while(1)

}




void *tcp_server::listener(void *pr)
{

    tcp_server *srv=(tcp_server*)pr;

    string thread_name=srv->server_name()+" tcp_server::listener";
    log_start_thread(thread_name);
    int last_accept_err=0;
    SOCKET accsock;
    if(!srv->tcpdata->asocket.extract_first(accsock))
    {
        logErr2("cannot extract sock in tcp listener");
        return NULL;
    }
    while (1) {
        if(g_must_terminate)
        {
            return NULL;
        }
        try {
            sockaddr_in sa;
            int len = sizeof(sa);
            PTR_getter<c_socket> __nsock(NULL);
            SOCKET ns = accept(accsock,(sockaddr *) &sa, (socklen_t *) &len);

            if(ns==-1)
            {
                if(last_accept_err==0 && last_accept_err%1000==0)
                {
                    logErr2("accept socket: %s (%d times)",strerror(errno),last_accept_err);
                }
                last_accept_err++;

                continue;
            }
            last_accept_err=0;

#ifdef WIN32
            u_long f = 1;
            int r=ioctlsocket(ns, FIONBIO, &f);
#else
            int f = 1;
            int r=ioctl(ns, FIONBIO, &f);
#endif
            if(r==-1)
            {
                logErr2("ioctl: %s",strerror(errno));
            }
#ifdef TCPBUFFF
            int val=0x8000;
            if(setsockopt(ns,SOL_SOCKET,SO_RCVBUF,(char*)&val,sizeof(val)))
            {
                DBG(logErr2("setsockopt failed %s %d (%s)",__FILE__,__LINE__,strerror(errno)));
            }
            val=0x8000;
            if(setsockopt(ns,SOL_SOCKET,SO_SNDBUF,(char*)&val,sizeof(val)))
            {
                DBG(logErr2("setsockopt failed %s %d (%s)",__FILE__,__LINE__,strerror(errno)));
            }
#endif
            SOCKET_id nsi;
            CONTAINER(nsi)=ns;
            on_socket_create(nsi);
            if(ns!=-1)
            {
                c_socket *s=new c_socket(ns, string(srv->server_name())+" sock"/*,srv->get_socket_inactive_kill_timeout()*/);
                LEAKCTL_ADD(s);
                memcpy(&s->peer_address,&sa,sizeof(s->peer_address));
                s->peer_ipaddress = sockaddr_to_string(s->peer_address.sin_addr.s_addr);
                __nsock=s;
                ++tcp_total_accepted_connections;
            }

            if (__nsock) {
                {
                    /* add accepted connection into conn_list */
                    srv->tcpdata->conn_list.push_back(__nsock);
                }
                /* if free handler available do nothing
                esle create additional handler */
                bool found_free = (srv->tcpdata->handlers_count.get()-srv->tcpdata->handlers_busy.get()>0);

                if (!found_free ) {
                    if (srv->tcpdata->max_number_of_accept_handlers == 0 || srv->tcpdata->handlers_count.get() < (int)srv->tcpdata->max_number_of_accept_handlers) {
                        srv->create_thread(handler,srv->server_name()+" tcp handler");
                        ++srv->tcpdata->handlers_count;
                    }
                }
            }
            else
            {

            }
        }
        STDCATCHS("tcp_server__listener STDCATCHS");
    }
    return NULL;
}

m_var<bool> TCP_enabled(true);

void tcp_server::load_config(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m)
{   MUTEX_INSPECTOR;
    m_var<string> listen_addresses;
    set_cfg_str(m,	prefix+"listen_addresses", pn,listen_addresses, "INADDR_ANY:80;INADDR_ANY:81");
    vector<string> v=splitTOK(";, ",listen_addresses);
    for(unsigned i=0; i<v.size(); i++)
    {
        vector<string>vv=splitTOK(":",v[i]);
        if(vv.size()==2)
        {
            tcpdata->listen_addresses.push_back(make_pair(vv[0],atoi(vv[1])));
        }
    }
    set_cfg_i(m,	prefix+"accept_handler_timeout_usec", pn,tcpdata->accept_handler_timeout_usec,20000);
    set_cfg_i(m,	prefix+"max_number_of_accept_handlers", pn,tcpdata->max_number_of_accept_handlers,0);

}
