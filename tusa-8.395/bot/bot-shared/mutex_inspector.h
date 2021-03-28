/*
core
(c) belialov.com
*/
#ifndef _MINSTPKETR_HJ
#define _MINSTPKETR_HJ
#ifndef WIN32
#include <pthread.h>
#endif
#include <string>
#include <map>
#include <list>
#include <deque>
#include <stack>
#include "_mutex.h"
#include "errs.h"
using namespace std;
class mutex_inspector
{
public:
	mutex_inspector(const char* ff, int ll);
	mutex_inspector(const char* ff, int ll,const char *ss);
	~mutex_inspector();
};

#ifdef DEBUG
//#define _DMI() dump_mutex_inspector(pthread_self())
std::string dump_mutex_inspectors();
std::string dump_mutex_inspector(pthread_t pt);
class lockctl{
public:
lockctl(const char* f,int l);
~lockctl();
};
#else
//#define _DMI() std::string("")
#endif
#if defined(DEBUG) || defined(__STL_DEBUG)
#define MUTEX_INSPECTOR  mutex_inspector kall12344(__FILE__,__LINE__);
#define MUTEX_INSPECTORS(a)  mutex_inspector kall123444(__FILE__,__LINE__,a);
#else 
#define MUTEX_INSPECTOR
#define MUTEX_INSPECTORS(a)
#endif

#if defined(CORE_DEBUG)
#define CORE_MUTEX_INSPECTOR  mutex_inspector kall12344(__FILE__,__LINE__);
#define CORE_MUTEX_INSPECTORS(a)  mutex_inspector kall123444(__FILE__,__LINE__,a);
#else 
#define CORE_MUTEX_INSPECTOR
#define CORE_MUTEX_INSPECTORS(a)
#endif

void log_start_thread(const std::string &thread_name);


#endif
