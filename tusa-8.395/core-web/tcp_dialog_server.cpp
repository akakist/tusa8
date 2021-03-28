#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include "_mutex.h"
#include <list>
#include "mutex_inspector.h"
#include <stdio.h>
#include <map>
#include <set>
#include "lockctl.h"
#include "c_conn.h"
#include "tcp_dialog_server.h"
#ifndef WIN32
#include <sys/poll.h>
#endif
#include "tcp_server_data.h"
#ifdef WIN32
#endif
#include "str_lib.h"
#include <errno.h>
using namespace std;
#ifdef THREAD_NAME_DBUG
#endif


void tcp_dialog_server::start(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m)
{
    MUTEX_INSPECTOR;
    tcp_server::start(pn,prefix,m);
    load_config(pn,prefix,m);


    for (unsigned int i=0; i<dialog_handler_cnt; i++)
    {

        create_thread(handler,server_name()+" tcp_dialog_server::handler");
    }

    {

        create_thread(socket_selector, server_name()+" tcp_dialog_server::socket_selector");
    }

}
tcp_dialog_server::tcp_dialog_server()
{   MUTEX_INSPECTOR;

    /*  constructor
    	parameters - config m_vars refs
    */
}
tcp_dialog_server::~tcp_dialog_server()
{


}





void *tcp_dialog_server::handler(void* pr)
{
    tcp_dialog_server *srv=(tcp_dialog_server*)pr;
    string thread_name=srv->server_name()+" tcp_dialog_server::handler";
    log_start_thread(thread_name);

    while (1) {
        if (g_must_terminate)return NULL;
        //    char buf[1024 + 100];
        {
            if (srv->USLEEP(srv->dialog_handler_timeout_usec))return NULL;
        }
        try {


            while (1) {
                if (g_must_terminate)return NULL;
                /* construct empty getter
                */

//                PTR_getter<c_socket> __SD(NULL);
                //              PTR_getter<c_socket> __S(NULL);
                PTR_getter<c_socket>  P(NULL);
                if (srv->readable_socks.extract_first(P))
                {
                    //logErr2("srv->process_buffer");
                    srv->process_buffer(P);

                }
                {
                    if (srv->USLEEP(srv->dialog_handler_timeout_usec))return NULL;
                }


            }//while



        }
        STDCATCHS("tcp_dialog handler");
    }
}

static _mutex CMX;
static map<string,int> sock_counters;
void sk_set(const string& nm,int cnt)
{
    M_LOCK(CMX);
    sock_counters[nm]=cnt;
}
string dump_tcp_dial()
{
    string out;
    {
        M_LOCK(CMX);
        for(map<string,int> ::iterator i=sock_counters.begin(); i!=sock_counters.end(); i++)
        {
            out+="TDS: "+i->first+" = "+std::to_string(i->second)+"<br>\n";
        }
    }
    return out;
}

