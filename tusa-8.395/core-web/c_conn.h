#ifndef C_CONN__K
#define C_CONN__K
#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include <map>
#include "_mutex.h"
#include "m_var.h"
#include "PTR.h"


#ifdef WIN32
typedef int socklen_t;
#include <winsock.h>
#endif

#ifndef WIN32
#define SOCKET int
#define INVALID_SOCKET (-1)
#endif

#ifdef DEBUG
struct SOCKET_id
{
    int container;
    SOCKET_id():container(0) {}
};
/*namespace std{
	_STLP_TEMPLATE_NULL struct hash< SOCKET_id> {
		size_t operator()( SOCKET_id n) const { return CONTAINER(n); }
	};*/
//}
inline int operator < (const SOCKET_id& a,const SOCKET_id& b) {
    return a.container<b.container;
}
inline bool operator == (const SOCKET_id& a,const SOCKET_id& b) {
    return a.container==b.container;
}
inline bool operator != (const SOCKET_id& a,const SOCKET_id& b) {
    return a.container!=b.container;
}
#else
typedef unsigned int SOCKET_id;
#endif
class st_rsa;

class c_socket: public base_container
{
public:
    std::string peer_ipaddress;
    sockaddr_in peer_address;
    std::string rd_buf;
    std::string send_buf;
    int recv0;
    time_t create_time;

    const char *getclassname()const {
        return "c_socket";
    }
private:
    SOCKET m__sock;
    std::string name;

public:
    st_rsa *rsa;

    std::string extra_user_id;
    c_socket(SOCKET sock, const string& nm);
    virtual ~c_socket();

    std::string get_name()const;
    void set_name(const std::string&);

    SOCKET_id get_socket_fd() const;
    void disconnect__sk();
    bool connected() const;
    int connect(const struct sockaddr*,socklen_t);
    void sock_linger(time_t sec);

};
class out_oscar_buffer;
struct OCALC;
void reply_socket(const PTR_getter <c_socket> &__S, const std::string & _data);
void reply_socket(const PTR_getter <c_socket> &__S, const OCALC &_data);

std::string __gets(const PTR_getter <c_socket> &__S);
void disconnect(const PTR_getter <c_socket> &__S);
unsigned long gethostname(const char *hostname);
void handle_socket_error(const char *op,int _errno, const char * file, int line);
size_t S_recv(const PTR_getter<c_socket>&__S,void *buf, size_t len, int flags);

#ifdef _WIN32
struct pollfd
{
    SOCKET fd;                     /* File descriptor to poll.  */
    short int events;           /* Types of events poller cares about.  */
    short int revents;          /* Types of events that actually occurred.  */
};
static int poll (struct pollfd *__fds, int __nfds, int __timeout) {
    return 0;
}

#define POLLIN          0x001           /* There is data to read.  */
#define POLLPRI         0x002           /* There is urgent data to read.  */
#define POLLOUT         0x004           /* Writing now will not block.  */
#define POLLERR         0x008           /* Error condition.  */
#define POLLHUP         0x010           /* Hung up.  */
#define POLLNVAL        0x020           /* Invalid polling request.  */
#endif


#endif
