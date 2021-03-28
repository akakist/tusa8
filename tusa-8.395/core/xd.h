#ifndef _MINSTPKETR_HJ__999
#define _MINSTPKETR_HJ__999
#ifndef WIN32
#include <pthread.h>
#endif
#include <string>
#include <map>
#include <list>
#include <deque>
#include <stack>
#include "Mutex.h"
#include "Errs.h"
class mutex_inspector
{
public:
    mutex_inspector(const char* ff, int ll);
    mutex_inspector(const char* ff, int ll,const char *ss);
    ~mutex_inspector();
};
#define MUTEX_INSPECTOR  mutex_inspector kall12344(__FILE__,__LINE__);
#define MUTEX_INSPECTORS(a)  mutex_inspector kall123444(__FILE,__LINE__,a);
#define _DMI() dump_mutex_inspector(pthread_self())
std::string dump_mutex_inspectors();
std::string dump_mutex_inspector(pthread_t pt);




void log_start_thread(const std::string &thread_name);

#endif
