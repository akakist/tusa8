#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <sys/stat.h>
#include "mutex_inspector.h"
#include "tcp_server_data.h"
#include "file_set.h"
#include <set>
#include "str_lib.h"
#include "tcp_sender.h"
#include "tcp_stats.h"
#include "M_SET.h"
#include "threadable.h"
#include <cerrno>
#include "c_conn.h"
#ifndef _WIN32
#include <poll.h>
#endif


bool destroy_sender=false;
bool disable_sender=false;
bool writeable_on_select(const PTR_getter<c_socket>& __S, int timeout_sec, int timeout_usec);

class S_EVENT
{

public:
    enum {
        ADD_DATA,
        REMOVE_SOCK,
    };
    int type;
    S_EVENT(int t):type(t) {}
    virtual ~S_EVENT() {}
};
class S_EVENT_ADD_DATA:public S_EVENT
{
public:
    PTR_getter<c_socket> __S;
    S_EVENT_ADD_DATA(const PTR_getter<c_socket> &Soc):S_EVENT(ADD_DATA),__S(Soc) {}
};
class S_EVENT_REMOVE_SOCK:public S_EVENT
{
public:
    SOCKET_id fd;
    S_EVENT_REMOVE_SOCK(const SOCKET_id &S):S_EVENT(REMOVE_SOCK),fd(S) {}
};




class sender2:public threadable
{

public:
    M_DEQUE<S_EVENT*> events;
private:


public:
    const char* getclassname()const {
        return "sender2";
    }

    sender2();
    void on_socket_create(SOCKET_id fd);
    void on_socket_close(SOCKET_id fd);
    void start(const string& pn,const string& prefix,map<string,string>&m);

    void write_data(const PTR_getter<c_socket> &socket, const string& buf);
    void load_config(const string& pn,const string& prefix,map<string,string>&m);
    static void *sender_proc(void *pr);
    unsigned int timeout_usec;

};
static sender2 *__sender2=NULL;

static m_var<unsigned int> senderMap_count;
string sender2_dump()
{
    string out;
    if(__sender2)
    {
        out+="sender2::events.size() "+std::to_string(__sender2->events.size())+"<br>\n";
        out+="sender2::senderMap_count "+std::to_string(senderMap_count)+"<br>\n";
    }
    return out;
}

#ifdef WIN32
__declspec(dllexport)
#endif
void init_senders(const string& pn,const string& prefix,map<string,string>&m)
{
    MUTEX_INSPECTOR;
    __sender2=new sender2();
    LEAKCTL_ADD(__sender2);
    __sender2->start(pn,prefix,m);
}
void hup_senders(const string& pn,const string& prefix,map<string,string>&m)
{
    MUTEX_INSPECTOR;
    if (__sender2)__sender2->load_config(pn,prefix,m);
}
void destroy_senders()
{
    MUTEX_INSPECTOR;
    if(__sender2)
    {
        __sender2->kill_threads();
        LEAKCTL_REMOVE(__sender2);
        delete __sender2;
        __sender2=NULL;
    }
}
void write_sender_data(const PTR_getter<c_socket> &socket, const string& buf)
{
    MUTEX_INSPECTOR;
    if (__sender2)
    {
        __sender2->write_data(socket, buf);
    }
}

