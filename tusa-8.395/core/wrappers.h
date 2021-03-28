#ifndef wrappers_H_
#define wrappers_H_
#include <stdio.h>
#ifndef WIN32
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include <string>
using namespace std;
#ifdef WIN32
typedef struct _stat STAT;
#else
typedef struct stat STAT;
#ifndef SOCKET
#define SOCKET int
#endif
#endif


#ifdef WIN32
extern "C"
{

    int __ms_socket(int a, int b, int c);
    int __ms_getsockname(int s, struct sockaddr *addr, int *len);
    int __ms_listen(int s, int bl);
    int __ms_recvfrom(int s, char *buf, int len, int flags, struct sockaddr *to, int *tolen);
    int __ms_sendto(int s, char *buf, int len, int flags, struct sockaddr *to, int tolen);
    int __ms_recv(int s, char *buf, int len, int flags);
    int __ms_send(int s, char *buf, int len, int flags);
    int __ms_accept(int s, struct sockaddr *sa, int *len);
    int __ms_bind(int s, const struct sockaddr *sa, int len);
    int __ms_connect(int s, const struct sockaddr *sa, int len);
    int __ms_closesocket(int s);
    int __ms_fseek(FILE *f, long l, int t);
    int __ms_fwrite(const void *a, int b, int c, FILE *d);
    int __ms_fread(void *a, int b, int c, FILE *d);
    int __ms_fclose(FILE *f);
    FILE * __ms_fopen(char *a, char *b);

}
#endif

int my_connect(int s, const struct sockaddr *sa, int len);
int my_bind(int s, const struct sockaddr *sa, int len);
int my_accept(int s, struct sockaddr *sa, int *len);
int my_send(int s, char *buf, int len, int flags);
int my_recv(int s, char *buf, int len, int flags);
int my_sendto(int s, char *buf, int len, int flags, struct sockaddr *to, int tolen);
int my_recvfrom(int s, char *buf, int len, int flags, struct sockaddr *to, int *tolen);
int my_listen(int s, int bl);
int my_getsockname(int s, struct sockaddr *addr, int *len);
int my_closesocket(int s);

int my_fseek(FILE *f, long l, int t);
size_t my_fwrite(const void *a, int b, int c, FILE *d);
size_t my_fread(void *a, int b, int c, FILE *d);
int my_fclose(FILE *f);
FILE * my_fopen(char *a, char *b);
void my_mkdir(const string& path);
string my_getcwd();
int my_chdir(const string &s);
int my_stat(const string& pathname, STAT *buf);
#endif
