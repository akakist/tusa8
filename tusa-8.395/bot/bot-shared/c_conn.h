#ifndef C_CONN__K
#define C_CONN__K

#include <sys/types.h>
#include <sys/socket.h>
#include <map>
#include <string>

using namespace std;

class c_socket
{
public:
	int sock;
	int timeout;/*for select*/
	int recv_0_counter;
	c_socket();
	c_socket(int sock);
	c_socket(int sock, int timeout);
	virtual ~c_socket();
	void disconnect();
	virtual void on_disconnect();
	bool connected();
	bool readable();
	bool writeable();
	bool readable(time_t);
	bool writeable(time_t);
	int recv(void* buf,size_t len,int flags);
	int recvfrom(void* buf,size_t len,int flags,struct sockaddr* from,socklen_t *fromlen);
	int recvmsg(struct msghdr *msg,int flags);
	int send(const void* buf,size_t len,int flags);
	int sendto(const void* buf,size_t len,int flags,struct sockaddr* to,socklen_t tolen);
	int sendmsg(struct msghdr *msg,int flags);
	int close();
	int bind(struct sockaddr* myaddr,socklen_t addrlen);
	int listen(int backlog);
	int accept(struct sockaddr* addr,socklen_t *addrlen);
	int setsockopt(int,int,void*,socklen_t);
	int getsockopt(int,int,void*,socklen_t*);
	int connect(const struct sockaddr*,socklen_t);
	int puts(const string &);
	string gets();
	unsigned char getchar();
	
	int response(const string &);
};

#endif
