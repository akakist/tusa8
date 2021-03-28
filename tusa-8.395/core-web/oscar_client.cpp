#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "errs.h"
#include "str_lib.h"
#ifndef WIN32
#include <sys/timeb.h>
#include <netdb.h>
#endif
#include "oscar_buffer.h"
#include "PTR.h"

#include "oscar_client.h"
#include "c_conn.h"
#ifndef WIN32
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <errno.h>




void dump_traf_incoming(const string& bb);
void dump_traf_outgoing(const string& bb);

struct oscar_client_data
{
    const char* getclassname()const {
        return "oscar_client_data";
    }
    oscar_client_data();
    string in_buffer;
    bool isConnected();
    PTR_getter<c_socket> sock$;
    void cleanup();
    deque<pair<string,unsigned char> > packets;


};
void oscar_client_data::cleanup()
{
    in_buffer.clear();
    sock$=NULL;
}

oscar_client_data::oscar_client_data():sock$(NULL)
{
}
#ifndef WIN32
void *oscar_client::PTHR_handler(void *pr)
{
    MUTEX_INSPECTOR;
    log_start_thread("oscar_client parser");
    oscar_client *bcid=(oscar_client*)pr;
    if(!bcid) {
        logErr2("srall %s %d",__FILE__,__LINE__);
        return NULL;
    }
    while(1)
    {
        if(g_must_terminate) return NULL;
        try {
            bcid->handle_packets();
            bcid->USLEEP(bcid->handler_timeout_usec);
        }
        STDCATCHS("oscar_client::parse_handler");
    }
    return NULL;
}
#endif
bool oscar_client::receiver()
{
    XTRY;
    MUTEX_INSPECTOR;
    char recv_buf[4096];
    if(!ocd)
    {
        MUTEX_INSPECTOR;
        return false;
    }
    //PTR_getter<c_socket> __SK();
    if(ocd->sock$)
    {
        MUTEX_INSPECTOR;
        size_t rret=0;
        {
            MUTEX_INSPECTOR;
            XTRY;
            rret=S_recv(ocd->sock$,recv_buf,sizeof(recv_buf),0);
            if(rret)
                TRACEDBG(logErr2("recvd %d bytes",rret));
            XPASS;
        }
        if(rret==-1)
        {
            XTRY;
            MUTEX_INSPECTOR;
            ocd->sock$=NULL;
            on_disconnect("recv");
            XPASS;
            return false;
        }

        if(rret>0)
        {
            XTRY;
            MUTEX_INSPECTOR;
            {
                M_LOCK(ocdmx);
                ocd->in_buffer+=string(recv_buf,rret);
            }
#ifdef DUMP_TRAF
            dump_traf_incoming(string(recv_buf,rret));
#endif

            do_parse_buffer();
            XPASS;
        }

    }
    {
        XTRY;
        MUTEX_INSPECTOR;
        if(USLEEP(receiver_timeout_usec))return true;
        XPASS;
    }
    XPASS;
    return false;

}
void *oscar_client::PTHR_receiver(void *pr)
{
    MUTEX_INSPECTOR;

    oscar_client *bcid=(oscar_client*)pr;
    if(!bcid) {
        logErr2("srall %s %d",__FILE__,__LINE__);
        return NULL;
    }
    while(1)
    {

        if(bcid->USLEEP(10000)) return NULL;
        try {
            XTRY;
            if(bcid->receiver())
            {
                logErr2("recv_handler exiting");
                return NULL;
            }
            XPASS;

        }
#ifndef WIN32
        STDCATCHS("oscar_client::PTHR_receiver");
#else
        catch(...) {}
#endif
    }


    return NULL;
}
void oscar_client::handle_packets()
{
    XTRY;
    MUTEX_INSPECTOR;
    while(1)
    {
        MUTEX_INSPECTOR;
        XTRY;
        pair<string,unsigned char> command;
        bool success=false;
        {

            M_LOCK(ocdmx);
            if(ocd->packets.size())
            {
                TRACEDBG(logErr2("command=*ocd->packets.begin();"));
                command=*ocd->packets.begin();
                ocd->packets.pop_front();
                success=true;
            }
            else return;
        }
        if(!success) return;
        //if(!command.first.size())return;

        {

            oscar_buffer in_b(command.first.data(),command.first.size());
            switch (command.second) {
            case 1:
                parse_ch1(in_b);
                break;
            case 2:
                parse_ch2(in_b);
                break;
            case 3:
                parse_ch3(in_b);
                break;
            case 4:
                parse_ch4(in_b);
                break;
            case 5:
                parse_ch5(in_b);
                break;
            case 6:
                parse_ch6(in_b);
                break;
            default:
                break;
            }
        }
        XPASS;


    }
    XPASS;
}
void oscar_client::do_parse_buffer()
{
//	logErr2("void oscar_client::do_parse_buffer()");
    try {
        while (1)
        {

            unsigned char start_byte, channel;
            string command;
            bool need_disconnect=false;
            {
                {
                    M_LOCK(ocdmx);
                    if(ocd->in_buffer.size()==0)return;
                    start_byte=ocd->in_buffer[0];
                }

                if(start_byte == OSCAR2_START_BYTE)
                {
                    M_LOCK(ocdmx);
                    if (ocd->in_buffer.size()<FLAP2_HEADER_SIZE)
                        return;
                    OSCAR2_DATA_LEN_T data_len;
                    memcpy(&data_len,ocd->in_buffer.data()+FLAP2_DATA_SIZE_OFFSET,sizeof(data_len));
                    memcpy(&channel,ocd->in_buffer.data()+FLAP2_CHANNEL_OFFSET,sizeof(channel));
                    int tl=FLAP2_HEADER_SIZE+data_len;
                    if(ocd->in_buffer.size()>=tl)
                    {
                        command=ocd->in_buffer.substr(FLAP2_HEADER_SIZE,data_len);
                        size_t  bsz=ocd->in_buffer.size();
                        ocd->in_buffer=ocd->in_buffer.substr(tl,bsz-tl);
                    }
                    else return;
                }
                else  {
                    need_disconnect=true;
                }
                if(need_disconnect)
                {
                    DBG(logErr2("Invalid Start Byte on FLAP %d  %s %d",start_byte,__FILE__,__LINE__));
                    M_LOCK(ocdmx);
                    ocd->in_buffer.clear();
                }
                if(start_byte==OSCAR2_START_BYTE)
                {
                    M_LOCK(ocdmx);
                    TRACEDBG(logErr2("ocd->packets"));
                    ocd->packets.push_back(make_pair(command,channel));

                }
            }
        }
    }
    STDCATCHS("Parse()");

}

