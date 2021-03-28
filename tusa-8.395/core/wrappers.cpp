
#include "stdafx.h"
#include "mutex_inspector.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <stdlib.h>
#ifdef WIN32
#	include <process.h>
#	include "wrappers.h"
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <winsock.h>
#else
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <sys/stat.h>
#endif

#include <sys/types.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include "str_lib.h"
#include <fcntl.h>
#include "copy_file.h"
#ifndef WIN32
#	ifndef SOCKET_ERROR
#		define SOCKET_ERROR -1
#	else
#		error SOCKET_ERROR is already defined!!!
#	endif
#	ifndef INVALID_SOCKET
#		define INVALID_SOCKET -1
#	else
#		error INVALID_SOCKET is already defined!!!
#	endif
#endif

#include "wrappers.h"
#include <errno.h>
#include "tcp_stats.h"

void wrappers_cpp() {}













FILE *my_fopen(char *a, char *b)
{
#ifdef macintosh
    return fopen(a, b);
#else
    return ::fopen(a, b);
#endif
}

int my_fclose(FILE * f)
{
#ifdef macintosh
    return fclose(f);
#else
    return ::fclose(f);
#endif

}
size_t my_fread(void *a, int b, int c, FILE * d)
{

#ifdef macintosh
    return fread(a, b, c, d);
#else
    return ::fread(a, b, c, d);
#endif

}

size_t my_fwrite(const void *a, int b, int c, FILE * d)
{
#ifdef macintosh
    return fwrite(a, b, c, d);
#else
    return fwrite(a, b, c, d);
#endif

}

void my_mkdir(const string & path)
{
    int res;
#ifdef WIN32
    res = _mkdir(path.c_str());
#else
#if defined(macintosh)
    res = mkdir(to_mac_enc(path).c_str());
#else
    res = mkdir(path.c_str(), 0755);
#endif
#endif
    if (res) {
        throw cError("mkdir %s %s",path.c_str(),strerror(errno));
    }
}


int my_stat(const string & pathname, STAT * buf)
{
#if defined(macintosh)
    return stat(to_mac_enc(pathname).c_str(), buf);
#elif defined (WIN32)
    return ::_stat((char *) pathname.c_str(), buf);
#else
    return stat(pathname.c_str(), buf);
#endif
    return -1;
}
