/*
core
(c) belialov.com
*/
//#include "stdafx.h"
#include <list>
#include "errs.h"
#include "_mutex.h"
#include "mutex_inspector.h"
#include <stdio.h>
#include <errno.h>
#include <ctime>
#include "str_lib.h"
#ifdef WIN32
#define snprintf _snprintf
#endif

#define MUTEXLOCK_TIMEOUT 50
void _mutex_cpp(){}
#ifndef WIN32
using namespace std;
/*
	declare not declared in std headers
*/
extern "C" int pthread_mutexattr_settype (pthread_mutexattr_t *__attr, int __kind);
#endif
_mutex::_mutex()
{
// init mutex
#ifdef WIN32
	InitializeCriticalSection((CRITICAL_SECTION *) & m_);
#else
	pthread_mutexattr_t mt;
	pthread_mutexattr_init(&mt);
#ifndef __FreeBSD__
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

_mutex::~_mutex()
{
// destroy mutex
#ifdef WIN32
DeleteCriticalSection((CRITICAL_SECTION*)&m_);
#else	

	int res = pthread_mutex_destroy(&m_);
	if (res)
	{
		char s[100];
		snprintf(s,sizeof(s)-1,"pthread_mutex_destroy:  %p ",&m_);
		throw cError((string)s+_DMI());
	}
#endif		
}

void _mutex::lock()
{
// lock mutex
#ifndef WIN32
#	ifdef DEBUG
// cyclic try lock. If exceeded timeout throw to check dead locks
	time_t start_t=time(NULL);
	while(trylock())
	{
		usleep(1000);
		if(time(NULL)-start_t>MUTEXLOCK_TIMEOUT)
		{
			char s[300];
			snprintf(s,sizeof(s)-1,"MUTEXLOCK_TIMEOUT %p: ",&m_);
			string ss=(string)s+_DMI()+"\nMREPORT_END";
			throw cError(ss);
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
int _mutex::trylock() 
{
/*
	try lock. Only unix version available
*/
	int res=pthread_mutex_trylock((pthread_mutex_t*)&m_);
   	return res;
}
#endif

void _mutex::unlock()
{
// mutex unlock
#ifdef WIN32
	LeaveCriticalSection((CRITICAL_SECTION*)&m_);
#else	
	pthread_mutex_unlock((pthread_mutex_t*)&m_);
#endif
}

#ifdef WIN32
/* win32 wrappers*/
pthread_t pthread_self(){return (pthread_t)GetCurrentThreadId();}
void usleep(unsigned long l){Sleep(l/1000);}

int pthread_create(pthread_t * pt, void *attr, void *(f)
		     (void *), void *par)
{

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	DWORD tid;
	HANDLE ret = CreateThread(&sa,	// pointer to security attributes
				  0,	// initial thread stack size
				  (unsigned long (__stdcall *) (void *))f,	// pointer to thread function
				  par,	// argument for new thread
				  0,	// creation flags
				  &tid	// pointer to receive thread ID
		);

	if (ret == NULL)
		return 1;
	else
		return 0;
	return 0;
}
void sleep(unsigned long i)
{
	Sleep(i*1000);
}

#endif
