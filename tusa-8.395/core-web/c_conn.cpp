#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef WIN32
#include <unistd.h>
#endif
#include <sys/types.h>
#ifndef WIN32
#include <sys/time.h>
#endif
#include "c_conn.h"
#include <stdio.h>
#include "tcp_stats.h"
#include "mutex_inspector.h"
#ifndef WIN32
#include <arpa/inet.h>
#endif
#include <set>
#include "str_lib.h"
using namespace std;

#include <map>
#include "st_stuff.h"
#include "tcp_sender.h"
#include "oscar_buffer.h"

#ifndef WIN32
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <errno.h>
#ifndef _WIN32
#include <poll.h>
#endif
#include "st_rsa.h"

using namespace std;
_mutex CMX;
string c_socket_dump()
{
    string out;

    return out;
}
bool readable_abs(const PTR_getter<c_socket> &__S,int millsec, bool ck_inbuffer);
void on_socket_close(SOCKET_id sock);
bool writeable_on_select(const PTR_getter<c_socket>& __S, int timeout_sec, int timeout_usec);
bool readable_abs_on_select(const PTR_getter<c_socket> &__S,int sec, int usec,const SOCKET_id& fd);


void disconnect(const PTR_getter <c_socket> &__S)
{
    MUTEX_INSPECTOR;
    PGLW(c_socket,s,__S);
    s->disconnect__sk();
}

void c_socket::disconnect__sk()
{
    MUTEX_INSPECTOR;
    //logErr2("socket disconnect %s",_DMI().c_str());
    if (m__sock!=-1)
    {


#ifdef _WIN32
        ::closesocket(m__sock);
#else
        ::close(m__sock);
#endif
        m__sock=-1;
    }
}
void c_conn_cpp() {}
m_var<long long> uniq_id_gen(0);
c_socket::c_socket(SOCKET s, const string& nm): m__sock(s),base_container("c_socket"),recv0(0),create_time(time(NULL)),name(nm),rsa(NULL)
{
}
c_socket::~c_socket()
{

    try {
        //logErr2("c_socket::~c_socket %s",_DMI().c_str());
        if(rsa) delete rsa;
        disconnect__sk();
    }
    catch (...) {}
}

bool c_socket::connected() const
{
    if (m__sock != -1) {
        return true;
    }
    return false;
}


size_t S_recv(const PTR_getter<c_socket>&__S,void *buf, size_t len, int flags)
{
    MUTEX_INSPECTOR;

    if (!__S)
    {
        logErr2("if(!__S)");
        return -1;
    }


    {
        PGLW(c_socket,s,__S);

        size_t rds=s->rd_buf.size();
        if (rds)
        {
            if (len>=rds)
            {
                memcpy(buf,s->rd_buf.data(),rds);
                s->rd_buf.clear();
                return rds;
            }
            else
            {
                memcpy(buf,s->rd_buf.data(),len);
                s->rd_buf=s->rd_buf.substr(len,s->rd_buf.size()-len);
                return len;

            }
        }
    }
    char b[4096*4];
    int ret;
    bool need_disconnect=false;
    if (readable_abs(__S,2,false))
    {
        {
            int __errno=0;
            {
                PGLW(c_socket,s,__S);
                SOCKET_id fd=s->get_socket_fd();
                if (CONTAINER(fd)==-1) return -1;
                ret=::recv(CONTAINER(fd),b, sizeof(b), flags);
                if (ret==-1)
                {

                    __errno=errno;
                    logErr2("recv failed %d %s",errno,strerror(errno));
                    handle_socket_error("recv__1",__errno,__FILE__,__LINE__);
                    switch (__errno)
                    {
                    case EAGAIN:
                        break;
#ifndef _WIN32

#endif

                    default:
                        need_disconnect=true;
                    }

                }
                else if (ret==0)
                {
//#ifdef KALL
                    s->recv0++;
                    if (s->recv0>10)need_disconnect=true;
                    logErr2("need_disconnect=true; %s %d",__FILE__,__LINE__);
//#endif

                }
                else
                {

                    s->recv0=0;
                }

            }
            if (need_disconnect)
            {
                disconnect(__S);
                return -1;
            }
            if (ret>0)tcp_total_recv+=ret;
            {
                if (__S)
                {

                    if (ret!=0 && ret!=-1)
                    {   PGLW(c_socket,s,__S);

                        s->rd_buf+=string(b,ret);
                        size_t rds=s->rd_buf.size();
                        if (rds)
                        {
                            if (len>=rds)
                            {

                                memcpy(buf,s->rd_buf.data(),rds);
                                s->rd_buf="";
                                return rds;
                            }
                            else
                            {

                                memcpy(buf,s->rd_buf.data(),len);
                                s->rd_buf=s->rd_buf.substr(len,s->rd_buf.size()-len);
                                return len;

                            }
                        }

                    }
                }
            }

        }
    }
    return 0;
}

