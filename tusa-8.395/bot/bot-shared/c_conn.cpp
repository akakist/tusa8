#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include "c_conn.h"
#include <stdio.h>
#include <string>
#include "errs.h"
#include "errno.h"
#include "mutex_inspector.h"

void c_conn_cpp(){}
c_socket::c_socket(): sock(-1),timeout(20),recv_0_counter(0)
{
#ifdef USE_SSL
	ssl = NULL;
	use_ssl = false;
#endif
}

c_socket::c_socket(int s):sock(s),timeout(20)
{
#ifdef USE_SSL
	ssl = NULL;
	use_ssl = false;
#endif
}

c_socket::c_socket(int s,int t):sock(s),timeout(t)
{
#ifdef USE_SSL
	ssl = NULL;
	use_ssl = false;
#endif
}

c_socket::~c_socket()
{
	disconnect();
}

bool c_socket::connected()
{MUTEX_INSPECTOR;
	if (sock != -1) {
		return true;
	}
	return false;
}
int c_socket::close()
{MUTEX_INSPECTOR;
	int rv=0;
	if (sock != -1) {
#ifdef WIN32
		rv=::closesocket(sock);
#else
		rv=::close(sock);
#endif
		sock = -1;
	}
	return rv;
}

void c_socket::disconnect()
{MUTEX_INSPECTOR;
	if (sock != -1) {
		close();
#ifdef USE_SSL
		if (use_ssl && ssl)
			SSL_free(ssl);
#endif
		sock = -1;
	}
}

#ifdef USE_SSL
#endif

bool c_socket::readable(time_t t)
{MUTEX_INSPECTOR;
	fd_set rdfs;	/*for select */
	if(sock==-1)return false;
#ifdef USE_SSL
	if (use_ssl) {
		if (SSL_pending(ssl)) {
			return true;
		}
	}
#endif

	FD_ZERO(&rdfs);
	FD_SET(sock, &rdfs);
	struct timeval tv;
	tv.tv_sec = t;
	tv.tv_usec = 0;
	int iii;
	{
		iii = select(sock + 1, &rdfs, NULL, NULL, &tv);
	}
	if (iii == -1) {
		return false;
	}
	return FD_ISSET(sock, &rdfs);
}

bool c_socket::writeable(time_t t)
{MUTEX_INSPECTOR;

	if(sock==-1)return false;
	fd_set  wrfs;	/*for select */
	FD_ZERO(&wrfs);
	FD_SET(sock, &wrfs);
	struct timeval tv;
	tv.tv_sec = t;
	tv.tv_usec = 0;
	int iii;
	{
		iii = select(sock + 1, NULL, &wrfs, NULL, &tv);
	}
	if (iii == -1) {
		return false;
	}
	return FD_ISSET(sock, &wrfs);
}
void c_socket::on_disconnect(){

}

bool c_socket::readable()
{MUTEX_INSPECTOR;
	if(sock==-1)throw socketError("sock==-1");
	return readable(timeout);
}
bool c_socket::writeable()
{MUTEX_INSPECTOR;
	if(sock==-1)throw socketError("sock==-1");
	return writeable(timeout);
}

