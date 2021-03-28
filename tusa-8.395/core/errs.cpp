#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "errs.h"
#include "str_lib.h"
#include "_mutex.h"
#include <cstdarg>
#include <ctime>
#include "m_var.h"
#include "server_config.h"
#include "mutex_inspector.h"
#include <stdio.h>
#include "st_stuff.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

void errs_cpp() {}
static _mutex _localtime_MX("_localtime_MX");
struct tm LOCALTIME(time_t timep)
{
    MUTEX_LOCK aa(_localtime_MX);
    struct tm *p=localtime(&timep);
    return *p;
}


_mutex log_mutex("log_mutex");

static FILE *log_f2=NULL;
static time_t open_f_time2;
static string log_fname2;
m_var<string> __APPNAME;
string log_name(const string& prefix)
{
    MUTEX_INSPECTOR;
    string logname = prefix+"."+__APPNAME.get();
    tm tt=LOCALTIME(time(NULL));
    char ss[200];
    snprintf(ss,sizeof(ss)-1,".%04d%02d%02d",1900+tt.tm_year,tt.tm_mon+1,tt.tm_mday);
    logname+=ss;
    return logname;
}
#ifdef WIN32
__declspec (dllexport)
#endif
void logErr(const char *fmt, ...)
{
    /*log format into log file _c_log_name
    */


#ifndef WIN32
    MUTEX_TRY_LOCK kall(log_mutex);
    if (!kall.result) {
#else
    M_LOCK(log_mutex);
    if(1) {
#endif
        string ln=log_name("log");
        va_list argptr;
        va_start(argptr, fmt);
        if(log_f2 && (time(NULL)-open_f_time2>1000 || log_fname2!=ln))
        {
            fclose(log_f2);
            log_f2=NULL;
        }
        if(!log_f2)
        {



            log_f2 = ::fopen(log_name("log").c_str(), "a");
            log_fname2=ln;
            open_f_time2=time(NULL);
        }
        if (log_f2) {
            time_t t = time(NULL);
            string ss = ctime(&t);
            if (ss.size())
            {   
                fprintf(log_f2, "%s: ", ss.substr(0, ss.size() - 1).c_str());
            }
            {   
                vfprintf(log_f2, fmt, argptr);
            }
            {   
                fprintf(log_f2, "\n");
            }
            ::fflush(log_f2);
        }
    }

}
#ifdef WIN32
__declspec (dllexport)
#endif
m_var<string> last_msg;
void logErr2(const char *fmt, ...)
{
    /*log format into log file _c_log_name and into stdout
    */
#ifndef WIN32
    MUTEX_TRY_LOCK kall(log_mutex);
    if (!kall.result) {
#else
//	M_LOCK(log_mutex);
    if (1) {
#endif
        string logname=log_name("log");
        if(log_f2 && ( time(NULL)-open_f_time2>1000 ||log_fname2!=logname))
        {
            fclose(log_f2);
            log_f2=NULL;
        }
        if(!log_f2)
        {
            log_f2 = ::fopen(log_name("log").c_str(), "a");
            log_fname2=logname;
            open_f_time2=time(NULL);

        }
        if (log_f2) {
            time_t t = time(NULL);
            string ss = ::ctime(&t);
            if (ss.size())
            {
                ::fprintf(log_f2, "%s: ", ss.substr(0, ss.size() - 1).c_str());
            }
            {
                va_list argptr;
                va_start(argptr, fmt);
                ::vfprintf(log_f2, fmt, argptr);
                va_end(argptr);
            }
            {
                ::fprintf(log_f2, "\n");
                ::fflush(log_f2);
            }
        }
        {

            va_list argptr;
            va_start(argptr, fmt);
            ::vfprintf(stderr,fmt, argptr);
            fprintf(stderr,"\n");
            va_end(argptr);
        }
    }
}

void logErrN(const char* prefix,const char *fmt, ...)
{

    st_FILE f(log_name(prefix),"a");


    if (f.get_descriptor())
    {
        time_t t = time(NULL);
        string ss = ::ctime(&t);
        if (ss.size())
        {
            ::fprintf(f.get_descriptor(), "%s: ", ss.substr(0, ss.size() - 1).c_str());
        }
        {
            va_list argptr;
            va_start(argptr, fmt);
            ::vfprintf(f.get_descriptor(), fmt, argptr);
            va_end(argptr);
        }
        {
            ::fprintf(f.get_descriptor(), "\n");

        }
    }
    { 

        va_list argptr;
        va_start(argptr, fmt);
        ::vfprintf(stderr,fmt, argptr);
        fprintf(stderr,"\n");
        va_end(argptr);
    }

}

cError::cError(const char *_message, ...) {
    va_list list;
    char buff[1024];

    va_start(list, _message);
    vsnprintf(buff, sizeof(buff), _message, list);
    va_end(list);
#ifdef DEBUG
    logErr2("%s",buff);
#endif
    error_string = buff;

}


cError::cError(const string & e)
{

    error_string = e;
#ifdef DEBUG
    logErr2("%s",e.c_str());
#endif
}