void on_socket_create(SOCKET_id sock)
{
    if (__sender2) __sender2->on_socket_create(sock);
}
void on_socket_close(SOCKET_id sock)
{
    if (__sender2) __sender2->on_socket_close(sock);
}
void sender2::start(const string& pn,const string& prefix,map<string,string>&m)
{
    MUTEX_INSPECTOR;
    load_config(pn,prefix,m);
    create_thread(sender_proc,"sender_proc");
}
void sender2::write_data(const PTR_getter<c_socket> &socket, const string& _buf)
{
    MUTEX_INSPECTOR;
    if(!socket)
    {
        logErr2("write data to non socket %s",_DMI().c_str());
        return;
    }
    {
        PGLW(c_socket,s,socket);
        s->send_buf+=_buf;
    }
    events.push_back(new S_EVENT_ADD_DATA(socket));

}
sender2::sender2()
{

}
void sender2::on_socket_create(SOCKET_id fd)
{
    events.push_back(new S_EVENT_REMOVE_SOCK(fd));
}
void sender2::on_socket_close(SOCKET_id fd)
{
    events.push_back(new S_EVENT_REMOVE_SOCK(fd));
}
void sender2::load_config(const string& pn,const string& prefix,map<string,string>&m)
{   MUTEX_INSPECTOR;

    set_cfg_i(m,prefix+"timeout_usec", pn,timeout_usec, 30000);
}
void cleanup_sockets(map<SOCKET_id,PTR_getter<c_socket> > &ccc)
{
    MUTEX_INSPECTOR;
    time_t cur_t=time(NULL);
    vector<SOCKET_id> rm;
    for(map<SOCKET_id,PTR_getter<c_socket> >::iterator i=ccc.begin(); i!=ccc.end(); i++)
    {

        {
            MUTEX_INSPECTOR;
            PGLW(c_socket,s,i->second)
            {
                if(CONTAINER(i->first)==-1 || i->first!=s->get_socket_fd())
                {
                    MUTEX_INSPECTOR;
                    rm.push_back(i->first);
                    s->disconnect__sk();
                }
            }
        }
    }
    for(unsigned i=0; i<rm.size(); i++)
    {
        MUTEX_INSPECTOR;
        ccc.erase(rm[i]);
    }

}
void handle_events(map<SOCKET_id,PTR_getter<c_socket> > &ccc,sender2* snd)
{
    deque<S_EVENT*>d =snd->events.extract_all();
    while(d.size())
    {
        MUTEX_INSPECTOR;
        S_EVENT *e=*d.begin();
        d.pop_front();
        try {
            if(e->type==S_EVENT::ADD_DATA)
            {
                S_EVENT_ADD_DATA *ev=(S_EVENT_ADD_DATA *) e;
                SOCKET_id fd;
                {
                    PGLW(c_socket,s,ev->__S);
                    fd=s->get_socket_fd();
                }
                ccc.insert(make_pair(fd,ev->__S));
            }
            if(e->type==S_EVENT::REMOVE_SOCK)
            {
                S_EVENT_REMOVE_SOCK *ev=(S_EVENT_REMOVE_SOCK *)e;
                ccc.erase(ev->fd);
            }

        }
        STDCATCHS("sender_proc");
        delete e;

    }
}
void send_to_socket(map<SOCKET_id,PTR_getter<c_socket> > &ccc,sender2* snd,PTR_getter<c_socket>  __S,const SOCKET_id &fd)
{
    PGLW(c_socket,s,__S);
    if(s->get_socket_fd()!=fd)
    {
        ccc.erase(fd);
        return;
        //continue;
    }
    int res;
    if(disable_sender)
    {
        res= s->send_buf.size();
    }
    else
    {
#ifdef WIN32
        res=::send(CONTAINER(fd),(char*)s->send_buf.data(), s->send_buf.size(),0);
#else
        res=::send(CONTAINER(fd),(char*)s->send_buf.data(), s->send_buf.size(),MSG_NOSIGNAL | MSG_DONTWAIT);
#endif
        TRACEDBG(logErr2("send ret %d",res));
    }
    if (res!=-1)
    {
        MUTEX_INSPECTOR;
        tcp_total_send += res;
        if (res!=s->send_buf.size())
        {
            s->send_buf=s->send_buf.substr(res,s->send_buf.size()-res);
        }
        else
        {
            s->send_buf="";
            ccc.erase(fd);
            return;
        }
    }
    else
    {
        MUTEX_INSPECTOR;
        int __errno=errno;
        if(__errno!=EBADF)
        {
            handle_socket_error("send",__errno,__FILE__,__LINE__);
        }
        if(__errno!=EAGAIN)
        {
            MUTEX_INSPECTOR;
            s->disconnect__sk();
            ccc.erase(fd);
            return;
        }
    }

}
void do_poll_and_send(map<SOCKET_id,PTR_getter<c_socket> > &ccc,sender2* snd)
{
#ifdef _WIN32
    pollfd pollfds[1024];
#else
    pollfd pollfds[ccc.size()+1];
#endif
    int idx=0;
    bool full=rand()%100==0;
    for(map<SOCKET_id,PTR_getter<c_socket> >::iterator i=ccc.begin(); i!=ccc.end(); i++)
    {
        MUTEX_INSPECTOR;
        pollfds[idx].fd=CONTAINER(i->first);
        pollfds[idx].events=POLLOUT;
        pollfds[idx].revents=0;
        idx++;

    }
    if (idx)
    {
        MUTEX_INSPECTOR;
        int ret;
        {
            ret=poll(pollfds,idx,5);
        }
        if (ret==-1)
        {
            logErr2("poll returned -1 (%s) (%s %d)",strerror(errno),__FILE__,__LINE__);
        }
        else if(ret>0)
        {


            {
                MUTEX_INSPECTOR;
                for (int ii=0; ii<idx; ii++)
                {
                    SOCKET_id fd;
                    CONTAINER(fd)=pollfds[ii].fd;

                    if (pollfds[ii].revents & (POLLERR|POLLHUP|POLLNVAL))
                    {
                        MUTEX_INSPECTOR;
                        map<SOCKET_id,PTR_getter<c_socket> > ::iterator it=ccc.find(fd);
                        if(it!=ccc.end())
                        {
                            MUTEX_INSPECTOR;
                            PGLW(c_socket,s,it->second);
                            s->disconnect__sk();
                        }
                        ccc.erase(fd);
                        continue;
                    }


                    if ((pollfds[ii].revents & POLLOUT) || full)
                    {
                        map<SOCKET_id,PTR_getter<c_socket> > ::iterator it=ccc.find(fd);
                        if(it!=ccc.end())
                        {
                            MUTEX_INSPECTOR;
                            send_to_socket(ccc,snd,it->second,fd);

                        }
                    }
                }
            }
        }
    }

}
void do_select_and_send(map<SOCKET_id,PTR_getter<c_socket> > &ccc,sender2* snd)
{
    int idx=0;
    bool full=rand()%100==0;
    vector<pair<SOCKET_id,PTR_getter<c_socket> > > tosend;
    for(map<SOCKET_id,PTR_getter<c_socket> >::iterator i=ccc.begin(); i!=ccc.end(); i++)
    {
        SOCKET_id fd;
        size_t ssize=0;
        {
            PGLR(c_socket,s,i->second);
            fd=s->get_socket_fd();
            ssize=s->send_buf.size();
        }
        if(ssize)
        {
            if(writeable_on_select(i->second,0,0))
            {
                tosend.push_back(make_pair(fd,i->second));
                send_to_socket(ccc,snd,i->second,fd);
            }
        }
    }
    for(unsigned i=0; i<tosend.size(); i++)
        send_to_socket(ccc,snd,tosend[i].second,tosend[i].first);

}


void *sender2::sender_proc(void *pr)
{
    MUTEX_INSPECTOR;

    sender2 *snd=(sender2*)pr;
    string thread_name="sender2::sender_proc";
    log_start_thread(thread_name);
    map<SOCKET_id,PTR_getter<c_socket> > ccc;
    while (1) {
        try {
            if(g_must_terminate) return NULL;
            if(destroy_sender) return NULL;
            senderMap_count=ccc.size();

            if(rand()%10000==0)
            {
                cleanup_sockets(ccc);
            }
            if(g_must_terminate) return NULL;
            handle_events(ccc,snd);
#ifndef WIN32
            do_poll_and_send(ccc,snd);
#else
            do_select_and_send(ccc,snd);
#endif
            //
        }
#ifndef WIN32
        STDCATCHS("sender while(1) STDCATCHS");
#else
        catch(...) {}
#endif
        {
            if (snd->USLEEP(snd->timeout_usec))return NULL;
        }
    }
    return NULL;
}