int c_socket::recv(void *buf, size_t len, int flags)
{MUTEX_INSPECTOR;
	if(sock==-1)throw socketError("sock==-1");
	int rc=0;
#ifndef WIN32
	rc=::recv(sock,buf, len, flags);
#else
	rc=::recv(sock,(char*)buf, len, flags);
#endif
	if(rc==0) recv_0_counter++;
	else recv_0_counter=0;
	if(recv_0_counter>10) throw socketError("connection reset by peer");
	return rc;
}
int c_socket::recvfrom(void *buf, size_t len, int flags, struct sockaddr *from, socklen_t * fromlen)
{MUTEX_INSPECTOR;
	if(sock==-1)throw socketError("sock==-1");
#ifndef WIN32
	return ::recvfrom(sock,buf, len, flags, from, fromlen);
#else
	return ::recvfrom(sock,(char*)buf, len, flags, from, fromlen);
#endif
}
#ifndef WIN32
int c_socket::recvmsg(struct msghdr *msg, int flags)
{MUTEX_INSPECTOR;
	if(sock==-1)throw socketError("sock==-1");
	return ::recvmsg(sock,msg,flags);
}
#endif
int c_socket::send(const void *b, size_t len, int flags)
{MUTEX_INSPECTOR;
	if(sock==-1)throw socketError("sock==-1");
	int offs=0;
	char *buf=(char*)b;
	while(1)
	{
	    
	int r=
#ifndef WIN32
	 ::send(sock,&buf[offs], len-offs,flags);
#else
	 ::send(sock,(char*)&buf[offs], len-offs,flags);
#endif
	    if(r==-1) throw socketError("send ret -1");
	    if(r==len-offs) break;
	    else offs+=r;
	}
}
/*int c_socket::sendto(const void *buf, size_t len, int flags, struct sockaddr *to, socklen_t tolen)
{MUTEX_INSPECTOR;
	if(sock==-1)throw socketError("sock==-1");
#ifndef WIN32
	return ::sendto(sock,buf, len, flags, to, tolen);
#else
	return ::sendto(sock,(char*)buf, len, flags, to, tolen);
#endif
}
*/
#ifndef WIN32
/*int c_socket::sendmsg(struct msghdr *msg, int flags)
{MUTEX_INSPECTOR;
	if(sock==-1)throw socketError("sock==-1");
	return ::sendmsg(sock,msg,flags);
}*/
#endif
int c_socket::bind(struct sockaddr *myaddr, socklen_t addrlen)
{MUTEX_INSPECTOR;
	if(sock==-1)throw socketError("sock==-1");
	return ::bind(sock,myaddr, addrlen);
}
int c_socket::listen(int backlog)
{MUTEX_INSPECTOR;
	if(sock==-1)throw socketError("sock==-1");
	return ::listen(sock, backlog);
}
int c_socket::accept(struct sockaddr *addr, socklen_t * addrlen)
{MUTEX_INSPECTOR;
	if(sock==-1)throw socketError("sock==-1");
		return ::accept(sock,addr, addrlen);
}
int c_socket::setsockopt(int a, int b, void *c, socklen_t d)
{MUTEX_INSPECTOR;
	if(sock==-1)throw socketError("sock==-1");
#ifndef WIN32
	return	::setsockopt(sock,a,b,c,d);
#else
	return	::setsockopt(sock,a,b,(char*)c,d);
#endif
}
int c_socket::getsockopt(int a, int b, void *c, socklen_t* d)
{MUTEX_INSPECTOR;
	if(sock==-1)throw socketError("sock==-1");
#ifndef WIN32
	return	::getsockopt(sock,a,b,c,d);
#else
	return	::getsockopt(sock,a,b,(char*)c,d);
#endif
}

int c_socket::connect(const struct sockaddr* a,socklen_t b)
{MUTEX_INSPECTOR;
	if(sock==-1)throw socketError("sock==-1");
	int z=::connect(sock,a,b);
	if(z!=0) throw_cErrorAPI("connect");
	return z;
}

int c_socket::puts(const string &s)
{MUTEX_INSPECTOR;
	int z=send(s.c_str(),s.size(),0);
	if(z==-1) throw_cErrorAPI("puts");
	return z;
}

unsigned char c_socket::getchar()
{MUTEX_INSPECTOR;
	unsigned char s[4];
	int z=recv((char *)s, 1, 0);
	if(z==-1) throw_cErrorAPI("getc");
	return s[0];
}

string c_socket::gets(){MUTEX_INSPECTOR;
	string out;
	char c = 0;
	while (c != 10 /*\n */ ) {
		c=getchar();
		if (c != 10 && c != 13)
		out += c;
	}
	return out;
}

int c_socket::response(const string &s)
{MUTEX_INSPECTOR;
#ifdef DEBUG
	printf("|<< %s\n",s.c_str());
#endif
	logErr("|<< %s",s.c_str());
	return puts(s+"\n");
}
