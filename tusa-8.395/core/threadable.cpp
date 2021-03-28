#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "threadable.h"
#include "errs.h"
#ifndef WIN32
#include <pthread.h>
#endif
#include "mutex_inspector.h"
#include "str_lib.h"
#include <errno.h>

#ifdef _WIN32
static M_MAP<pair<DWORD,HANDLE>, string> g_pths;
#else
static M_MAP<pthread_t, string> g_pths;
#endif
bool g_must_terminate=false;
bool threadable::SLEEP(int sec)
{
    //XTRY;

    sleep(sec);

    //XPASS;
    return false;
}
bool threadable::USLEEP(int usec)
{
    //XTRY;
    usleep(usec);
    //XPASS;
    return false;
}
threadable::~threadable()
{
}
threadable::threadable()
{
}
void threadable::create_thread(void * (*start_routine)(void *), const std::string & __name)
{
    MUTEX_INSPECTOR;
    XTRY;
#ifdef WIN32
    DWORD thread_id;
    HANDLE thread_handle=CreateThread (NULL, 0,(LPTHREAD_START_ROUTINE) start_routine, (void*)this, 0, &thread_id);
    g_pths.set(make_pair(thread_id,thread_handle),__name);

#else
    pthread_t pt;

    {
        int res = pthread_create(&pt, NULL, start_routine, (void *)this);

        if (res != 0) {
            string err=strerror(errno);
            throw cError("--Error: cant create pthread "+err+" "+__name+_DMI());
        }
        g_pths.set(pt,__name);
    }
#endif
    XPASS;

}
void threadable::kill_threads()
{
    MUTEX_INSPECTOR;


    g_must_terminate=true;
    g_kill_threads();

}
void g_create_thread(void * (*start_routine)(void *), void *P,const std::string & __name)
{
#ifdef WIN32
    DWORD thread_id;
    HANDLE thread_handle=CreateThread (NULL, 0,(LPTHREAD_START_ROUTINE) start_routine, (void*)P, 0, &thread_id);
    g_pths.set(pair<DWORD,HANDLE>(thread_id,thread_handle),__name);
#else
    pthread_t pt;
    logErr2("running thread 0x%x (%d) '%s'",pt,pt,__name.c_str());

    int res = pthread_create(&pt, NULL, start_routine, (void *)P);

    if (res != 0) {
        string err=strerror(errno);
        throw cError("--Error: cant create pthread "+err+" "+__name+_DMI());
        throw cError("--Error: cant create pthread "+__name+_DMI());
    }
#endif
}
void g_kill_threads()
{
    g_must_terminate=true;
#ifdef _WIN32
    map<pair<DWORD,HANDLE>, string> m=g_pths.extract_all();
    for (map<pair<DWORD,HANDLE>, string>::const_iterator i=m.begin(); i!=m.end(); i++)
#else
    map<pthread_t, string> m=g_pths.extract_all();
    for (map<pthread_t, string>::const_iterator i=m.begin(); i!=m.end(); i++)
#endif
    {
        logErr2("Teminating thread %s",i->second.c_str());
#ifdef _WIN32
        if(WaitForSingleObject(i->first.second,10000)==WAIT_TIMEOUT)
            logErr2("failed");
        else logErr2("success");
#else

#endif
    }

}
