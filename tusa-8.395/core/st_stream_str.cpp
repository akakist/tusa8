

#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#ifndef WIN32
#include <sys/stat.h>
#endif
//#include "st_stuff.h"
#include "str_lib.h"
#include "errs.h"
#include <errno.h>

#include <errno.h>
#include "st_stream_str.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
void st_stream_str_cpp() {}
int st_stream_str::readsome (void *b, int n)
{
    string sn=container.substr(inidx,n);
    memcpy(b,sn.data(),sn.size());
    inidx+=sn.size();
    return sn.size();
}
int st_stream_str::writesome (const void *b, int n) const
{
    string *sc=(string *)&container;
    (*sc)+=string((char*)b,n);
    return n;
}