void oscar_client::client_connect( const char *host, unsigned short port)
{   MUTEX_INSPECTOR;
    SOCKET __sock = ::socket(AF_INET, SOCK_STREAM, 0);
    TRACEDBG(logErr2("try connect %s %d",host,port));
    if (__sock <= 0)
        throw cError("Couldn't create socket"+_DMI());
    struct sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(port);
    remoteAddr.sin_addr.s_addr = gethostname(host);



    while(1)
    {
        int res;
        {
            res=::connect(__sock,(const struct sockaddr *)&remoteAddr, sizeof(struct sockaddr));
        }
        if(res==-1)
        {
            logErr2("cant connect %s %d",host,port);
            break_connect("connect_failed");


            {
                if(SLEEP(1)) return;
            }

            return;
        }
        else
        {
#ifdef DEBUG
            TRACEDBG(logErr2("connected %s %d",host,port));
#endif
            {
                c_socket *s=new c_socket(__sock,(string)"oscar_client socket "+getclassname()/*,get_socket_inactive_kill_timeout()*/);
                LEAKCTL_ADD(s);
                ocd->sock$=s;
            }
            break;
        }
        {
        }
    }

#ifdef WIN32
    u_long f = 1;
    int r=ioctlsocket(__sock, FIONBIO, &f);
#else
    int f = 1;
    int r=ioctl(__sock, FIONBIO, &f);
#endif
    if(r==-1)
    {
        logErr2("ioctl: %s",strerror(errno));
    }

    int val=0x8000;
    if(setsockopt(__sock,SOL_SOCKET,SO_RCVBUF,(char*)&val,sizeof(val)))
    {
        DBG(logErr2("setsockopt failed %s %d (%s)",__FILE__,__LINE__,strerror(errno)));
    }
    val=0x8000;
    if(setsockopt(__sock,SOL_SOCKET,SO_SNDBUF,(char*)&val,sizeof(val)))
    {
        DBG(logErr2("setsockopt failed %s %d (%s)",__FILE__,__LINE__,strerror(errno)));
    }
    on_socket_create(__sock);

}

void oscar_client::break_connect(const string& reason)
{
    MUTEX_INSPECTOR;
    PTR_getter<c_socket> __S(ocd->sock$);
    ocd->sock$=NULL;
    if(__S) {
        disconnect(__S);
    }
    cleanup();
    on_disconnect(reason);


}
oscar_client::oscar_client():ocd(new oscar_client_data) {
    LEAKCTL_ADD(ocd);
}
oscar_client::~oscar_client() {
    LEAKCTL_REMOVE(ocd);
    delete ocd;
}


void oscar_client::reply(const string& buf)
{
    MUTEX_INSPECTOR;
    reply_socket(ocd->sock$,buf);
}
void oscar_client::start(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m)
{
    load_config(pn,prefix,m);

#ifndef WIN32
    for(unsigned i=0; i<handlers_count; i++)
        create_thread(PTHR_handler," handler");
#endif

    create_thread(PTHR_receiver," receiver");

}
void oscar_client::load_config(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m)
{
    set_cfg_i(m,	prefix+"handlers_count", pn,handlers_count,2);
#ifndef WIN32
    set_cfg_i(m,	prefix+"handler_timeout_usec", pn,handler_timeout_usec, 10000);
#endif
    set_cfg_i(m,	prefix+"receiver_timeout_usec", pn,receiver_timeout_usec, 20);
    set_cfg_str(m,	prefix+"bind_ip", pn,bind_ip, "INADDR_ANY");

}
void oscar_client::do_disconnect()
{
    PTR_getter<c_socket> __S(ocd->sock$);
    ocd->sock$=NULL;
    if(__S) {
        disconnect(__S);
    }

}
void oscar_client::cleanup()
{
    M_LOCK(ocdmx);
    LEAKCTL_REMOVE(ocd);
    delete ocd;
    ocd=new oscar_client_data;
    LEAKCTL_ADD(ocd);
    ocd->cleanup();
}