SOCKET_id c_socket::get_socket_fd() const
{
    SOCKET_id s;
    CONTAINER(s)=m__sock;
    return s;
}
void c_socket::sock_linger(time_t sec)
{
#ifdef TCP_LINGERFERRR

    struct linger l;
    l.l_onoff=1;
    l.l_linger=(u_short)sec;
    if (::setsockopt(m__sock,SOL_SOCKET,SO_LINGER,
#ifndef WIN32
                     &l
#else
                     (const char*)&l
#endif
                     , sizeof(l))!=0)
    {
        throw cError("linger failed"+_DMI());
    }
#endif
}


void reply_socket(const PTR_getter <c_socket> &__S, const OCALC & _data)
{
    return reply_socket(__S,  _data.as_string());

}
void reply_socket(const PTR_getter <c_socket> &__S, const std::string & _data)
{
    write_sender_data(__S,_data);
}
std::string __gets(const PTR_getter <c_socket> &__S, const string &delim)
{
    MUTEX_INSPECTOR;
    SOCKET_id fd;
    {
        PGLW(c_socket,s,__S);
        fd=s->get_socket_fd();
        size_t pos=s->rd_buf.find(delim);
        if (pos!=-1)
        {
            string o=s->rd_buf.substr(0,pos);
            size_t delimsz=delim.size();
            s->rd_buf=s->rd_buf.substr(pos+delimsz,s->rd_buf.size()-(pos+delimsz));
            return o;
        }
    }
    if (CONTAINER(fd)==-1) {
        return "";
    }




    time_t __start_t=time(NULL);
    int recv0=0;
    while (1)
    {

        if (time(NULL)-__start_t>20) {
            return "";
            throw cError("break loop "+_DMI());
            return "";
        }
        if (readable_abs(__S,2,false))
        {
            char b[4096];
            int res;
            int __errno;
            SOCKET_id fd;
            {
                PGLW(c_socket,s,__S);
                fd=s->get_socket_fd();
                if (CONTAINER(fd)==-1)return "";
                res=::recv(CONTAINER(fd),b,sizeof(b),0);
                if (res>0)
                {

                    s->rd_buf+=string(b,res);
                    tcp_total_recv+=res;
                    __start_t=time(NULL);
                    size_t pos=s->rd_buf.find(delim);
                    if (pos!=-1)
                    {
                        string o=s->rd_buf.substr(0,pos);
                        size_t delimsz=delim.size();
                        s->rd_buf=s->rd_buf.substr(pos+delimsz,s->rd_buf.size()-(pos+delimsz));
                        return o;
                    }
                }
                if (res==-1)
                {
                    __errno=errno;
                    handle_socket_error("recv",__errno,__FILE__,__LINE__);

                    if (__errno==EAGAIN)
                    {
                        continue;
                    }
                    else
                    {
                        s->disconnect__sk();
                        return "";
                    }
                }

                if (res==0)
                {
                    recv0++;
                    if (recv0>10)
                    {
                        s->disconnect__sk();
                        return "";
                    }
                    continue;
                }
                else {
                    recv0=0;
                }

            }
        }
    }
    return "";
}






unsigned long gethostname(const char *hostname)
{   MUTEX_INSPECTOR;

    // try and resolve hostname
    struct hostent *hostEnt;
    if ((hostEnt = gethostbyname(hostname)) == NULL || hostEnt->h_addrtype != AF_INET) {
        throw cError("DNS lookup failed"+_DMI());
    } else {
        return *((unsigned long *)(hostEnt->h_addr));
    }
}

