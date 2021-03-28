
#ifdef __GNUC__
#pragma implementation				// gcc: implement sql_udf.h
#endif

#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef __WITH_LICENSE
#include <sys/types.h>
#include <vector>
#include <map>
#include <stdio.h>
#include "_mutex.h"
#include "errs.h"
#include "str_lib.h"
#include "st_stuff.h"
#include <errno.h>
#include "file_set.h"
//#include "st_stream_str.h"
#include "st_rsa.h"
#include "mutex_inspector.h"
#include "M_MAP.h"
#include "license.h"
void license_cpp() {}

char __S1[]="--Error: license failed, time expired";
char __S2[]="--Error: license failed, sign not matched";
char __S3[]="Unable load license";
char __S4[]="license signed OK";
char __S5[]="Server license have invalid sign";
char __S6[]="Server license expired";
char __S7[]="Server license not valid for host ";
char __S8[]="license failed";
char __S9[]="Server license not valid for port";
char __S10[]="Server license not valid for port %d";
char __S11[]="Server license have invalid application type";
char __S12[]="Server license expired";
char __S13[]="Server license signed wrong";
char __S14[]="feature not defined in license";
char __S15[]="License not specified in server.conf";
m_var<string> license_pathname;

_mutex headers_host_mx;
map<string,int> headers_host;
void license_add_host(const string&sh)
{
    M_LOCK(headers_host_mx);
    if(!headers_host.count(sh))
        headers_host[sh]=1;
    else
        headers_host[sh]++;

}
string license_get_max_host()
{
    M_LOCK(headers_host_mx);
    pair<int,string> max=make_pair(0,"");
    for(map<string,int>::iterator i=headers_host.begin(); i!=headers_host.end(); i++)
    {
        if(max.first<i->second) max=make_pair(i->second,i->first);
    }
    return max.second;
}

#endif