void* tcp_dialog_server::socket_selector(void*pr)
{   MUTEX_INSPECTOR;
    tcp_dialog_server *srv=(tcp_dialog_server*)pr;
    string thread_name=srv->server_name()+" tcp_dialog_server::socket_selector";
    log_start_thread(thread_name);

    map<SOCKET_id,PTR_getter<c_socket> > ccc;
    time_t last_full_select=time(NULL);
    while (1) {
        MUTEX_INSPECTOR;
        if (g_must_terminate) return NULL;
        try {//MUTEX_INSPECTOR;
            while (1)
            {
                {
                    if (srv->USLEEP(srv->selector_timeout_usec))return NULL;
                }
                if (g_must_terminate) return NULL;
                {
                    deque<PTR_getter<c_socket> >dq=srv->accepteds.extract_all();
                    for (unsigned i=0; i<dq.size(); i++)
                    {   MUTEX_INSPECTOR;
                        SOCKET_id fd;
                        CONTAINER(fd)=-1;
                        {
                            PGLR(c_socket,s,dq[i]);
                            fd=s->get_socket_fd();
                        }
                        ccc.insert(make_pair(fd,dq[i]));
                    }
#ifdef _WIN32
                    pollfd pollfds[1024];
#else
                    pollfd pollfds[ccc.size()+1];
#endif
                    int idx=0;
                    for (map<SOCKET_id,PTR_getter<c_socket> > ::iterator i=ccc.begin(); i!=ccc.end(); i++)
                    {   MUTEX_INSPECTOR;
                        pollfds[idx].fd=CONTAINER(i->first);
                        pollfds[idx].events=POLLIN;
                        pollfds[idx].revents=0;
                        idx++;

                    }
                    time_t cur_t=time(NULL);
                    if(rand()%200==0)
                    {
                        vector<SOCKET_id> rm;
                        for (map<SOCKET_id,PTR_getter<c_socket>  > ::iterator i=ccc.begin(); i!=ccc.end(); i++)
                        {
                            bool _rm=false;
                            {   MUTEX_INSPECTOR;
                                PGLR(c_socket,s,i->second);
                                if(s->get_socket_fd()!=i->first)
                                {
                                    rm.push_back(i->first);
                                    _rm=true;
                                }


                            }
                            if(_rm)
                            {
                                srv->on_delete(i->second,"connection lost " __FILE__ " "+std::to_string(__LINE__));
                            }
                        }
                        for(unsigned i=0; i<rm.size(); i++)
                        {
                            ccc.erase(rm[i]);
                        }
                    }
                    if (idx)
                    {   MUTEX_INSPECTOR;

                        int ret;
                        {
                            ret=poll(pollfds,idx,5);
                        }
                        if (ret==-1)
                        {
                            logErr2("poll returned -1 (%s) (%s %d)",strerror(errno),__FILE__,__LINE__);
                        }
                        else if (ret>0)
                        {   MUTEX_INSPECTOR;
                            {
                                char buf[1024*32];
                                for (int ii=0; ii<idx; ii++)
                                {
                                    SOCKET_id fd;
                                    CONTAINER(fd)=pollfds[ii].fd;

                                    if (pollfds[ii].revents & (POLLIN|POLLPRI))
                                    {   MUTEX_INSPECTOR;
                                        SOCKET_id fd;
                                        CONTAINER(fd)=pollfds[ii].fd;
                                        map<SOCKET_id,PTR_getter<c_socket> > ::iterator it=ccc.find(fd);
                                        if (it==ccc.end())
                                        {   MUTEX_INSPECTOR;
//											logErr2("(it==ccc.end()");
                                            continue;
                                        }
                                        {   MUTEX_INSPECTOR;
                                            PTR_getter<c_socket> P=it->second;
                                            int r=-1;
                                            int __errno=0;
                                            {   MUTEX_INSPECTOR;
                                                PGLW(c_socket,s,P);
                                                if (fd!=s->get_socket_fd())
                                                {
                                                    logErr2("(fd!=s->get_socket_fd()");
                                                    ccc.erase(it->first);
                                                    continue;
                                                }
                                                r=::recv(CONTAINER(fd),buf,sizeof(buf),0);
                                                __errno=errno;
                                            }
                                            if (r==-1)
                                            {   MUTEX_INSPECTOR;
                                                if (__errno==EAGAIN) continue;
                                                else
                                                {
                                                    MUTEX_INSPECTOR;
                                                    srv->on_delete(P,"connection lost " __FILE__ " "+std::to_string(__LINE__));
                                                    disconnect(P);
                                                    ccc.erase(fd);
                                                    continue;
                                                }
                                            }
                                            if (r>0)
                                            {   MUTEX_INSPECTOR;

                                                {
                                                    PGLW(c_socket,s,P);
                                                    s->rd_buf+=string(buf,r);
                                                    s->recv0=0;
                                                }
                                                srv->readable_socks.push_back(P);
                                            }
                                            bool need_disconnect=false;
                                            if (r==0)
                                            {   MUTEX_INSPECTOR;
                                                PGLW(c_socket,s,P);
                                                s->recv0++;
                                                if (s->recv0>10)
                                                {
                                                    need_disconnect=true;
                                                    //logErr2("need_disconnect true if s->recv0>10");
                                                    //sleep(3);
                                                }
                                            }
                                            if (need_disconnect)
                                            {   MUTEX_INSPECTOR;
                                                srv->on_delete(P,"connection lost " __FILE__ " "+std::to_string(__LINE__));
                                                disconnect(P);
                                                ccc.erase(fd);
                                            }
                                        }

                                    }
                                    if (pollfds[ii].revents & (POLLERR|POLLHUP|POLLNVAL))
                                    {   MUTEX_INSPECTOR;
                                        SOCKET_id fd;
                                        CONTAINER(fd)=pollfds[ii].fd;
                                        map<SOCKET_id,PTR_getter<c_socket> > ::iterator it=ccc.find(fd);

                                        if (it!=ccc.end())
                                        {
                                            MUTEX_INSPECTOR;
                                            srv->on_delete(it->second,"connection lost " __FILE__ " "+std::to_string(__LINE__));
                                            disconnect(it->second);
                                            ccc.erase(fd);
                                            continue;
                                        }
                                    }

                                }
                            }
                        }
                    }



                }

            }
        }
        STDCATCHS("tcpdial_socket_selector");
    }
    return NULL;
}