void handle_socket_error(const char *op,int _errno, const char * file, int line)
{
#ifndef _WIN32

#define E(aA) case aA: {\
	const char *z=#aA;\
	logErr2("socket io (%s): %s %s (%s %d)",op,z,strerror(aA),file,line);} break;

    switch (_errno)
    {
    case EAGAIN:
        break;

        E(EACCES);
        E(EBADF);
        E(ECONNREFUSED);
    case ECONNRESET:
        break;
        E(EDESTADDRREQ);
        E(EISCONN);
        E(EMSGSIZE);
        E(ENOBUFS);
        E(ENOTCONN);
        E(EFAULT);
        E(EINTR);
        E(EINVAL);
        E(ENOMEM);
        E(ENOTSOCK);
        E(EOPNOTSUPP);
    case EPIPE:
        break;
        E(ETIMEDOUT);
        E(EHOSTUNREACH);

    default:
        logErr2("unknown socket errno %d: %s",_errno,strerror(_errno));
    }
#endif
}



std::string c_socket::get_name()const {
    return name;
}
void c_socket::set_name(const string&n)
{
    name=n;

}

bool writeable_on_select(const PTR_getter<c_socket>& __S, int timeout_sec, int timeout_usec)
{
    int iii;
    {
        SOCKET_id fd;
        {
            PGLR(c_socket,s,__S);
            fd=s->get_socket_fd();
        }

        if(CONTAINER(fd)==INVALID_SOCKET)return false;
        fd_set  wrfs;
        FD_ZERO(&wrfs);
        FD_SET(CONTAINER(fd), &wrfs);
        struct timeval tv;
        tv.tv_sec = timeout_sec;
        tv.tv_usec = timeout_usec;

        iii = select(CONTAINER(fd) + 1, NULL, &wrfs, NULL, &tv);
        if(iii!=-1)
            return FD_ISSET(CONTAINER(fd), &wrfs);
    }
    if (iii == -1) {
        disconnect(__S);
        return false;
    }
    return false;


}

bool readable_abs_on_select(const PTR_getter<c_socket> &__S,int sec, int usec, const SOCKET_id &fd)
{
    MUTEX_INSPECTOR;

    int iii;
    {
        if(CONTAINER(fd)==INVALID_SOCKET)return false;
        fd_set rdfs;    /*for select */
        FD_ZERO(&rdfs);
        FD_SET(CONTAINER(fd), &rdfs);
        struct timeval tv;
        tv.tv_sec = sec;
        tv.tv_usec = usec;

        iii = select(CONTAINER(fd) + 1, &rdfs, NULL, NULL, &tv);
        if(iii!=-1)
            return FD_ISSET(CONTAINER(fd), &rdfs);
    }
    if (iii == -1) {
        disconnect(__S);
        return false;
    }
    return false;

}

bool readable_abs_on_poll(const PTR_getter<c_socket> &__S,int millisec, const SOCKET_id&fd);
bool readable_abs(const PTR_getter<c_socket> &__S,int millisec, bool ckIntBuf)
{
    SOCKET_id fd;
    {
        PGLW(c_socket,s,__S);
        if (ckIntBuf&&s->rd_buf.size()) return true;
        fd=s->get_socket_fd();
    }
#ifndef WIN32
    return readable_abs_on_poll(__S,millisec,fd);
#else
    return readable_abs_on_select(__S,0,millisec*1000,fd);
#endif

}
bool readable_abs_on_poll(const PTR_getter<c_socket> &__S,int millisec, const SOCKET_id&fd)
{
    MUTEX_INSPECTOR;

    {

        pollfd pollfds[1];
        pollfds[0].fd=CONTAINER(fd);
        pollfds[0].events=POLLIN;
        pollfds[0].revents=0;
        int ret=0;
        {
            ret=poll(pollfds,1,millisec);
        }
        if (ret==-1)
        {
            logErr2("poll returned -1 (%s) (%s %d)",strerror(errno),__FILE__,__LINE__);
        }
        else if (ret>0)
        {
            if (pollfds[0].revents & (POLLERR|POLLHUP|POLLNVAL))
            {
                disconnect(__S);
                return false;
            }
            if (pollfds[0].revents & (POLLIN|POLLPRI))
            {
                //			logErr2("pollfds[0].revents %d",pollfds[0].revents );
                return true;
            }

        }

    }
    return false;

}
