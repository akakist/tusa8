#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <list>
#include "errs.h"
#include "_mutex.h"
#include "_mutex1.h"
#include "mutex_inspector.h"
#include <stdio.h>
#include <errno.h>
#include <ctime>
#include "str_lib.h"
#ifdef WIN32
#define snprintf _snprintf
#endif
using namespace std;

/* declare not declared in std headers
*/
#ifndef WIN32
extern "C" int pthread_mutexattr_settype (pthread_mutexattr_t *__attr, int __kind);
#endif


#define MUTEXLOCK_TIMEOUT 50
_mutex1::_mutex1()
{
#ifdef WIN32
    InitializeCriticalSection((CRITICAL_SECTION*)&m_);
#else
    pthread_mutexattr_t mt;
    pthread_mutexattr_init(&mt);
#if !defined __FreeBSD__ && !defined __MACH__
    pthread_mutexattr_settype(&mt, PTHREAD_MUTEX_RECURSIVE_NP);
#else
    pthread_mutexattr_settype(&mt, PTHREAD_MUTEX_RECURSIVE);
#endif

#ifndef _MSL_NO_EXCEPTIONS
    if (pthread_mutex_init(&m_, &mt))
        throw bad_alloc();
#else
    pthread_mutex_init(&m_, &mt);
#endif
    pthread_mutexattr_destroy(&mt);
#endif
}
_mutex1::~_mutex1()
{
#ifdef WIN32
    DeleteCriticalSection((CRITICAL_SECTION*)&m_);
#else

    int res = pthread_mutex_destroy(&m_);
    if (res)
    {
        char s[100];
        snprintf(s,sizeof(s)-1,"pthread_mutex_destroy:  %p ",&m_);
        throw cError(string(s)+_DMI());
    }
#endif
}
void _mutex1::lock()
{
// lock mutex
#ifndef WIN32
#	ifdef USE_MUTEX_TRYLOCK
// cyclic try lock. If exceeded timeout throw to check dead locks
    time_t start_t=time(NULL);
    while(trylock())
    {
        usleep(1000);
        if(time(NULL)-start_t>MUTEXLOCK_TIMEOUT)
        {
#ifdef DEBUG
            char s[300];
            snprintf(s,sizeof(s)-1,"MUTEXLOCK_TIMEOUT \n ");
            string ss=(string)s+dump_mutex_inspectors()+"\nMREPORT_END";
            throw cError(ss);
#else
            throw cError("MUTEXLOCK_TIMEOUT");
#endif

        }
    }
#	else
    int res=pthread_mutex_lock((pthread_mutex_t*)&m_);
    if(res) throw cError("--Error: pthread_mutex_lock returns "+itostring(res));
#	endif
#else
    EnterCriticalSection((CRITICAL_SECTION*)&m_);
#endif
}
#ifndef WIN32
int _mutex1::trylock()
{
    int res=pthread_mutex_trylock((pthread_mutex_t*)&m_);
    return res;
}
#endif
void _mutex1::unlock()
{
#ifdef WIN32
    LeaveCriticalSection((CRITICAL_SECTION*)&m_);
#else
    pthread_mutex_unlock((pthread_mutex_t*)&m_);
#endif
}