void tcp_dialog_connection::process_tcp()
{   MUTEX_INSPECTOR;
    /* 	inherit call from tcp_server
    	called on accept connection to read and process it
    */

    // antiflood check
#ifdef ANTIFLOOD_CHECK
    if (time(NULL)-server->last_antiflood_period_start>server->antiflood_period) {
        server->last_antiflood_period_start=time(NULL);
        server->connects_in_period=0;
    }
    ++server->connects_in_period;
    if (server->connects_in_period.get()>server->antiflood_period_cnt.get())
        return;

#endif

    server->on_accept_dialog_socket(socket);
    server->accepteds.push_back(socket);
    /*   do something on accept connecctioen
    ex: send 220 FTP server is ready
    */
}



string tcp_linedialog_server::extract_command(string& buffer)
{   MUTEX_INSPECTOR;
    /*    	get line if available
    */


    if (!buffer.size())return "";

    int pz=buffer.find("\n");
    if(pz==-1) return "";
    string cmd=buffer.substr(0,pz);
    buffer=buffer.substr(pz+1, buffer.size()-pz-1);
    return cmd;
}


void tcp_linedialog_server::process_buffer(const PTR_getter<c_socket>& __SD)
{
    MUTEX_INSPECTOR;
    while (1)
    {

        std::string command;
        PTR_getter<c_socket> __Sk(__SD);

        {
            PGLW(c_socket,sd,__Sk);
            if (sd->rd_buf.size()) {
                command=extract_command(sd->rd_buf);
            }
        }

        if (command.size()) {
            process_command(command,__SD);
        }
        else
            break;
    }
}

void reply_second_data(const PTR_getter<c_socket>& __SD, const OCALC&  _data)
{
    if (__SD)
        reply_socket(__SD,_data);

}
void reply_second_data(const PTR_getter<c_socket>& __SD, const std::string & _data)
{

    if (__SD)
        reply_socket(__SD,_data);
}



void tcp_dialog_server::load_config(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m)
{   MUTEX_INSPECTOR;

    set_cfg_i(m,prefix+"dialog_handler_timeout_usec", pn,dialog_handler_timeout_usec,11111);
    set_cfg_i(m,prefix+"dialog_handler_cnt", pn,dialog_handler_cnt,7);
    set_cfg_i(m,prefix+"selector_timeout_usec", pn,selector_timeout_usec,1000);

#ifdef ANTIFLOOD_CHECK

    set_cfg_i(m,prefix+"antiflood_period", pn,antiflood_period,20);
    set_cfg_i(m,prefix+"antiflood_period_cnt", pn,antiflood_period_cnt,5);
#endif

}
void tcp_linedialog_server::start(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m)
{
    MUTEX_INSPECTOR;

    tcp_dialog_server::start(pn,prefix,m);
    load_config(pn,prefix,m);
}
void tcp_linedialog_server::load_config(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m)
{   MUTEX_INSPECTOR;

}
tcp_linedialog_server::tcp_linedialog_server()
{
}
